#include "BranchModule.h"
#include "Morphospace.h"


BranchModule::BranchModule(BranchNode* root) : root(root) {}

BranchModule::~BranchModule() {
	for (BranchModule* child : children) {
		delete child;
	}
}

void BranchModule::updateModule(float elapsedTime) {

	if (!reachedMaturity) {
		GrowthParameters* growthParameters = GrowthParameters::instance;

		// Firstly we update the growth rate from the vigour which we previously calculated
		growthRate = eqt::growthRate(vigour, growthParameters->vMin, growthParameters->vMax, growthParameters->gP);

		// Secondly we update the module physiological age
		float ageVariation = eqt::ageVariation(elapsedTime, growthRate);
		physiologicalAge += ageVariation;

	}

	// Then we update the diameter and the length of each of the segments of the module
	root->children[0]->updateNode(physiologicalAge);


	if (!reachedMaturity) {
		adapt();
	
		calculateCenterOfGeometry();
		reachedMaturity = reachedMatureAge(root);
	}


	for (BranchModule* child : children) {
		child->updateModule(elapsedTime);
	}

	// We are not orienting the modules so until next delivery we don't attach any
	if (reachedMaturity && !tips.empty()) {

		distributeLightAndVigor();

		float vMin = GrowthParameters::instance->vMin;

		for (BranchNode* tip : tips) {
			if (tip->vigour > vMin && tip->children.size() == 0) {
				attachModule(tip);
			}
		}
	}
}

void BranchModule::adapt() { root->adapt(); }

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

void BranchModule::attachModule(BranchNode* root) {

	Morphospace* morphospace = Morphospace::instance;
	GrowthParameters* growthParameters = GrowthParameters::instance;

	// TODO For now we assume 
	float determinacyMS = eqt::determinacyMS(vigour, growthParameters->determinacy, (float)growthParameters->vRootMax);

	// Selecting new module from morphospace
	BranchModule* module = morphospace->selectModule(growthParameters->apicalControl, determinacyMS, root);
	module->root = root;
	module->parent = this;
	module->tree = tree;
	module->calculateCenterOfGeometry();
	module->physiologicalAge = 0.0f;
	module->setOrientation(root->rotation); // We set the module orientation to the orientation of the segment where it is attached
	children.push_back(module);
	tree->modules.push_back(module);
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
	Vec3 rootPosition = root->calculatePosition();
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
	if (root->children.size() == 0 || root->isTip)
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
	setOrientationRecurs(orientation, root->children[0]);
}

void BranchModule::setOrientationRecurs(Mat4& orientation, BranchNode* curr) {

	curr->moduleOrientation = orientation;

	if (curr->isTip) {
		return;
	}
	for (BranchNode* child : curr->children)
	{
		setOrientationRecurs(orientation, child);
	}
}




