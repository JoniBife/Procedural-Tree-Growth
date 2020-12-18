#include "BranchModule.h"
#include "Morphospace.h"
#include <numeric>

BranchModule::BranchModule(BranchNode* root) : root(root) {}

BranchModule::~BranchModule() {
	for (BranchModule* child : children) {
		delete child;
	}
}

void BranchModule::updateModule(float elapsedTime) {

	GrowthParameters* growthParameters = GrowthParameters::instance;

	// Firstly we update the growth rate from the vigour which we previously calculated
	growthRate = eqt::growthRate(vigour, growthParameters->vMin, growthParameters->vMax, growthParameters->gP);

	// Secondly we update the module physiological age
	physiologicalAge += eqt::ageVariation(elapsedTime, growthRate);

	// Then we update the diameter and the length of each of the segments of the module
	root->updateNode(physiologicalAge);

	calculateCenterOfGeometry();

	if (reachedMatureAge(root)) {
		/*while (!tips.empty()) {
			BranchNode* tip = tips.back();
			tips.pop_back();
			attachModule(tip);
		}*/
		//float lightExposureTerminalNodes = lightExposure / (tips.size());
	}

	for (BranchModule* child : children) {
		child->updateModule(elapsedTime);
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
	float determinacyMS = eqt::determinacyMS(vigour, growthParameters->determinacy, growthParameters->vRootMax);

	// Selecting new module from morphospace
	BranchModule* module = morphospace->selectModule(growthParameters->apicalControl, determinacyMS, root);
	module->root = root;
	module->parent = this;
	children.push_back(module);

	//module->root = root;
	//module->orientation = orientation;
	//module->parent = this;
	//module->main = this->main;
	//module->tree = tree;
}

Vec3 maxPos = { FLT_MIN, FLT_MIN, FLT_MIN };
Vec3 minPos = { FLT_MAX, FLT_MAX, FLT_MAX };



void calculateCenterOfGeometryRecurs(BranchNode* node, Vec3 position) {

	Vec3 realPosition = position + node->relativePosition * (node->branchLength/node->maxBranchLength);

	if (node->children.size() == 0) {

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
	} else {
		for (BranchNode* child : node->children) {
			calculateCenterOfGeometryRecurs(child, realPosition);
		}
	}
}

void BranchModule::calculateCenterOfGeometry() {
	calculateCenterOfGeometryRecurs(root->children[0], root->relativePosition);

	Vec3 center = (maxPos + minPos) * 0.5f;

	Vec3 diff = maxPos - minPos;

	float radius = diff.x / 2;

	if (diff.y > radius)
		radius = diff.y / 2;
	
	if (diff.z > radius)
		radius = diff.z / 2;

	boundingSphere.center = center;
	boundingSphere.radius = radius;
}



