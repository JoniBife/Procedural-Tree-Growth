#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H

#include <vector>
#include "../math/Vec4.h"
#include "../scene/SceneGraph.h"
#include "Equations.h"
#include "GrowthParameters.h"

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

	GrowthParameters* growthParameters;
	SceneNode* sceneGraphNode; // Contains the mesh that is the cylinder that extends from the parent's position to this position
	
	BranchNode* parent;
	std::vector<BranchNode*> children;

	void updateNode(float modulePhysiologicalAge) {

		if (physiologicalAge > modulePhysiologicalAge)
			return;

		if (parent) {
			// First we calculate the segment physiological age to be used in the branch length
			float branchSegmentAge = segmentPhysiologicalAge(modulePhysiologicalAge, parent->physiologicalAge);

			// Secondly we calculate the diameter
			branchDiameter = segmentDiameter(this, growthParameters->thickeningFactor);

			// Thirdly we calculate the branch length
			float prevBranchLength = branchLength;
			branchLength = segmentLength(maxBranchLength, branchSegmentAge, growthParameters->scalingCoefficient);

			// We then scale the cylinder and translate it upwards so that its base stays in the same position
			Mat4 scaling = Mat4::scaling({ branchDiameter, branchLength, branchDiameter});
			Mat4 translation = Mat4::translation({ 0.0f, branchLength/2, 0.0f });

			Vec3 dir = (relativePosition - parent->relativePosition).normalize();

			float yaw = atan2f(dir.z, dir.x);
			float pitch = asinf(-dir.y);
			Mat4 rotation = Mat4::rotation(pitch, Vec3::X)* Mat4::rotation(yaw, Vec3::Y);

			// Finally we translate the node to its correct position in world space (TODO CHECK IF THIS IS WORLD SPACE)
			Mat4 positioning = Mat4::translation(parent->calculatePosition());

			sceneGraphNode->setModel(rotation * translation * scaling);
		}

		for (BranchNode* child : children) {
			child->updateNode(modulePhysiologicalAge);
		}
	}

	// When we adapt a node, we also have to adapt its children
	void adapt(Vec3 adaptation) {
		relativePosition += adaptation;
		for (BranchNode* child : children)
		{
			child->adapt(adaptation);
		}
	}


	// We calculate the position of this node relative to the position of the parents
	// So until it reaches the root, all the positions are relatives positions to each other.
	Vec3 calculatePosition() {
		if (!parent)
			return relativePosition;
		return relativePosition + parent->calculatePosition();
	}

	// Should return a pointer to himself
	BranchNode* createChild(const Vec3& relativePosition) {
		BranchNode* child = new BranchNode();
		child->relativePosition = relativePosition;
		child->parent = this;
		child->maxBranchLength = (this->relativePosition - relativePosition).magnitude();
		children.push_back(child);
		return child;
	}

	// Equation 8 of the paper
	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor) {
		if (branchNode->children.size() > 0) {
			float branchSegmentDiameter = 0;
			for (BranchNode* child : branchNode->children) {
				branchSegmentDiameter += segmentDiameter(child, thickeningFactor);
			}
			return sqrtf(branchSegmentDiameter * branchSegmentDiameter);
		}
		return thickeningFactor;
	}
};

#endif
