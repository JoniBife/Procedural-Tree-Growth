#include "Equations.h"

// Equation 2 of the paper
float eqt::vigor(float vigorModule, float apicalControl, float lightExposureUm, float lightExposureUl) {
	return vigorModule * ((apicalControl * lightExposureUm) / (apicalControl * lightExposureUm + (1 - apicalControl) * lightExposureUl));
}

// Equation 5 of the Paper
float eqt::growthRate(float vigour, float minVigour, float maxVigour, float plantGrowthRate) {
	return S((vigour - minVigour) / (maxVigour - minVigour)) * plantGrowthRate;
}

// Equation 6 of the paper
float eqt::ageVariation(float elapsedTime, float growthRate) {
	return growthRate * elapsedTime;
}

// Equation 7 of the paper
float eqt::segmentPhysiologicalAge(float au, float an) {
	return fmaxf(0, au - an);
}

// Equation 9 of the paper
float eqt::segmentLength(float maxLength, float physiologicalAge, float scalingCoefficient) {
	return fminf(maxLength, scalingCoefficient * physiologicalAge);
}

// Equation 10 of the paper 
Vec3 eqt::tropismOffset(float physiologicalAge, float g1, float g2, const Vec3& gravityDir) {
	return (g1 * gravityDir * g2) / (physiologicalAge + g1);
}