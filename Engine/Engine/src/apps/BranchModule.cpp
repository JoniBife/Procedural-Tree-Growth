#include "BranchModule.h"
#include "Morphospace.h"
#include "../utils/OpenGLUtils.h"


BranchModule::BranchModule(BranchNode* root) : root(root) {}

BranchModule::~BranchModule() {
	for (BranchModule* child : children) {
		delete child;
	}
}

void BranchModule::updateModule(float elapsedTime, std::vector<Vec4>& vertices) {

	if (!reachedMaturity) {
		GrowthParameters* growthParameters = GrowthParameters::instance;

		// Firstly we update the growth rate from the vigour which we previously calculated
		growthRate = eqt::growthRate(vigour, growthParameters->vMin, growthParameters->vMax, growthParameters->gP);

		// Secondly we update the module physiological age
		float ageVariation = eqt::ageVariation(elapsedTime, growthRate);
		physiologicalAge += ageVariation;

	}

	// Then we update the diameter and the length of each of the segments of the module
	Vec4 zeroVec(0.0f, 0.0f, 0.0f, 0.0f);
	root->updateNode(physiologicalAge, vertices, zeroVec, true);

	if (!reachedMaturity) {
		calculateCenterOfGeometry();
		reachedMaturity = reachedMatureAge(root);
	} else {
		if (!tips.empty()) {
			distributeLightAndVigor();
			
			float vMin = GrowthParameters::instance->vMin;

			std::vector<BranchNode*> toRemove;

			for (BranchNode*& tip : tips) {
				if (tree->modules.size() == GrowthParameters::instance->maxModules) {
					break;
				}
				if (tip->vigour > vMin) {
					attachModule(tip);
					toRemove.push_back(tip);
				}
			}

			for (BranchNode*& tip : toRemove) {
				tips.remove(tip);
			}
		}

		for (BranchModule* child : children) {
			child->updateModule(elapsedTime, vertices);
		}
	}
}

bool BranchModule::reachedMatureAge(BranchNode* branch) {

	if (branch->children.size() == 0)
		return branch->branchLength == branch->maxBranchLength;

	bool childrenReachedMaturity = true;

	for (BranchNode* child : branch->children)
	{
		if (child->branchLength != child->maxBranchLength)
			return false;
		childrenReachedMaturity &= reachedMatureAge(child);
	}

	return childrenReachedMaturity;
}

void BranchModule::attachModule(BranchNode*& root) {

	Morphospace* morphospace = Morphospace::instance;
	GrowthParameters* growthParameters = GrowthParameters::instance;

	bool main = false;
	if (root->main) {
		main = true;
	}

	float determinacyMS = eqt::determinacyMS(vigour, growthParameters->determinacy, (float)growthParameters->vMax);

	// Selecting new module from morphospace
	BranchModule* module = morphospace->selectModule(growthParameters->apicalControl, determinacyMS, root, true);
	module->main = main;

	Mat4 orientation = Qtrn::fromDir(root->relativePosition.normalize()).toRotationMatrix() * Mat4::rotation(randomFloat(0.0f, PI * 2), Vec3::Y);

	module->setOrientation(orientation);

	module->vigour = vigour;
	module->parent = this;
	module->physiologicalAge = root->physiologicalAge;
	module->calculateCenterOfGeometry();
	module->tree = tree;
	children.push_back(module);
	tree->modules.push_back(module);
	tree->tips.push_back(module);

	// This module is no longer a tip so we remove it from the list
	if (isTip) {
		tree->tips.remove(this);
		isTip = false;
	}
}

void BranchModule::calculateCenterOfGeometryRecurs(BranchNode* node, Vec3 position, std::vector<Vec3>& currentTips) {

	Vec3 realPosition = position + node->relativePosition * (node->branchLength/node->maxBranchLength);

	if (node->children.size() == 0 || node->branchLength < node->maxBranchLength || node->isTip) {

		if (realPosition.x > maxPos.x)
			maxPos.x = realPosition.x;
		else if (realPosition.x < minPos.x)
			minPos.x = realPosition.x;

		if (realPosition.y > maxPos.y)
			maxPos.y = realPosition.y;
		else if (realPosition.y < minPos.y)
			minPos.y = realPosition.y;

		if (realPosition.z > maxPos.z)
			maxPos.z = realPosition.z;
		else if (realPosition.z < minPos.z)
			minPos.z = realPosition.z;

		currentTips.push_back(realPosition);
	} else {
		for (BranchNode* child : node->children) {
			calculateCenterOfGeometryRecurs(child, realPosition, currentTips);
		}
	}
}

void BranchModule::calculateCenterOfGeometry() {

	std::vector<Vec3> currentTips;
	Vec3 rootPosition = Vec3(root->positionWithDiameter.x, root->positionWithDiameter.y, root->positionWithDiameter.z);
	currentTips.push_back(rootPosition);

	if (rootPosition.x > maxPos.x)
		maxPos.x = rootPosition.x;
	else if (rootPosition.x < minPos.x)
		minPos.x = rootPosition.x;

	if (rootPosition.y > maxPos.y)
		maxPos.y = rootPosition.y;
	else if (rootPosition.y < minPos.y)
		minPos.y = rootPosition.y;

	if (rootPosition.z > maxPos.z)
		maxPos.z = rootPosition.z;
	else if (rootPosition.z < minPos.z)
		minPos.z = rootPosition.z;

	calculateCenterOfGeometryRecurs(root->children[0], rootPosition, currentTips);

	Vec3 center = (maxPos + minPos) * 0.5f;

	float radius = 0.0f;

	for (Vec3 tip : currentTips) {
		Vec3 diff = tip - center;
		float sqrdMagnitude = diff.sqrMagnitude();
		if (sqrdMagnitude > radius)
			radius = sqrdMagnitude;
	}

	boundingSphere.center = center;
	boundingSphere.radius = sqrtf(radius);
}

void BranchModule::accumulateNodeLightExposure(BranchNode* node) {
	if (!node->parent)
		return;

	node->parent->lightExposure += node->lightExposure;

	if (node->parent == root)
		return;

	accumulateNodeLightExposure(node->parent);
}

void BranchModule::calculateVigorFluxes(BranchNode* root) {
	if (root->children.size() == 0 || (root->isTip && root != this->root))
		return;

	float sumLateralLightExposure = 0.0f;
	float mainLightExposure = 0.0f;

	// First we calculate the sum of lateral nodes light exposure and the main light exposure
	for (BranchNode* child : root->children) {

		// Is child a lateral branch
		if (!child->main)
			sumLateralLightExposure += child->lightExposure;
		else
			mainLightExposure = child->lightExposure;
	}

	// Secondly we calculate the vigor of the main node using equation 2, and the lateral vigor
	float vigorMain = eqt::vigor(root->vigour, GrowthParameters::instance->apicalControl, mainLightExposure, sumLateralLightExposure);
	float vigorLateral = root->vigour - vigorMain;

	// Finally we distribute the lateral vigor correctly using the porportional amount of light exposure they provided to equation 2
	for (BranchNode* child : root->children) {
		// Is child a lateral branch
		if (!child->main) {
			// Each lateral node takes a percentage of the lateral vigor
			child->vigour = (child->lightExposure / sumLateralLightExposure) * vigorLateral;
		}
		else
			child->vigour = vigorMain;

		// The child then propagates its vigor to his children
		calculateVigorFluxes(child);
	}
	
}

void BranchModule::distributeLightAndVigor() {
	float lightExposureTerminalNodes = lightExposure / (tips.size());

	// We do a basipetal (tip-to-base) pass from tips to root accumulating the value of total light exposure
	for (BranchNode* tip : tips) {
		tip->lightExposure = lightExposureTerminalNodes;
		accumulateNodeLightExposure(tip);
	}

	// Finally we do an acropetal (base-to-tip) pass calculating vigor fluxes at each node intersection
	calculateVigorFluxes(root);
}

void BranchModule::setOrientation(Mat4& orientation) {

	for (BranchNode* child : root->children)
	{
		setOrientationRecurs(orientation, child);
	}
	//setOrientationRecurs(orientation, root->children[0]);
}

void BranchModule::setOrientationRecurs(Mat4& orientation, BranchNode* curr) {

	Vec4 relativePosition = curr->relativePosition.toVec4();
	relativePosition.w = 1.0f;
	relativePosition = orientation * relativePosition;

	curr->relativePosition = Vec3(relativePosition.x, relativePosition.y, relativePosition.z);

	if (curr->isTip) {
		return;
	}
	for (BranchNode* child : curr->children)
	{
		setOrientationRecurs(orientation, child);
	}
}

void BranchModule::generateLeaves(SceneGraph* sceneGraph, Leaves* leaves) {
	leaves->removeLeaves();
	generateLeavesRecursively(sceneGraph, leaves, root, true);
}

void BranchModule::generateLeavesRecursively(SceneGraph* sceneGraph, Leaves* leaves, BranchNode* curr, bool isRoot) {
	
	// If we are in a tip we create the leaves scene node
	if (!isRoot && (curr->children.size() == 0 || !curr->reachedMax)) {

		for (int i = 0; i < 2; ++i) {
			float randomScale = randomFloat(1.0f, 5.0f);

			Vec3 translation = Vec3(curr->positionWithDiameter.x, curr->positionWithDiameter.y, curr->positionWithDiameter.z);
			Mat4 orientation = Mat4::translation(translation)
				* Qtrn::fromDir(curr->relativePosition.normalize()).toRotationMatrix()
				* Mat4::rotation(randomFloat(0.0f, 2 * PI), Vec3::Y)
				* Mat4::rotation(-PI / 2, Vec3::X)
				* Mat4::scaling({ randomScale, 0.0f, randomScale});
			SceneNode* sceneNodeLeaves = sceneGraph->getRoot()->createChild(leaves->quad, orientation, leaves->shader);
			sceneNodeLeaves->setBeforeDrawFunction([=](ShaderProgram* sp) {
				glEnable(GL_BLEND);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
				glBlendEquation(GL_FUNC_ADD);
				GL_CALL(glDisable(GL_CULL_FACE));
			});
			sceneNodeLeaves->setAfterDrawFunction([=]() {
				GL_CALL(glEnable(GL_CULL_FACE));
				GL_CALL(glDisable(GL_BLEND));
			});
			sceneNodeLeaves->addTexture(leaves->texture);
			sceneNodeLeaves->addTexture(leaves->depthMap);
			leaves->sceneNodes.push_back(sceneNodeLeaves);
		}
		return;
	}

	for (BranchNode* child : curr->children) {
		generateLeavesRecursively(sceneGraph, leaves, child);
	}
}




