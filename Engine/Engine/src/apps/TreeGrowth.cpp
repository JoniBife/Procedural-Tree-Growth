#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeGrowth.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/OrbitCameraController.h"

static Mesh* cube;
static ShaderProgram* sp;
static ICameraController* cameraController;
static Camera* camera;
static SceneGraph* sceneGraph;

void TreeGrowth::start() {
	cube = Mesh::loadFromFile("../Engine/objs/monkey.obj");
	cube->transform(Mat4::scaling(0.15f)); // Blender Cube has a width and height of 2

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderCGJ.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderCGJ.glsl");
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
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, float(getWindowWidth() / getWindowHeight()), 0.001f, 100.0f);

	Mat4 currProjection = perspectiveProj;

	cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	//cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, getWindow());

	// Initializing the camera and adding the controller
	camera = new Camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), currProjection, uboBp);
	camera->addCameraController(cameraController);

	sceneGraph = new SceneGraph(camera);
	sceneGraph->getRoot()->setShaderProgram(sp);
	sceneGraph->getRoot()->createChild(cube, Mat4::IDENTITY);
	sceneGraph->init();
};

void TreeGrowth::update() {
	cameraController->processInputAndMove(getElapsedTime());
	sceneGraph->draw();
};

void TreeGrowth::end() {
	delete sceneGraph;
	delete camera;
	delete cameraController;
	delete sp;
	delete cube;
};