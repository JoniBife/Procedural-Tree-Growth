#include "ShadowMapping.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/SphericalCameraController.h"
#include "../controllers/FreeCameraController.h"
#include "../textures/Texture2D.h"
#include "../utils/ColorRGBA.h"
#include "../math/MathAux.h"
#include "../textures/DepthMap.h"

static ShaderProgram* spShadows;
static ShaderProgram* spDepthMap;
static FreeCameraController* cameraController;
static Mesh* cube;
static Mesh* plane;

static SceneNode* sceneNodeCube;
static SceneNode* sceneNodeCube2;
static SceneNode* sceneNodeCube3;
static SceneNode* sceneNodePlane;

static GLint cameraPosL;
static GLint cameraPosL2;

static Vec3 lightPosition(-2.0f, 4.0f, -1.0f);

static void setupShaders(SceneGraph* sceneGraph, Camera* camera) {

	// Loading the shaders and creating the shader program
	Shader vs3(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderSM.glsl");
	Shader fs3(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderSM.glsl");
	spDepthMap = new ShaderProgram(vs3, fs3);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = spDepthMap->getUniformBlockIndex("SharedMatrices");
	spDepthMap->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	// Loading the shaders and creating the shader program
	Shader vs4(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderShadows.glsl");
	Shader fs4(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderShadows.glsl");
	spShadows = new ShaderProgram(vs4, fs4);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spShadows->getUniformBlockIndex("SharedMatrices");
	spShadows->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	GLint textureID = spShadows->getUniformLocation("shadowMap");
	spShadows->use();
	spShadows->setUniform(textureID, 0);
	spShadows->stopUsing();
}
static void setupLight() {
	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;

	float specularStrength = 1.0f;
	unsigned int shininess = 16;

	GLint lightColorL = spShadows->getUniformLocation("lightColor");
	GLint ambientStrenghtL = spShadows->getUniformLocation("ambientStrength");
	GLint lightPositionL = spShadows->getUniformLocation("lightPosition");
	GLint specularStrengthL = spShadows->getUniformLocation("specularStrength");
	GLint shininessL = spShadows->getUniformLocation("shininess");
	cameraPosL = spShadows->getUniformLocation("viewPos");

	spShadows->use();
	spShadows->setUniform(lightColorL, lightColor);
	spShadows->setUniform(ambientStrenghtL, ambientStrength);
	spShadows->setUniform(lightPositionL, lightPosition);
	spShadows->setUniform(specularStrengthL, specularStrength);
	spShadows->setUniform(shininessL, shininess);
	spShadows->stopUsing();
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

void ShadowMapping::start() {
	setupShaders(getSceneGraph(), getCamera());
	setupLight();
	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());

	plane = Mesh::loadFromFile("../Engine/objs/plane.obj");
	plane->paint(ColorRGBA::ORANGE);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, spDepthMap);
	sceneNodePlane->setModel(Mat4::translation({ 0.0f, -0.5f, 0.0f }));

	cube = Mesh::loadFromFile("../Engine/objs/sphere.obj");
	cube->paint(ColorRGBA::GREEN);
	sceneNodeCube = getSceneGraph()->getRoot()->createChild(cube, Mat4::IDENTITY, spDepthMap);
	sceneNodeCube->setModel(Mat4::translation({ 0.0f, 1.5f, 0.0f }) * Mat4::scaling(3.0f));

	sceneNodeCube2 = getSceneGraph()->getRoot()->createChild(cube, Mat4::IDENTITY, spDepthMap);
	sceneNodeCube2->setModel(Mat4::translation({ 2.0f, 0.0f, 1.0f }) * Mat4::scaling(0.5f));

	sceneNodeCube3 = getSceneGraph()->getRoot()->createChild(cube, Mat4::IDENTITY, spDepthMap);
	sceneNodeCube3->setModel(Mat4::translation({ -1.0f, 0.0f, 2.0f }) * Mat4::rotation(PI / 3.0f, { 1.0f, 0.0f, 1.0f }) * Mat4::scaling(0.25f));

	DepthMap* depthMap = new DepthMap(2048, 2048);

	sceneNodePlane->addTexture(depthMap);
	sceneNodeCube->addTexture(depthMap);
	sceneNodeCube2->addTexture(depthMap);
	sceneNodeCube3->addTexture(depthMap);

	Mat4 lightProj = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);

	Vec3 lightPos(-2.0f, 4.0f, -1.0f); // eye
	Vec3 lightTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 lightUp(0.0f, 1.0f, 0.0f); // up

	Mat4 lightView = lookAt(lightPosition, lightTarget, lightUp);

	spDepthMap->use();
	GLint uniformLSM = spDepthMap->getUniformLocation("lightSpaceMatrix");
	spDepthMap->setUniform(uniformLSM, lightProj * lightView);
	spDepthMap->stopUsing();

	spShadows->use();
	uniformLSM = spShadows->getUniformLocation("lightSpaceMatrix");
	spShadows->setUniform(uniformLSM, lightProj * lightView);
	spShadows->stopUsing();

	depthMap->setOnRenderToDepthMap(this, [&]() {

		// First we render to the depth map using the depth shaders
		sceneNodePlane->setShaderProgram(spDepthMap);
		sceneNodeCube->setShaderProgram(spDepthMap);
		sceneNodeCube2->setShaderProgram(spDepthMap);
		sceneNodeCube3->setShaderProgram(spDepthMap);
	}, [&]() {

		// Then return to the regular shaders
		sceneNodePlane->setShaderProgram(spShadows);
		sceneNodeCube->setShaderProgram(spShadows);
		sceneNodeCube2->setShaderProgram(spShadows);
		sceneNodeCube3->setShaderProgram(spShadows);
	});
}

void ShadowMapping::update() {
	spShadows->use();
	spShadows->setUniform(cameraPosL, cameraController->position);
	spShadows->stopUsing();
}

void ShadowMapping::end() {
	delete cameraController;
	delete spShadows;
	delete cube;
	delete plane;
}