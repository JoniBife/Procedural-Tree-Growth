#ifndef BRANCH_MODULE_H
#define BRANCH_MODULE_H

#include "BranchNode.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../apps/BoundingSphere.h"
#include "Equations.h"
//#include "Tree.h"

struct BranchModule {

	float lightExposure;
	float vigour;
	float growthRate; // Upsilon in the paper
	float physiologicalAge = 0.0f;

	bool main = false;

	Vec3 orientation; // Orientation is represented using Euler Angles (yaw, pitch, roll)

	BranchModule* parent;
	std::vector<BranchModule*> children;

	BoundingSphere boundingSphere;

	//Tree* tree; // The tree where this module is contained

	BranchNode* root;
	std::vector<BranchNode*> tips;

	void updateModule(float elapsedTime);

	void adapt();

	bool reachedMatureAge(BranchNode* branch);

	BranchModule* attachModule(BranchNode* root);

};

#endif

