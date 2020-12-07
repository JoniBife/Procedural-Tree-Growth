/*#ifndef TREE_H
#define TREE_H

#include "GrowthParameters.h"
#include "BranchModule.h"

class Tree {

private:
	GrowthParameters* growthParameters;
	BranchModule* root; 
	float currTime;

public:
	void startGrowth(GrowthParameters* growthParameters);
	void pauseGrowth();
	void resumeGrowth();

private:
	void distributeLightAndVigour();

	void developModules();

};

#endif*/
