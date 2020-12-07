#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeGrowth.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/SphericalCameraController.h"
#include "../textures/Texture2D.h"
#include "../utils/ColorRGBA.h"
#include "BranchModule.h"
#include "GrowthParameters.h"
#include "Equations.h"

static Mesh* cylinder;
static ShaderProgram* sp;
static ICameraController* cameraController;
static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static GrowthParameters* growthParameters;
static BranchModule* module;

static float currTime = 0.0f;

void setupTree(SceneGraph* sceneGraph) {
	growthParameters = new GrowthParameters();
	growthParameters->gP = 0.12f;
	growthParameters->scalingCoefficient = 1.29f;
	growthParameters->vRootMax = 900;
	growthParameters->thickeningFactor = 1.41f;
	float vMin = 0;

	module = new BranchModule();
	module->growthRate = growthRate(growthParameters->vRootMax, vMin, growthParameters->vRootMax, growthParameters->gP);
	module->root = new BranchNode();
	module->root->relativePosition = { 0.0f, 0.0f, 0.0f };

	BranchNode* child = module->root->createChild({ 10,10,0 });
	child->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
	child->growthParameters = growthParameters;

	child->sceneGraphNode->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(sp->getUniformLocation("lightPos"), Vec3(1.0f, 2.0f, 2.0f));
		sp->setUniform(sp->getUniformLocation("viewPos"), Vec3(0.0f, 0.0f, 5.0f));
	});
	child->sceneGraphNode->addTexture(woodTexture);
	child->sceneGraphNode->addTexture(woodTextureNormalMap);

	//child->createChild({ 0,0,0 });
	//child->createChild({ 0,0,0 });

}

void TreeGrowth::start() {
	// Loading the cylinder 
	cylinder = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	cylinder->calculateTangentsAndBitangents();
	//cylinder->transform(Mat4::translation({0,0,-20}));

	// Loading the shaders and creating the shader program
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader2.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader2.glsl");
	sp = new ShaderProgram(vs, fs);
	getSceneGraph()->getRoot()->setShaderProgram(sp);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	// Loading textures
	/*/
	woodTexture = new Texture2D("../Engine/textures/Seamless_tree_bark_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/Seamless_tree_bark_texture_NORMAL.jpg");
	/**/
	woodTexture = new Texture2D("../Engine/textures/tileable_wood_planks_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/tileable_wood_planks_texture_NORMAL.jpg");
	/**/

	// Adding a spherical camera controller
	cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	getCamera()->addCameraController(cameraController);

	setupTree(getSceneGraph());

	// Scene graph related 
	/*SceneNode* node = getSceneGraph()->getRoot()->createChild(cylinder, Mat4::IDENTITY);
	node->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(sp->getUniformLocation("lightPos"), Vec3(1.0f, 2.0f, 2.0f));
		sp->setUniform(sp->getUniformLocation("viewPos"), Vec3(0.0f, 0.0f, 5.0f));
	});
	node->addTexture(woodTexture);
	node->addTexture(woodTextureNormalMap);*/
};

void TreeGrowth::update() {
	module->updateModule((float)getElapsedTime());
}

void TreeGrowth::end() {
	delete cameraController;
	delete sp;
	delete cylinder;
	delete woodTexture;
};


