#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <math.h>
//#include "BranchNode.h"


float ageVariation(float elapsedTime, float growthRate) {
	return growthRate * elapsedTime;
}

// Equation 5 of the Paper
#define S(X) 3*(X*X) - 2*(X*X*X) // Sigmoid-like function used to smoothly interpolate vigour
float growthRate(float vigour, float minVigour, float maxVigour, float plantGrowthRate) {
	return S((vigour - minVigour) / (maxVigour - minVigour)) * plantGrowthRate;
}

// Equation 7 of the paper
float segmentPhysiologicalAge(float au, float an) {
	return fmaxf(0, au - an);
}

// Equation 9 of the paper
float segmentLength(float maxLength, float physiologicalAge, float scalingCoefficient) {
	return fminf(maxLength, scalingCoefficient * physiologicalAge);
}

// Equation 10 of the paper 
Vec3 tropismOffset(float physiologicalAge, float g1, float g2, const Vec3& gravityDir) {
	return (g1 * gravityDir * g2) / (physiologicalAge + g1);
}


#endif
