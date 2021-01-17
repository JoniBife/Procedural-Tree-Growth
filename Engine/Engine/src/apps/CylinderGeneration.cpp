#include "CylinderGeneration.h"
#include "../view/Transformations.h"
#include "../controllers/FreeCameraController.h"
#include "../math/MathAux.h"
#include "../textures/Texture2D.h"
#include "TreeGrowthUI.h"
#include "../textures/DepthMap.h"

static ShaderProgram* spCylinder; // Used with the cylinders
static ShaderProgram* spDepthMapCylinder; // Used with the cylinders to generate the depth map
static ShaderProgram* spShadows; // Used with any other object
static ShaderProgram* spDepthMap; // Used with any other object to generate the depth map
static Mesh* points;
static Mesh* plane;
static FreeCameraController* cameraController;
static SceneNode* cylinder;
static SceneNode* sceneNodePlane;
static float numberOfFrames = 0.0f;
static float totalElapsedTime = 0.0f;

static GLint cameraPosLsp;
static GLint cameraPosLspShadows;
static Vec3 lightPosition(5.0f, 10.0f, 10.0f);

static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static DepthMap* depthMap;

static TreeGrowthUI* treeGrowthUI;

static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

std::vector<Vec4> dynamicVertices = {
		{0.0f, -2.0f, 0.0f , 1.5f},
		{0.0f, 2.0f, 0.0f , 0.5f}
};

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

	Mat4 lightProj = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 70.5f);

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
	float cameraMovementSpeed = 5.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 0.0f, 3.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 1.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 1000.0f);
	Mat4 perspectiveProj = perspective(PI / 2.0f, float(windowWidth / windowHeight), 0.001f, 1000.0f);

	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, window);

	camera->addCameraController(cameraController);
	camera->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	camera->setProjection(perspectiveProj);
}

void Cylinder::start() {
	treeGrowthUI = new TreeGrowthUI(getGui(), float(getWindowWidth()), float(getWindowHeight()), [=](GrowthParameters gp) {
		std::cout << "Start" << std::endl;
	}, [=]() {
		std::cout << "Pause" << std::endl;
	}, [=]() {
		std::cout << "Stop" << std::endl;
	});

	setSkyBox({
		"../Engine/textures/skybox/right.jpg",
		"../Engine/textures/skybox/left.jpg",
		"../Engine/textures/skybox/top.jpg",
		"../Engine/textures/skybox/bottom.jpg",
		"../Engine/textures/skybox/front.jpg",
		"../Engine/textures/skybox/back.jpg"
	});

	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());
	setupShaders(getSceneGraph(), getCamera());
	setupTextures();
	setupLightAndShadows();

	plane = Mesh::loadFromFile("../Engine/objs/plane.obj");
	plane->paint(ColorRGBA::ORANGE);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, spShadows);
	sceneNodePlane->setModel(Mat4::translation({ 0.0f, -2.0f, 0.0f }) * Mat4::scaling(0.5f));
	sceneNodePlane->addTexture(depthMap);

	std::vector<Vec4> vertices = {
		{0.0f, -2.0f, 0.0f , 0.8f},
		{0.0f, 2.0f, 0.0f , 0.5f},
		{0.0f, 2.0f, 0.0f , 0.5f},
		{1.5f, 4.0f, 0.0f , 0.3f},
		{0.0f, 2.0f, 0.0f , 0.5f},
		{-1.5f, 4.0f, 0.0f , 0.3f},
		{1.5f, 4.0f, 0.0f , 0.3f},
		{2.5f, 6.0f, 0.0f , 0.2f},
		{1.5f, 4.0f, 0.0f , 0.3f},
		{0.5f, 6.0f, 0.0f , 0.2f},
		{-1.5f, 4.0f, 0.0f , 0.3f},
		{-2.5f, 6.0f, 2.0f , 0.2f},
		{-1.5f, 4.0f, 0.0f , 0.3f},
		{-0.5f, 6.0f, -3.0f , 0.2f}
	};

	points = new Mesh(vertices);
	points->setPrimitive(GL_LINES);
	points->setVerticesBufferType(GL_STREAM_DRAW);
	//points->setVerticesBufferSize(1000);

	cylinder = getSceneGraph()->getRoot()->createChild(points, Mat4::IDENTITY, spCylinder);

	float currOffset = 0.1f;

	/*for (int i = 0; i < 1000; ++i) {
		currOffset += 0.1f;
		cylinder = getSceneGraph()->getRoot()->createChild(points, Mat4::translation({currOffset, 0.0f, 0.0f}), spCylinder);
	}*/

	cylinder->addTexture(woodTexture);
	cylinder->addTexture(woodTextureNormalMap);
	cylinder->addTexture(depthMap);

	depthMap->setOnRenderToDepthMap(this, [&]() {

		// First we render to the depth map using the depth shaders
		cylinder->setShaderProgram(spDepthMapCylinder);
		sceneNodePlane->setShaderProgram(spDepthMap);
	}, [&]() {

		// Then return to the regular shaders
		cylinder->setShaderProgram(spCylinder);
		sceneNodePlane->setShaderProgram(spShadows);
	});
}

float currOffset = 0.01f;
int i = 0;

void Cylinder::update() {
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

	//dynamicVertices[1].y += currOffset;

	//points->updateVertices(dynamicVertices);

	/*if (i < 1000) {
		currOffset += 0.1f;
		dynamicVertices.push_back({ currOffset, -2.0f, 0.0f , 1.5f });
		dynamicVertices.push_back({ currOffset, 2.0f, 0.0f , 0.5f });

		points->updateVertices(dynamicVertices);
		++i;
	}*/

}

void Cylinder::end() {

}