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

	Vec3 relativePosition; // The position of the node relative to its parent
	float physiologicalAge = 0.0f;
	float branchLength = 0.0f;
	float branchDiameter;
	float maxBranchLength;

	float lightExposure;
	float vigour;

	bool isTip;
	bool main = false;

	SceneNode* sceneGraphNode; // Contains the mesh that is the cylinder that extends from the parent's position to this position
	
	BranchNode* parent;
	std::vector<BranchNode*> children;

	int reachedMax = 0;

	~BranchNode();

	void updateNode(float modulePhysiologicalAge);

	// When we adapt a node, we also have to adapt its children
	void adapt();

	// We calculate the position of this node relative to the position of the parents
	// So until it reaches the root, all the positions are relatives positions to each other.
	Vec3 calculatePosition();

	BranchNode* createChild(const Vec3& relativePosition, float scaleLength, bool isTip = false);

	
	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor, float lerpFactor, bool first = true);

	// Equation 8 of the paper
	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor);
};

#endif
