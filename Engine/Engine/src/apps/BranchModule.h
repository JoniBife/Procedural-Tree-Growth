#ifndef BRANCH_MODULE_H
#define BRANCH_MODULE_H

#include "BranchNode.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../apps/BoundingSphere.h"
#include "Equations.h"
//#include "Tree.h"

struct BranchModule {

public:
	float lightExposure;
	float vigour;
	float growthRate; // Upsilon in the paper
	float physiologicalAge = 0.0f;

	bool main = false;

	Vec3 orientation; // Orientation is represented using Euler Angles (yaw, pitch, roll)

	BranchModule* parent;
	std::vector<BranchModule*> children;

	BoundingSphere boundingSphere;

	BranchNode* root;
	std::vector<BranchNode*> tips;

	BranchModule(BranchNode* root);

	~BranchModule();

	void updateModule(float elapsedTime);

	void adapt();

	bool reachedMatureAge(BranchNode* branch);

	void attachModule(BranchNode* root);

	void calculateCenterOfGeometry();

};

#endif

