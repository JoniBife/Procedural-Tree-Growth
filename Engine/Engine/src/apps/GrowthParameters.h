#include "../math/Vec3.h"
#ifndef GROWTH_PARAMETERS_H
#define GROWTH_PARAMETERS_H

struct GrowthParameters {
	unsigned int pMax; // Maximum plant age
	unsigned int vRootMax; // Maximum plant vigour (The maximum vigour of the root)
	float gP; // Growth rate of the whole plant
	float apicalControl; // Lambda in the paper, represents the growth suppression of an existing subdominant branch by a higher dominanting shoot thta functions to maintain crown dominance by a central stem
	float determinacy; // D in the paper
	// float fAge; // Since we are not making flowering, we don't need a flowering age
	Vec3 gravityDir;

	float tropismAngle; // Alpha in the paper
	float w1, w2; //  Weights that controll the impact of optimization criteria in gradient descent
	float g1; // Controls how fast the tropism effect decreases with time
	float g2; // Controls the overall strength of the tropism
	float thickeningFactor; // Phi in the paper
	float scalingCoefficient; // Its a scaling coefficient used in the paper for calculating the segment lengt

	float vMax; // Maximum module vigour, not clear in the paper whether this is global or module specific
	float vMin; // Minimum module vigour, not clear in the paper whether this is global or module specific

	unsigned int maxModules;

	int leavesPerTip;

	static GrowthParameters* instance; // TODO Use proper singleton pattern


};

#endif
