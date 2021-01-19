#ifndef BRANCH_MODULE_H
#define BRANCH_MODULE_H

#include "BranchNode.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../apps/BoundingSphere.h"
#include "Equations.h"
#include <numeric>
#include "Leaves.h"

class Tree;

struct BranchModule {

public:
	float lightExposure = 0.0f;
	float vigour = 0.0f;
	float growthRate = 0.0f; // Upsilon in the paper
	float physiologicalAge = 0.0f;

	bool main = false;
	bool reachedMaturity = false;

	Vec3 orientation; // Orientation is represented using Euler Angles (yaw, pitch, roll)

	BranchModule* parent;
	std::vector<BranchModule*> children;

	BoundingSphere boundingSphere;

	Tree* tree;

	BranchNode* root;
	std::vector<BranchNode*> tips;

	BranchModule(BranchNode* root);

	~BranchModule();

	void updateModule(float elapsedTime, std::vector<Vec4>& vertices);

	bool reachedMatureAge(BranchNode* branch);

	void attachModule(BranchNode*& root);

	Vec3 maxPos = { FLT_MIN, FLT_MIN, FLT_MIN };
	Vec3 minPos = { FLT_MAX, FLT_MAX, FLT_MAX };

	void calculateCenterOfGeometryRecurs(BranchNode* node, Vec3 position, std::vector<Vec3>& currentTips);
	void calculateCenterOfGeometry();

	// Methods related to the extended borchet-honda method at module scale
	void accumulateNodeLightExposure(BranchNode* node);
	void calculateVigorFluxes(BranchNode* root);
	void distributeLightAndVigor();

	void setOrientation(Mat4& orientation);

	void generateLeaves(SceneGraph* sceneGraph, Leaves* leaves);

private:

	void generateLeavesRecursively(SceneGraph* sceneGraph, Leaves* leaves, BranchNode* curr, bool isRoot = false);

	void setOrientationRecurs(Mat4& orientation, BranchNode* curr);

};

#include "Tree.h"

#endif

