#ifndef MORPHOSPACE_H
#define MORPHOSPACE_H

#include <iostream>
#include "BranchModule.h"
#include "../scene/SceneGraph.h"
#include "GrowthParameters.h"
#include "../textures/Texture2D.h"
#include "../meshes/Mesh.h"
#include "Singleton.h"

class Morphospace {

private:
	// A bidimentional array with Branch Module creation functions
	std::function<BranchModule*(BranchModule*)> modulePrototypeFunctions[3][3];
	float scaleLength; // Streches the module, so that the branches look thinner, while maintaining the same scale

public:
	static Morphospace* instance; // TODO Use proper singleton

	Morphospace(float scaleLength);
	BranchModule* selectModule(float apicalControl, float determinacy, BranchNode*& root);
};

#endif

