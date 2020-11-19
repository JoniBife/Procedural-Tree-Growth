#include "AppAVT.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../controllers/FreeCameraController.h"
#include "../controllers/OrbitCameraController.h"
#include "../view/Camera.h"
#include "../view/Transformations.h"
#include "../scene/SceneGraph.h"
#include "../math/Qtrn.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"

static Mesh* cube;
static Mesh* backpieceMesh;
static Mesh* frameMesh;
static ShaderProgram* sp;
static ICameraController* cameraController;
static Camera* camera;
static SceneGraph* sceneGraph;
static SceneNode* penroseTriangle;
static SceneNode* backpiece;
static SceneNode* frame;
std::vector<SceneNode*> cubes;
std::vector<Mat4> cubesInitialModels;

static const float offset = 0.1f;
static const float width = 0.2f;

SceneNode* createPenroseTriangle(SceneNode* parent) {

	cube->transform(Mat4::scaling(0.10f)); // Blender Cube has a width and height of 2
	cube->colors = {
		ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED, ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED,
		ColorRGBA::GREEN,ColorRGBA::GREEN,ColorRGBA::GREEN,ColorRGBA::BLUE,ColorRGBA::BLUE,ColorRGBA::BLUE,
		ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,

		ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED,ColorRGBA::RED,
		ColorRGBA::GREEN,ColorRGBA::GREEN,ColorRGBA::GREEN,ColorRGBA::BLUE,ColorRGBA::BLUE,ColorRGBA::BLUE,
		ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f,ColorRGBA::WHITE - 0.3f
	};

	SceneNode* base = parent->createChild();
	base->setModel(Mat4::translation({ 0.6f * (width + offset), 0.0f, 2.5f*(offset + width) }) * Mat4::rotation(M_PI/5, Vec3::Y) * Mat4::rotation(-M_PI/4, Vec3::X));

	base->createChild(cube, Mat4::translation(-(width + offset), -1.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(-(width + offset)*2, -1.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(-(width + offset) * 3, -1.5f * (width + offset), 0.0f));

	SceneNode* lastCube = base->createChild(cube, Mat4::translation(0.0f, 1.5f * (width + offset), -(width + offset) * 2));	
	lastCube->setBeforeDrawFunction([=](ShaderProgram* sp) {
		GL_CALL(glDisable(GL_DEPTH_TEST));
	});
	lastCube->setAfterDrawFunction([]() {
		GL_CALL(glEnable(GL_DEPTH_TEST));
	});

	SceneNode* cube1 = base->createChild(cube, Mat4::translation(0.0f, 1.5f * (width + offset), -(width + offset)));

	base->createChild(cube, Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -1.5f * (width + offset), 0.0f));

	cubes = base->getChildren();
	for (SceneNode* cube : cubes)
	{
		cubesInitialModels.push_back(cube->getModel());
	}

	return base;
}
SceneNode* createBackPiece(SceneNode* parent) {
	backpieceMesh->paint({ 222.0f/255.0f,184.0f / 255.0f,135.0f / 255.0f, 1.0f });


	float zTranslation = -((0.03f * 2.0f) + (0.5f *0.2f)); 

	SceneNode* backp = parent->createChild(backpieceMesh, Mat4::translation(0.0f, 0.0f, zTranslation)*Mat4::scaling(0.9f, 0.9f,0.03f));
	backp->setBeforeDrawFunction([](ShaderProgram* sp) {
		GL_CALL(glDisable(GL_DEPTH_TEST));
	});
	backp->setAfterDrawFunction([]() {
		GL_CALL(glEnable(GL_DEPTH_TEST));
	});
	return backp;
}
SceneNode* createFrame(SceneNode* parent) {
	frameMesh = Mesh::loadFromFile("../Engine/objs/frame.obj");
	frameMesh->paint({ 139.0f / 255.0f,69.0f / 255.0f,19.0f / 255.0f, 1.0f });
	return parent->createChild(frameMesh, Mat4::IDENTITY);
}

void AppAVT::start() {

	cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
	backpieceMesh = new Mesh();
	*backpieceMesh = *cube;
	
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderAVT.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderAVT.glsl");
	sp = new ShaderProgram(vs, fs);

	// Uniform buffer object binding point
	const GLuint uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, uboBp);

	float cameraMovementSpeed = 10.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 0.0f, 5.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 1.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 100.0f);
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, getWindowWidth() / getWindowHeight(), 0.001f, 10.0f);

	Mat4 currProjection = orthographicProj;

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, getWindow());

	// Initializing the camera and adding the controller
	camera = new Camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), currProjection, uboBp);
	camera->addCameraController(cameraController);

	sceneGraph = new SceneGraph(camera);
	sceneGraph->getRoot()->setShaderProgram(sp);
	
	frame = createFrame(sceneGraph->getRoot());
	backpiece = createBackPiece(frame);
	penroseTriangle = createPenroseTriangle(frame);
	
	sceneGraph->init();
}

static const Qtrn initialRotation(static_cast<float>(M_PI) /180.0f, {0.0f, 1.0f, 0.0f});
static const Qtrn finalRotation(static_cast<float>(M_PI) * 2.0f, { 0.0f, 1.0f, 0.0f });
static const float rotationDuration = 2.0f;
static bool rotating = false;

void rotatePainting(float elapsedTime) {

	static float currTime = 0.0f;
	currTime += elapsedTime;

	float normalizedTime;

	if (currTime > rotationDuration) {
		rotating = false;
		normalizedTime = 1.0f;
		currTime = 0.0f;
	}
	else {
		normalizedTime = currTime / rotationDuration;
	}

	Qtrn newRotation = Qtrn::slerp(initialRotation, finalRotation, normalizedTime);

	frame->setModel(newRotation.toRotationMatrix());
}


static bool animating = false;
static float translationDistance = width + offset;
static float translationDuration = 2.0f;

void animateTriangle(float elapsedTime) {

	static float currTime = 0.0f;
	currTime += elapsedTime;

	float normalizedTime;

	if (currTime > translationDuration) {
		normalizedTime = 1.0f;
		currTime = 0.0f;
	}
	else {
		normalizedTime = currTime / rotationDuration;
	}

	for (int i = 0; i < cubes.size(); ++i) {
		if (i >= 0 && i < 3) {
			Mat4 translation = Mat4::translation(translationDistance * normalizedTime, 0.0f, 0.0f);
			cubes[0]->setModel(cubesInitialModels[0] * translation);
			cubes[1]->setModel(cubesInitialModels[1] * translation);
			cubes[2]->setModel(cubesInitialModels[2] * translation);
		}
		else if (i >= 3 && i < 5) {
			Mat4 translation = Mat4::translation(0.0f, 0.0f, -(translationDistance * normalizedTime));
			cubes[3]->setModel(cubesInitialModels[3] * translation);
			cubes[4]->setModel(cubesInitialModels[4] * translation);
		}
		else {
			Mat4 translation1 = Mat4::translation(0.0f, 0.0f, -(translationDistance * normalizedTime));
			Mat4 translation2 = Mat4::translation(0.0f, translationDistance * normalizedTime, 0.0f);
			cubes[5]->setModel(cubesInitialModels[5] * translation1);
			cubes[6]->setModel(cubesInitialModels[6] * translation2);
			cubes[7]->setModel(cubesInitialModels[7] * translation2);
			cubes[8]->setModel(cubesInitialModels[8] * translation2);
		}
	}	
}

void AppAVT::update() {

	float elapsedTime = static_cast<float>(getElapsedTime());
	static bool CReleased = false;

	if (glfwGetKey(getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
		rotating = true;
	}

	if (glfwGetKey(getWindow(), GLFW_KEY_C) == GLFW_PRESS && CReleased) {		

			animating = !animating;
			CReleased = false;
	}

	if (glfwGetKey(getWindow(), GLFW_KEY_C) == GLFW_RELEASE)
		CReleased = true;

	if (rotating)
		rotatePainting(elapsedTime);

	if (animating)
		animateTriangle(elapsedTime);

	cameraController->processInputAndMove(elapsedTime);
	sceneGraph->draw();

	// Closing window if ESC is pressed
	if (glfwGetKey(getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(getWindow(), true);
	}
}

void AppAVT::end() {
	delete sceneGraph;
	delete camera;
	delete cameraController;
	delete sp;
	delete cube;
}