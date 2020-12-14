#include "BranchNode.h"

void BranchNode::updateNode(float modulePhysiologicalAge) {

	if (physiologicalAge > modulePhysiologicalAge)
		return;

	if (parent) {
		// First we calculate the segment physiological age to be used in the branch length
		float branchSegmentAge = eqt::segmentPhysiologicalAge(modulePhysiologicalAge, parent->physiologicalAge);

		// Thirdly we calculate the branch length
		branchLength = eqt::segmentLength(maxBranchLength, branchSegmentAge, growthParameters->scalingCoefficient);

		// Secondly we calculate the diameter
		branchDiameter = segmentDiameter(this, growthParameters->thickeningFactor/*,branchLength/maxBranchLength*/);

		// We then scale the cylinder and translate it upwards so that its base stays in the same position
		Mat4 scaling = Mat4::scaling({ branchDiameter, branchLength, branchDiameter });
		Mat4 translation = Mat4::translation({ 0.0f, branchLength / 2, 0.0f });

		// We also have to rotate the branch to the direction of its node
		Vec3 parentPosition = parent->calculatePosition();
		Vec3 dir = relativePosition.normalize();
		Qtrn q = Qtrn::fromDir(dir);
		Mat4 rotation = q.toRotationMatrix();

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

void BranchNode::adapt() {

	if (parent) {

		Vec3 gravityDir = -1 * Vec3::Y;

		Vec3 adaptation = eqt::tropismOffset(physiologicalAge, 0.1, 0.2, gravityDir);

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

Vec3 BranchNode::calculatePosition() {
	if (!parent)
		return relativePosition;
	return relativePosition + parent->calculatePosition();
}

BranchNode* BranchNode::createChild(const Vec3& relativePosition) {
	BranchNode* child = new BranchNode();
	child->relativePosition = relativePosition;
	child->parent = this;
	child->maxBranchLength = relativePosition.magnitude();
	children.push_back(child);
	return child;
}

float BranchNode::segmentDiameter(const BranchNode* branchNode, float thickeningFactor, float lerpFactor, bool first) {

	if (branchNode->children.size() == 0) {
		if (first)
			return lerp(0, thickeningFactor, lerpFactor);
		return thickeningFactor;
	}

	float branchSegmentDiameter = thickeningFactor;
	for (BranchNode* child : branchNode->children) {
		float childSegmentDiameter = segmentDiameter(child, thickeningFactor, child->branchLength / child->maxBranchLength, false);
		branchSegmentDiameter += SQR(childSegmentDiameter);
	}

	return lerp(0, sqrtf(branchSegmentDiameter), lerpFactor);
}

float BranchNode::segmentDiameter(const BranchNode* branchNode, float thickeningFactor) {
	if (branchNode->children.size() == 0)
		return thickeningFactor;

	float branchSegmentDiameter = 0.0f;
	for (BranchNode* child : branchNode->children) {
		float childSegmentDiameter = segmentDiameter(child, thickeningFactor);
		branchSegmentDiameter += SQR(childSegmentDiameter);
	}
	return sqrtf(branchSegmentDiameter);
}