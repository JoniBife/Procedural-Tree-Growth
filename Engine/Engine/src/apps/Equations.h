#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <math.h>
#include "../math/Vec3.h"


namespace eqt {

	// Equation 2 of the paper
	float vigor(float vigorModule, float apicalControl, float lightExposureUm, float lightExposureUl);

	// Equation 5 of the Paper
	#define S(X) 3*(X*X) - 2*(X*X*X) // Sigmoid-like function used to smoothly interpolate vigour
	float growthRate(float vigour, float minVigour, float maxVigour, float plantGrowthRate);

	// Equation 6 of the paper
	float ageVariation(float elapsedTime, float growthRate);

	// Equation 7 of the paper
	float segmentPhysiologicalAge(float au, float an);

	// Equation 9 of the paper
	float segmentLength(float maxLength, float physiologicalAge, float scalingCoefficient);

	// Equation 10 of the paper 
	Vec3 tropismOffset(float physiologicalAge, float g1, float g2, const Vec3& gravityDir);
}


#endif
