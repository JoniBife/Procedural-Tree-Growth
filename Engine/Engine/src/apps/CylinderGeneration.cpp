#include "CylinderGeneration.h"
#include "../view/Transformations.h"
#include "../controllers/FreeCameraController.h"
#include "../math/MathAux.h"
#include "../textures/Texture2D.h"
#include "TreeGrowthUI.h"
#include "../textures/DepthMap.h"
#include "../math/Qtrn.h"
#include "BranchNode.h"
#include "BranchModule.h"
#include "Morphospace.h"
#include "../utils/OpenGLUtils.h"
#include "Leaves.h"

static ShaderProgram* spCylinder; // Used with the cylinders
static ShaderProgram* spDepthMapCylinder; // Used with the cylinders to generate the depth map
static ShaderProgram* spShadows; // Used with any other object
static ShaderProgram* spDepthMap; // Used with any other object to generate the depth map
static ShaderProgram* spSimple;
static Mesh* points;
static Mesh* plane;
static FreeCameraController* cameraController;
static SceneNode* cylinder;
static SceneNode* sceneNodeSphere;
static SceneNode* sceneNodePlane;
static float numberOfFrames = 0.0f;
static float totalElapsedTime = 0.0f;

static BranchModule* root;

static GLint cameraPosLsp;
static GLint cameraPosLspShadows;
static Vec3 lightPosition(4.0f, 30.0f, 20.0f);

static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static DepthMap* depthMap;

static TreeGrowthUI* treeGrowthUI;

static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

static Mesh* sphere;

static Leaves* leaves;

static bool paused = true;

static void setupShaders(SceneGraph* sceneGraph, Camera* camera) {

	//////////// CYLINDER SHADERS /////////////////////////
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/cylinderVertexShader.glsl");
	Shader gs(GL_GEOMETRY_SHADER, "../Engine/shaders/cylinderGeometryShader.geom");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/cylinderFragmentShader.glsl");
	spCylinder = new ShaderProgram(vs, gs, fs);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = spCylinder->getUniformBlockIndex("SharedMatrices");
	spCylinder->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// DEPTH MAP SHADERS /////////////////////////
	Shader vs1(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderSM.glsl");
	Shader fs1(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderSM.glsl");
	spDepthMap = new ShaderProgram(vs1, fs1);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spDepthMap->getUniformBlockIndex("SharedMatrices");
	spDepthMap->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// PLANE SHADERS /////////////////////////
	Shader vs2(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderShadows.glsl");
	Shader fs2(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderShadows.glsl");
	spShadows = new ShaderProgram(vs2, fs2);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spShadows->getUniformBlockIndex("SharedMatrices");
	spShadows->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// DEPTH MAP CYLINDER SHADERS /////////////////////////
	Shader vs3(GL_VERTEX_SHADER, "../Engine/shaders/cylinderVertexShader.glsl");
	Shader gs3(GL_GEOMETRY_SHADER, "../Engine/shaders/cylinderGeometryShaderSM.geom");
	Shader fs3(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderSM.glsl");
	spDepthMapCylinder = new ShaderProgram(vs3, gs3,fs3);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spDepthMapCylinder->getUniformBlockIndex("SharedMatrices");
	spDepthMapCylinder->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	// Loading the shaders and creating the shader program
	Shader vs4(GL_VERTEX_SHADER, "../Engine/shaders/simpleVertexShader.glsl");
	Shader fs4(GL_FRAGMENT_SHADER, "../Engine/shaders/simpleFragmentShader.glsl");
	spSimple = new ShaderProgram(vs4, fs4);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spSimple->getUniformBlockIndex("SharedMatrices");
	spSimple->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());
}

static void setupTextures() {
	// Loading textures
	woodTexture = new Texture2D("../Engine/textures/barkTexture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/barkNormalMap.jpg");
	depthMap = new DepthMap(2048, 2048);

	GLint textureID = spCylinder->getUniformLocation("diffuseMap");
	GLint normalMapID = spCylinder->getUniformLocation("normalMap");
	GLint shadowMapID = spCylinder->getUniformLocation("shadowMap");
	
	spCylinder->use();
	spCylinder->setUniform(textureID, 0);
	spCylinder->setUniform(normalMapID, 1);
	spCylinder->setUniform(shadowMapID, 2);
	spCylinder->stopUsing();

	shadowMapID = spShadows->getUniformLocation("shadowMap");
	spShadows->use();
	spShadows->setUniform(shadowMapID, 0);
	spShadows->stopUsing();
}

static void setupLightAndShadows() {

	Mat4 lightProj = ortho(-100.0f, 100.0f, -10.0f, 100.0f, 1.0f, 70.5f);

	Vec3 lightTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 lightUp(0.0f, 1.0f, 0.0f); // up

	Mat4 lightView = lookAt(lightPosition, lightTarget, lightUp);

	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;

	float specularStrength = 1.0f;
	unsigned int shininess = 32;

	/////////////////// CYLINDER ///////////////////////
	GLint lightColorL = spCylinder->getUniformLocation("lightColor");
	GLint ambientStrenghtL = spCylinder->getUniformLocation("ambientStrength");
	GLint lightPositionL = spCylinder->getUniformLocation("lightPosition");
	GLint specularStrengthL = spCylinder->getUniformLocation("specularStrength");
	GLint shininessL = spCylinder->getUniformLocation("shininess");
	normalMappingL = spCylinder->getUniformLocation("normalMapping");
	cameraPosLsp = spCylinder->getUniformLocation("viewPos");
	GLint uniformLSM = spCylinder->getUniformLocation("lightSpaceMatrix");

	spCylinder->use();
	spCylinder->setUniform(lightColorL, lightColor);
	spCylinder->setUniform(ambientStrenghtL, ambientStrength);
	spCylinder->setUniform(lightPositionL, lightPosition);
	spCylinder->setUniform(specularStrengthL, specularStrength);
	spCylinder->setUniform(shininessL, shininess);
	spCylinder->setUniform(normalMappingL, normalMapping);
	spCylinder->setUniform(uniformLSM, lightProj * lightView);
	spCylinder->stopUsing();

	spDepthMapCylinder->use();
	uniformLSM = spDepthMapCylinder->getUniformLocation("lightSpaceMatrix");
	spDepthMapCylinder->setUniform(uniformLSM, lightProj * lightView);
	spDepthMapCylinder->stopUsing();
	/////////////////////////////////////////////////////

	/////////////////// OTHER OBJECTS ///////////////////
	lightColorL = spShadows->getUniformLocation("lightColor");
	ambientStrenghtL = spShadows->getUniformLocation("ambientStrength");
	lightPositionL = spShadows->getUniformLocation("lightPosition");
	specularStrengthL = spShadows->getUniformLocation("specularStrength");
	shininessL = spShadows->getUniformLocation("shininess");
	cameraPosLspShadows = spShadows->getUniformLocation("viewPos");
	uniformLSM = spShadows->getUniformLocation("lightSpaceMatrix");

	spShadows->use();
	spShadows->setUniform(lightColorL, lightColor);
	spShadows->setUniform(ambientStrenghtL, ambientStrength);
	spShadows->setUniform(lightPositionL, lightPosition);
	spShadows->setUniform(specularStrengthL, 0.1f);
	spShadows->setUniform(shininessL, shininess);
	spShadows->setUniform(uniformLSM, lightProj * lightView);
	spShadows->stopUsing();

	spDepthMap->use();
	uniformLSM = spDepthMap->getUniformLocation("lightSpaceMatrix");
	spDepthMap->setUniform(uniformLSM, lightProj * lightView);
	spDepthMap->stopUsing();
	/////////////////////////////////////////////////////
}

static void setupCamera(Camera* camera, GLFWwindow* window, int windowWidth, int windowHeight) {
	// Adding a spherical camera controller
	float cameraMovementSpeed = 30.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 30.0f, 50.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 50.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 1000.0f);
	Mat4 perspectiveProj = perspective(PI / 2.0f, float(windowWidth / windowHeight), 0.001f, 1000.0f);

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, window);

	//cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -5.0f);
	camera->addCameraController(cameraController);
	camera->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	camera->setProjection(perspectiveProj);
}

static void setupTree(SceneGraph* sceneGraph) {

	float scaleLength = 0.5f;
	Morphospace::instance = new Morphospace(scaleLength);

	BranchNode* rootNode = new BranchNode();
	rootNode->relativePosition = {0.0f, 0.0f, 0.0f};
	rootNode->vigour = (float)GrowthParameters::instance->vRootMax;

	root = Morphospace::instance->selectModule(GrowthParameters::instance->apicalControl, GrowthParameters::instance->determinacy, rootNode);
	root->vigour = (float)GrowthParameters::instance->vRootMax; // The vigour in the root module of the tree is vRootMax

	points = new Mesh();
	points->setPrimitive(GL_LINES);
	points->setVerticesBufferType(GL_STREAM_DRAW);
	points->setVerticesBufferSize(1000);
	points->init();

	root->sceneNode = sceneGraph->getRoot()->createChild(points, Mat4::IDENTITY, spCylinder);
	root->sceneNode->addTexture(woodTexture);
	root->sceneNode->addTexture(woodTextureNormalMap);
	root->sceneNode->addTexture(depthMap);
}

static void setDefaultGrowthParameters() {
	GrowthParameters* growthParameters = new GrowthParameters();
	growthParameters->gP = 0.12f;
	growthParameters->scalingCoefficient = 1.29f;
	growthParameters->vRootMax = 900;
	growthParameters->thickeningFactor = 0.01f; // original 1.41
	growthParameters->pMax = 950;
	growthParameters->vMin = 0.0f;
	growthParameters->vMax = (float)growthParameters->vRootMax;
	growthParameters->determinacy = 0.93f;
	growthParameters->apicalControl = 0.87f;
	growthParameters->tropismAngle = 0.66f;
	growthParameters->w1 = 0.14f;
	growthParameters->w2 = 0.14f;
	growthParameters->g1 = 0.2f;
	growthParameters->g2 = 3.0f;
	GrowthParameters::instance = growthParameters;
}

void Cylinder::start() {

	setDefaultGrowthParameters();

	treeGrowthUI = new TreeGrowthUI(getGui(), float(getWindowWidth()), float(getWindowHeight()), [&](GrowthParameters gp) {
		if (root) {
			root->sceneNode->deleteSceneNode();
			delete root;
		}
		// When the user presses start we setup the tree again 
		GrowthParameters::instance = new GrowthParameters(gp);
		setupTree(getSceneGraph());
		paused = false;
	}, [&]() {
		paused = !paused;
	}, [&]() {
		GrowthParameters::instance = new GrowthParameters(treeGrowthUI->getGrowthParameters());
		/*paused = true;
		if (root) {
			root->sceneNode->deleteSceneNode();
			delete root;
		}*/
	});

	setSkyBox({
		"../Engine/textures/skybox/right.png",
		"../Engine/textures/skybox/left.png",
		"../Engine/textures/skybox/top.png",
		"../Engine/textures/skybox/bottom.png",
		"../Engine/textures/skybox/front.png",
		"../Engine/textures/skybox/back.png"
	});

	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());
	setupShaders(getSceneGraph(), getCamera());
	setupTextures();
	setupLightAndShadows();

	// Setup leaves
	leaves = new Leaves();

	plane = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	plane->paint(ColorRGBA::GREEN);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, spShadows);
	sceneNodePlane->setModel(Mat4::scaling({200.0f, 0.1f, 200.0f}));
	sceneNodePlane->addTexture(depthMap);

	depthMap->setOnRenderToDepthMap(this, [&]() {
		// First we render to the depth map using the depth shaders
		//cylinder->setShaderProgram(spDepthMapCylinder);
		if (root) root->sceneNode->setShaderProgram(spDepthMapCylinder);
		sceneNodePlane->setShaderProgram(spDepthMap);
	}, [&]() {

		// Then return to the regular shaders
		//cylinder->setShaderProgram(spCylinder);
		if (root) root->sceneNode->setShaderProgram(spCylinder);
		sceneNodePlane->setShaderProgram(spShadows);
	});

	// Bounding sphere
	sphere = Mesh::loadFromFile("../Engine/objs/sphere.obj");
	sphere->paint(ColorRGBA::RED);

	sceneNodeSphere = getSceneGraph()->getRoot()->createChild(sphere, Mat4::IDENTITY, spSimple);
	sceneNodeSphere->setBeforeDrawFunction([=](ShaderProgram* sp) {
		GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	});
	sceneNodeSphere->setAfterDrawFunction([]() {
		GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	});

}

float currOffset = 0.01f;
int i = 0;
bool drawSphere = true;
bool hasLeaves = false;

void Cylinder::update() {

	static bool isBReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_B) == GLFW_PRESS && isBReleased) {
		isBReleased = false;
		drawSphere = !drawSphere;
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_B) == GLFW_RELEASE) {
		isBReleased = true;
	}

	if (!paused && root) {
		root->updateModule(getElapsedTime());

		if (drawSphere)
			sceneNodeSphere->setModel(Mat4::translation(root->boundingSphere.center) * Mat4::scaling(root->boundingSphere.radius * 2));
		else
			sceneNodeSphere->setModel(Mat4::ZERO);
	}

	treeGrowthUI->updateFPSCounter(float(getElapsedTime()));

	spCylinder->use();
	spCylinder->setUniform(cameraPosLsp, cameraController->position);

	static bool isNReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_PRESS && isNReleased) {
		isNReleased = false;
		normalMapping = (normalMapping == GL_TRUE) ? GL_FALSE : GL_TRUE;
		spCylinder->setUniform(normalMappingL, normalMapping);
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_RELEASE) {
		isNReleased = true;
	}

	spCylinder->stopUsing();

	spShadows->use();
	spShadows->setUniform(cameraPosLspShadows, cameraController->position);
	spShadows->stopUsing();


	static bool isLReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_L) == GLFW_PRESS && isLReleased) {
		isLReleased = false;

		hasLeaves = !hasLeaves;

		if (!hasLeaves)
			root->generateLeaves(getSceneGraph(), leaves);
		else
			leaves->removeLeaves();
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_L) == GLFW_RELEASE) {
		isLReleased = true;
	}

	//points->updateVertices(dynamicVertices);

}

void Cylinder::end() {

}