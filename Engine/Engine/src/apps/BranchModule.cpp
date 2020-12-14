#include "BranchModule.h"

void BranchModule::updateModule(float elapsedTime) {

	// Firstly we update the growth rate from the vigour which we previously calculated
	//growthRate = eqt::growthRate(vigour, ,,tree->growthParameters->gP);

	// Secondly we update the module physiological age
	physiologicalAge += eqt::ageVariation(elapsedTime, growthRate);

	// Then we update the diameter and the length of each of the segments of the module
	root->updateNode(physiologicalAge);

	if (reachedMatureAge(root)) {
		//BranchNode* tip = tips.back();
		//tips.pop_back();
		//attachModule(tip);
		//float lightExposureTerminalNodes = lightExposure / (tips.size());
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

BranchModule* BranchModule::attachModule(BranchNode* root) {
	BranchModule* module = new BranchModule();
	module->root = root;
	//module->orientation = orientation;
	module->parent = this;
	module->main = this->main;
	//module->tree = tree;
}