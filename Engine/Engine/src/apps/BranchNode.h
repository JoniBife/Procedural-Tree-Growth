#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H

#include <vector>
#include "../math/Vec4.h"
#include "../scene/SceneGraph.h"
#include "Equations.h"
#include "GrowthParameters.h"
#include "../math/MathAux.h"

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

	Mat4 translation;
	Mat4 rotation;

	GrowthParameters* growthParameters;
	SceneNode* sceneGraphNode; // Contains the mesh that is the cylinder that extends from the parent's position to this position
	
	BranchNode* parent;
	std::vector<BranchNode*> children;

	int reachedMax = 0;

	void updateNode(float modulePhysiologicalAge) {

		if (physiologicalAge > modulePhysiologicalAge)
			return;

		if (parent) {
			// First we calculate the segment physiological age to be used in the branch length
			float branchSegmentAge = segmentPhysiologicalAge(modulePhysiologicalAge, parent->physiologicalAge);

			// Thirdly we calculate the branch length
			float prevBranchLength = branchLength;
			branchLength = segmentLength(maxBranchLength, branchSegmentAge, growthParameters->scalingCoefficient);

			// Secondly we calculate the diameter
			branchDiameter = segmentDiameter(this, growthParameters->thickeningFactor,branchLength/maxBranchLength);

			// We then scale the cylinder and translate it upwards so that its base stays in the same position
			Mat4 scaling = Mat4::scaling({ branchDiameter, branchLength, branchDiameter });
			translation = Mat4::translation({ 0.0f, branchLength / 2, 0.0f });

			Vec3 parentPosition = parent->calculatePosition();
			Vec3 dir = ((parentPosition + relativePosition) - parentPosition).normalize();

			// double yaw = Math.Atan2(ds.X, ds.Y);double pitch = Math.Atan2(ds.Z, Math.Sqrt((ds.X * ds.X) + (ds.Y * ds.Y)));	

			//float yaw = atan2f(-dir.x, dir.y);
			//float pitch = atan2f(fsignf(dir.y) *dir.z, sqrtf((dir.x*dir.x) + (dir.y *dir.y)));
			//float yaw = atan2f(-dir.x, dir.z);
			//float pitch = atan2f(dir.z, sqrtf((dir.x * dir.x) + (dir.y * dir.y)));
			rotation = Mat4::rotationFromDir(dir);

			// Finally we translate the node to its correct position in world space (TODO CHECK IF THIS IS WORLD SPACE)
			Mat4 positioning = Mat4::translation(parent->calculatePosition());
			
			sceneGraphNode->setModel(positioning * rotation * translation * scaling);

			if (branchLength == maxBranchLength) {
				++reachedMax;
				if (reachedMax == 1) {
					physiologicalAge = modulePhysiologicalAge;
				}
			}
		}

		if (branchLength == maxBranchLength) {
			for (BranchNode* child : children) {
				child->updateNode(modulePhysiologicalAge);
			}
		}
	}

	// When we adapt a node, we also have to adapt its children
	void adapt() {

		if (parent) {

			Vec3 gravityDir = -1 * Vec3::Y;

			Vec3 adaptation = tropismOffset(physiologicalAge, 0.1, 0.2, gravityDir);

			Vec3 parentDir = parent->calculatePosition() - calculatePosition();

			if (parentDir.normalize() != gravityDir)
				relativePosition += adaptation;

			for (BranchNode* child : children)
			{
				child->adapt();
			}
		}
		else {

			for (BranchNode* child : children)
			{
				child->adapt();
			}
		}
	}


	// We calculate the position of this node relative to the position of the parents
	// So until it reaches the root, all the positions are relatives positions to each other.
	Vec3 calculatePosition() {
		if (!parent)
			return relativePosition;
		return relativePosition + parent->calculatePosition();
	}

	BranchNode* createChild(const Vec3& relativePosition) {
		BranchNode* child = new BranchNode();
		child->relativePosition = relativePosition;
		child->parent = this;
		Vec3 parentPosition =this->calculatePosition();
		child->maxBranchLength = ((parentPosition+relativePosition) - parentPosition).magnitude();
		children.push_back(child);
		return child;
	}

	// Equation 8 of the paper
	float segmentDiameter(const BranchNode* branchNode, float thickeningFactor, float lerpFactor) {

		if (branchNode->children.size() == 0)
			return lerp(0,thickeningFactor,lerpFactor);

		//if (branchNode->branchLength < branchNode->maxBranchLength)

		float branchSegmentDiameter = thickeningFactor;
		for (BranchNode* child : branchNode->children) {
			float childSegmentDiameter = segmentDiameter(child, thickeningFactor, child->branchLength/child->maxBranchLength);
			branchSegmentDiameter += SQR(childSegmentDiameter);
		}

		return lerp(0,sqrtf(branchSegmentDiameter), lerpFactor);
	}
};

#endif
