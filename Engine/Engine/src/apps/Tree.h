#ifndef TREE_H
#define TREE_H

#include <map> 
#include "GrowthParameters.h"
#include "BranchModule.h"

class Tree {

public:
	GrowthParameters* growthParameters;
	BranchModule* root; 
	float currTime;
	//std::map<int, float> intersections; // map that contains all the intersections volumes between spheres
	std::vector<BranchModule*> modules;
	std::vector<BranchModule*> tips;

public:
	void startGrowth(GrowthParameters* growthParameters);
	void pauseGrowth();
	void resumeGrowth();

	void grow(float elapsedTime);

private:
	void accumulateModuleLightExposure(BranchModule* module);
	void calculateVigorFluxes(BranchModule* root);
	void distributeLightAndVigor();

	void developModules(float elapsedTime);

};

#endif
