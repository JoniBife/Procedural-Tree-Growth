#include "BranchNode.h"

BranchNode::~BranchNode() {
	for(BranchNode * child : children) {
		delete child;
	}
}

void BranchNode::updateNode(float modulePhysiologicalAge, std::vector<Vec4>& vertices, Vec4& parentPositionWithDiameter, bool isRoot) {

	if (physiologicalAge > modulePhysiologicalAge)
		return;

	if (!isRoot) {

		// We calculate the diameter first because all nodes need to calculate the diameter including the root
		branchDiameter = segmentDiameter(this, GrowthParameters::instance->thickeningFactor, branchLength / maxBranchLength);

		// We calculate the segment physiological age to be used in the branch length
		float branchSegmentAge = eqt::segmentPhysiologicalAge(modulePhysiologicalAge, parent->physiologicalAge);

		// We calculate the branch length
		branchLength = eqt::segmentLength(maxBranchLength, branchSegmentAge, GrowthParameters::instance->scalingCoefficient);
		lightExposure = 0.0f;
		vigour = 0.0f;

		Vec3 parentPosition = Vec3(parentPositionWithDiameter.x, parentPositionWithDiameter.y, parentPositionWithDiameter.z);
		// We calculate the position of this node from the parents position and scale it with the length
	
		Vec3 gravityDir = Vec3(0,-1,0).normalize();
		adaptationOffset = eqt::tropismOffset(modulePhysiologicalAge, GrowthParameters::instance->g1, GrowthParameters::instance->g2, gravityDir);
		Vec3 nodePosition = (adaptationOffset + relativePosition).normalize() * branchLength + parentPosition;

		// We then add the diameter to the W component for the geometry shader to generate the cylinders
		positionWithDiameter = Vec4(nodePosition.x, nodePosition.y, nodePosition.z, branchDiameter);

		// Finally we add the two vertices that compose the cylinder
		vertices.push_back(parentPositionWithDiameter);
		vertices.push_back(positionWithDiameter);
		
		if (branchLength == maxBranchLength && !reachedMax) {
			// The age of a branch segment is 0 throughout the growth process, and once it reached the maximum length its
			// age is set to the age of the module at that time
			reachedMax = true;
			physiologicalAge = modulePhysiologicalAge;
		}
	}
	else {
		branchDiameter = segmentDiameter(this, GrowthParameters::instance->thickeningFactor, 1.0f, false);

		positionWithDiameter = parentPositionWithDiameter;
		// if its the root we only update the diameter
		positionWithDiameter.w = branchDiameter;
	}

	// We only update the children once this node is fully grown
	if (branchLength == maxBranchLength) {
		for (BranchNode* child : children) {
			child->updateNode(modulePhysiologicalAge, vertices, positionWithDiameter, false);
		}
	}
}

Vec3 BranchNode::calculatePosition() {
	if (!parent)
		return relativePosition;
	return relativePosition + parent->calculatePosition();
}

BranchNode* BranchNode::createChild(const Vec3& relativePosition, float scaleLength, bool isTip) {
	BranchNode* child = new BranchNode();
	child->relativePosition = relativePosition * scaleLength;
	child->parent = this;
	child->maxBranchLength = child->relativePosition.magnitude();
	child->isTip = isTip;
	children.push_back(child);
	return child;
}

float BranchNode::segmentDiameter(const BranchNode* branchNode, float thickeningFactor, float lerpFactor, bool first) {

	if (branchNode->children.size() == 0) {
		return lerp(0, thickeningFactor, lerpFactor);
	}

	float branchSegmentDiameter = 0.0f;
	for (BranchNode* child : branchNode->children) {
		float childSegmentDiameter = segmentDiameter(child, thickeningFactor, child->branchLength / child->maxBranchLength, false);
		branchSegmentDiameter += SQR(childSegmentDiameter);
	}

	if (branchSegmentDiameter < thickeningFactor) {
		branchSegmentDiameter = thickeningFactor;
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
