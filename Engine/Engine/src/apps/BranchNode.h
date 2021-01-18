#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H

#include <vector>
#include "../math/Vec4.h"
#include "../scene/SceneGraph.h"
#include "Equations.h"
#include "GrowthParameters.h"
#include "../math/MathAux.h"
#include "../math/Qtrn.h"

/*
* Represents a node within a branch module
* acyclic graph.
*/
struct BranchNode {

	Vec4 positionWithDiameter;

	Vec3 relativePosition; // The position of the node relative to its parent
	Vec3 adaptationOffset = { 0.0f, 5.0f, 0.0f }; // An offset that is added to the branch node position to simulate the adaptation of the module
	Vec3 currRelativePosition;
	float physiologicalAge = 0.0f;
	float branchLength = 0.0f;
	float branchDiameter;
	float maxBranchLength;

	Mat4 rotation; // The rotation matrix created from the dir of the branch segment associated with this node

	Mat4 moduleOrientation; // The orientation of the module where this node is contained

	float lightExposure;
	float vigour;

	bool isTip;
	bool main = false;
	
	BranchNode* parent;
	std::vector<BranchNode*> children;

	bool reachedMax = false;

	~BranchNode();

	void updateNode(float modulePhysiologicalAge, std::vector<Vec4>& vertices, Vec4& parentPosition, bool isRoot);

	// We calculate the position of this node relative to the position of the parents
	// So until it reaches the root, all the positions are relatives positions to each other.
	Vec3 calculatePosition();

	BranchNode* createChild(const Vec3& relativePosition, float scaleLength, bool isTip = false);

	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor, float lerpFactor, bool first = true);

	// Equation 8 of the paper
	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor);
};

#endif
