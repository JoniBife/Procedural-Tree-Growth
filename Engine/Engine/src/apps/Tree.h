#ifndef TREE_H
#define TREE_H

#include <map> 
#include "GrowthParameters.h"
#include "BranchModule.h"
#include "../meshes/Mesh.h"
#include "../textures/Texture2D.h"

class Tree {

public:
	GrowthParameters* growthParameters;
	BranchModule* root; 
	float currTime;
	//std::map<int, float> intersections; // map that contains all the intersections volumes between spheres
	std::vector<BranchModule*> modules;
	std::vector<BranchModule*> tips;
	Mesh* cylinder;
	
	Tree(const Vec3& positionRoot, SceneGraph* sceneGraph, Texture2D* woodTexture, Texture2D* woodNormalMap);

	~Tree();

public:
	void startGrowth(GrowthParameters* growthParameters);
	void pauseGrowth();
	void resumeGrowth();

	void grow(float elapsedTime);

private:
	// Methods related to the extended borchet-honda method at tree scale
	void accumulateModuleLightExposure(BranchModule* module);
	void calculateVigorFluxes(BranchModule* root);
	void distributeLightAndVigor();

	void developModules(float elapsedTime);

};

#endif
