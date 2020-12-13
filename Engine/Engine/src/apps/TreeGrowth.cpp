#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeGrowth.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/SphericalCameraController.h"
#include "../controllers/FreeCameraController.h"
#include "../textures/Texture2D.h"
#include "../utils/ColorRGBA.h"
#include "BranchModule.h"
#include "GrowthParameters.h"
#include "Equations.h"

static Mesh* cylinder;
static ShaderProgram* sp;
static FreeCameraController* cameraController;
static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static GrowthParameters* growthParameters;
static BranchModule* module;

static GLint cameraPosL;
static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

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
	cylinder = Mesh::loadFromFile("../Engine/objs/sphere.obj");
	//cylinder->paint({ 1.0f, 0.5f, 0.31f , 1.0f});
	cylinder->calculateTangents();
	//cylinder->transform(Mat4::translation({0,0,-20}));

	// Loading the shaders and creating the shader program
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderNM.glsl");
	//Shader gs(GL_GEOMETRY_SHADER, "../Engine/shaders/geometryShaderNM.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderNM.glsl");
	
	sp = new ShaderProgram(vs, fs);
	getSceneGraph()->getRoot()->setShaderProgram(sp);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	// Loading textures
	/**/
	woodTexture = new Texture2D("../Engine/textures/barkTexture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/barkNormalMap.jpg");

	GLint textureID = sp->getUniformLocation("diffuseMap");
	GLint normalMapID = sp->getUniformLocation("normalMap");

	sp->use();
	sp->setUniform(textureID, 0);
	sp->setUniform(normalMapID, 1);


	/*/
	woodTexture = new Texture2D("../Engine/textures/tileable_wood_planks_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/tileable_wood_planks_texture_NORMAL.jpg");
	/**/

	// Adding a spherical camera controller
	float cameraMovementSpeed = 10.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 0.0f, 2.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 1.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 100.0f);
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, float(getWindowWidth() / getWindowHeight()), 0.001f, 10.0f);

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, getWindow());
	
	//cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -5.0f);
	getCamera()->addCameraController(cameraController);
	getCamera()->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

	SceneNode* node = getSceneGraph()->getRoot()->createChild(cylinder, Mat4::IDENTITY);
	node->addTexture(woodTexture);
	node->addTexture(woodTextureNormalMap);
	//node->setModel(Mat4::scaling({ 0.0f, 5.0f, 0.0f }));

	Vec3 lightColor(0.0f, 1.0f, 0.0f);
	float ambientStrength = 0.3f;

	Vec4 lightPosition(0.5f, 0.3f, 0.3f, 1.0f);

	float specularStrength = 1.0f;
	unsigned int shininess = 4;

	GLint lightColorL = sp->getUniformLocation("lightColor");
	GLint ambientStrenghtL = sp->getUniformLocation("ambientStrength");
	GLint lightPositionL = sp->getUniformLocation("lightPosition");
	GLint specularStrengthL = sp->getUniformLocation("specularStrength");
	GLint shininessL = sp->getUniformLocation("shininess");
	GLint objectColorL = sp->getUniformLocation("color");
	normalMappingL = sp->getUniformLocation("normalMapping");
	cameraPosL = sp->getUniformLocation("viewPos");

	sp->use();
	sp->setUniform(lightColorL, lightColor);
	sp->setUniform(ambientStrenghtL, ambientStrength);
	sp->setUniform(lightPositionL, lightPosition*2);
	sp->setUniform(specularStrengthL, specularStrength);
	sp->setUniform(shininessL, shininess);
	sp->setUniform(normalMappingL, normalMapping);
	sp->setUniform(objectColorL, { 1.0f, 0.5f, 0.31f });
	sp->stopUsing();
};



void TreeGrowth::update() {

	sp->use();
	sp->setUniform(cameraPosL, cameraController->position);

	static bool isReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_M) == GLFW_PRESS && isReleased) {
		isReleased = false;
		normalMapping = (normalMapping == GL_TRUE) ? GL_FALSE : GL_TRUE;
		sp->setUniform(normalMappingL, normalMapping);
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_M) == GLFW_RELEASE) {
		isReleased = true;
	}

	sp->stopUsing();
	//module->updateModule((float)getElapsedTime());

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


