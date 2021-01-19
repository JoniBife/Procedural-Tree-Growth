#ifndef TREE_H
#define TREE_H

#include <map> 
#include "GrowthParameters.h"
#include "BranchModule.h"
#include "../meshes/Mesh.h"
#include "../textures/Texture2D.h"
#include <list>

class Tree {

public:
	BranchModule* root = nullptr;
	std::vector<BranchModule*> modules;
	std::list<BranchModule*> tips;
	SceneNode* sceneNode = nullptr;
	
	Tree(BranchModule* root);

	~Tree();

public:
	void grow(float elapsedTime);

private:
	// Methods related to the extended borchet-honda method at tree scale
	void accumulateModuleLightExposure(BranchModule* module);
	void calculateVigorFluxes(BranchModule* root);
	void distributeLightAndVigor();

	void developModules(float elapsedTime);

};

#endif
