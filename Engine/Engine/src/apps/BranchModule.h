#ifndef BRANCH_MODULE_H
#define BRANCH_MODULE_H

#include "BranchNode.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../apps/BoundingSphere.h"
#include "Equations.h"

struct BranchModule {
	float lightExposure;
	float vigour;
	float growthRate; // Upsilon in the paper
	float physiologicalAge = 0.0f;

	Vec4 relativePosition;
	Vec3 orientation; // Orientation is represented using Euler Angles (yaw, pitch, roll)

	//BoundingSphere boundingSphere;

	BranchNode* root;

	void updateModule(float elapsedTime) {

		// First we update the module physiological age
		physiologicalAge += growthRate * elapsedTime;

		// Then we update the diameter and the length of each of the segments of the module
		root->updateNode(physiologicalAge);
	}

	void adapt() { root->adapt(); }

	BranchModule() {}

};

#endif

