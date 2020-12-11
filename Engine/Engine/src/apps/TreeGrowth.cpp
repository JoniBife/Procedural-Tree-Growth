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
	growthParameters->thickeningFactor = 0.5f; // original 1.41
	float vMin = 0;

	module = new BranchModule();
	module->growthRate = growthRate(growthParameters->vRootMax, vMin, growthParameters->vRootMax, growthParameters->gP);
	module->root = new BranchNode();
	module->root->relativePosition = { 0.0f, 0.0f, 0.0f };

	BranchNode* child = module->root->createChild({ 0.0f,10.0f,0.0f });
	child->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
	child->growthParameters = growthParameters;
	child->sceneGraphNode->addTexture(woodTexture);
	/*{
		BranchNode* childA = child->createChild({ 5.0f,5,0.0f });
		childA->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
		childA->growthParameters = growthParameters;
		childA->sceneGraphNode->addTexture(woodTexture);
		{
			BranchNode* childA1 = childA->createChild({ 0.0f,5,0.0f });
			childA1->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
			childA1->growthParameters = growthParameters;
			childA1->sceneGraphNode->addTexture(woodTexture);

			BranchNode* childA2 = childA->createChild({ 5.0f,0.0f,0.0f });
			childA2->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
			childA2->growthParameters = growthParameters;
			childA2->sceneGraphNode->addTexture(woodTexture);
		}

		BranchNode* childB = child->createChild({ -5.0f,5.0f,0.0f });
		childB->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
		childB->growthParameters = growthParameters;
		childB->sceneGraphNode->addTexture(woodTexture);
		{
			BranchNode* childB1 = childB->createChild({ 0.0f,5,0.0f });
			childB1->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
			childB1->growthParameters = growthParameters;
			childB1->sceneGraphNode->addTexture(woodTexture);

			BranchNode* childB2 = childB->createChild({ -5.0f,0.0f,0.0f });
			childB2->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
			childB2->growthParameters = growthParameters;
			childB2->sceneGraphNode->addTexture(woodTexture);
		}
	}*/
}

void TreeGrowth::start() {
	// Loading the cylinder 
	cylinder = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	cylinder->calculateTangentsAndBitangents();
	//cylinder->transform(Mat4::translation({0,0,-20}));

	// Loading the shaders and creating the shader program
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderCGJ.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderCGJ.glsl");
	sp = new ShaderProgram(vs, fs);
	getSceneGraph()->getRoot()->setShaderProgram(sp);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	// Loading textures
	/**/
	woodTexture = new Texture2D("../Engine/textures/Seamless_tree_bark_texture.jpg");
	//woodTextureNormalMap = new Texture2D("../Engine/textures/Seamless_tree_bark_texture_NORMAL.jpg");
	/*/
	woodTexture = new Texture2D("../Engine/textures/tileable_wood_planks_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/tileable_wood_planks_texture_NORMAL.jpg");
	/**/

	// Adding a spherical camera controller
	cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -30.0f);
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

bool adapted = false;

void TreeGrowth::update() {
	module->updateModule((float)getElapsedTime());

	//if (module->physiologicalAge > 20 && !adapted) {
		//module->adapt();
	//}
}

void TreeGrowth::end() {
	delete cameraController;
	delete sp;
	delete cylinder;
	delete woodTexture;
};


