#include "Tree.h"
#include <math.h>
#include "Equations.h"
#include "Morphospace.h"

Tree::Tree(const Vec3& positionRoot, SceneGraph* sceneGraph, Texture2D* woodTexture, Texture2D* woodNormalMap) {

	cylinder = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	cylinder->calculateTangents();

	BranchNode* rootNode = new BranchNode();
	rootNode->relativePosition = positionRoot;
	rootNode->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
	rootNode->sceneGraphNode->addTexture(woodTexture);
	rootNode->sceneGraphNode->addTexture(woodNormalMap);
	rootNode->vigour =  (float)GrowthParameters::instance->vRootMax;

	root = Morphospace::instance->selectModule(GrowthParameters::instance->apicalControl, GrowthParameters::instance->determinacy, rootNode);
	root->vigour = (float)GrowthParameters::instance->vRootMax; // The vigour in the root module of the tree is vRootMax
	root->tree = this;
	root->setOrientation(Mat4::IDENTITY);
	modules.push_back(root);
}

Tree::~Tree() {
	delete cylinder;
	delete root->root; // First we delete the graph of BranchNodes
	delete root; // Then we delete the graph of BranchModules
}


void Tree::startGrowth(GrowthParameters* growthParameters) {
	this->growthParameters = growthParameters;
	currTime = 0.0f;
}

void Tree::pauseGrowth() {
	// TODO
}

void Tree::resumeGrowth() {
	// TODO
}

void Tree::grow(float elapsedTime) {
	distributeLightAndVigor();
	developModules(elapsedTime);
}

void Tree::accumulateModuleLightExposure(BranchModule* module) {
	if (!module->parent)
		return;

	module->parent->lightExposure += module->lightExposure;
	accumulateModuleLightExposure(module->parent);
}


void Tree::calculateVigorFluxes(BranchModule* module) {

	if (module->children.size() == 0)
		return;

	float sumLateralLightExposure = 0.0f;
	float mainLightExposure = 0.0f;

	// First we calculate the sum of lateral modules light exposure and the main light exposure
	for (BranchModule* child : root->children) {

		// Is child a lateral branch
		if (!child->main)
			sumLateralLightExposure += child->lightExposure;
		else
			mainLightExposure = child->lightExposure;
	}

	// Secondly we calculate the vigor of the main module using equation 2, and the lateral vigor
	float vigorMain = eqt::vigor(module->vigour, GrowthParameters::instance->apicalControl, mainLightExposure, sumLateralLightExposure);
	float vigorLateral = module->vigour - vigorMain;

	// Finally we distribute the lateral vigor correctly using the porportional amount of light exposure they provided to equation 2
	for (BranchModule* child : root->children) {
		// Is child a lateral branch
		if (!child->main) {
			// Each lateral module takes a percentage of the lateral vigor
			child->vigour = (child->lightExposure / sumLateralLightExposure) * vigorLateral; 
		}
		else
			child->vigour = vigorMain;

		// The child then propagates its vigor to his children
		calculateVigorFluxes(child);
	}

}


void Tree::distributeLightAndVigor() {

	// First we calculate the light exposure of each module
	for (BranchModule* moduleA : modules) {
		float fCollisions = 0.0f;
		for (BranchModule* moduleB : modules) {
			if (moduleA == moduleB)
				continue;

			fCollisions += moduleA->boundingSphere.intersectVolume(moduleB->boundingSphere)/1000;
		}
		moduleA->lightExposure = expf(-fCollisions);
	}

	// Then we do a basipetal (tip-to-base) pass from tips to root accumulating the value of total light exposure
	for (BranchModule* tip : tips) {
		accumulateModuleLightExposure(tip);
	}

	// Finally we do an acropetal (base-to-tip) pass calculating vigor fluxes at each module intersection
	calculateVigorFluxes(root);
}


void Tree::developModules(float elapsedTime) {
	root->updateModule(elapsedTime);
}