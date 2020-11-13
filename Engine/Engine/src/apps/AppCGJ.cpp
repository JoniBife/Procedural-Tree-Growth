#include "AppCGJ.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../controllers/FreeCameraController.h"
#include "../controllers/OrbitCameraController.h"
#include "../view/Camera.h"
#include "../view/Transformations.h"
#include "../scene/SceneGraph.h"
#include "../math/Qtrn.h"
#include "../utils/ColorRGBA.h"

Mesh* cube;
Mesh* cube2;
ShaderProgram* sp;
ICameraController* cameraController;
Camera* camera;
SceneGraph* sceneGraph;
SceneNode* lineTetromino;
SceneNode* LTetromino;
SceneNode* TTetromino1;
SceneNode* TTetromino2;

const float offset = 0.02f;
const float width = 0.3f;
SceneNode* createLineTetromino(SceneGraph* sceneGraph, Mesh* cube) {

	GLuint colorUniformLocation = sp->getUniformLocation("color");

	SceneNode* base = sceneGraph->createChild();
	base->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(colorUniformLocation, ColorRGBA::CYAN);
	});

	base->createChild(cube, Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -1.5f * (width + offset), 0.0f));

	return base;
}
SceneNode* createLTetromino(SceneGraph* sceneGraph, Mesh* cube) {

	GLuint colorUniformLocation = sp->getUniformLocation("color");

	SceneNode* base = sceneGraph->createChild();
	base->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(colorUniformLocation, ColorRGBA::ORANGE);
	});

	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f));
	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), 0.0f, 0.0f));
	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));

	return base;
}
SceneNode* createTTetromino(SceneGraph* sceneGraph, Mesh* cube, const Vec4& color) {

	GLuint colorUniformLocation = sp->getUniformLocation("color");

	SceneNode* base = sceneGraph->createChild();
	base->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(colorUniformLocation, color);
	});

	base->createChild(cube, Mat4::translation(-1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));

	return base;
}

Mat4 transformationLine = Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f) * Mat4::rotation(float(-M_PI_2), { 0, 0, 1 });
Mat4 transformationT1 = Mat4::translation(0.5f * (width + offset), 0.0f, 0.0f);
Mat4 transformationT2 = Mat4::translation(-1.0f * (width + offset), -0.5f * (width + offset), 0.0f) * Mat4::rotation(float(M_PI_2), { 0, 0, 1 });
Mat4 transformationL = Mat4::translation(0.5f * (width + offset), -1.0f * (width + offset), 0.0f) * Mat4::rotation(float(M_PI_2), { 0, 0, 1 });

void AppCGJ::start() { 
	cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
	cube->transform(Mat4::scaling(0.15f)); // Blender Cube has a width and height of 2

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
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, getWindowWidth() / getWindowHeight(), 0.001f, 100.0f);

	Mat4 currProjection = perspectiveProj;

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, getWindow());

	// Initializing the camera and adding the controller
	camera = new Camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), currProjection, uboBp);
	camera->addCameraController(cameraController);

	sceneGraph = new SceneGraph(camera);
	sceneGraph->getRoot()->setShaderProgram(sp);
	sceneGraph->getRoot()->setModel(Mat4::rotation(M_PI_2, Vec3::Y));
	//sceneGraph->getRoot()->createChild(cube, Mat4::IDENTITY);

	// Base
	//SceneNode* base = sceneGraph->getRoot()->createChild(cube.get(), Mat4::scaling(10.0f, 0.25f, 10.0f));
	lineTetromino = createLineTetromino(sceneGraph, cube);
	LTetromino = createLTetromino(sceneGraph, cube);
	TTetromino1 = createTTetromino(sceneGraph, cube, ColorRGBA::PURPLE);
	TTetromino2 = createTTetromino(sceneGraph, cube, ColorRGBA::RED);

	lineTetromino->setModel(transformationLine);
	LTetromino->setModel(transformationL);
	TTetromino1->setModel(transformationT1);
	TTetromino2->setModel(transformationT2);

	sceneGraph->init();
}

unsigned int currState = 0;
float totalDuration = 2.0f;
float currentTime = 0.0f;

//state 0
Vec3 LTranslation1(0, 0, width + offset);
Vec3 LTranslation2(0, 4 * (width + offset), 0);
Vec3 LTranslation3(0, 0, -(width + offset));




Mat4 nextModel;


void AppCGJ::update() { 
	
	currentTime += static_cast<float>(getElapsedTime());

	if (currentTime > totalDuration) {
		currentTime = totalDuration;
	}

	float normalizedTime = currentTime / totalDuration;

	if (currState == 0) {
		Vec3 translation = LTranslation1 * normalizedTime;
		Mat4 transformation = Mat4::translation(translation) * transformationL;
		LTetromino->setModel(transformation);

		if(translation == LTranslation1)
		{
			currState++;
			currentTime = 0;
			transformationL = transformation;
		}
	}
	else if (currState == 1) {
		Vec3 translation = LTranslation2 * normalizedTime;
		Mat4 transformation = Mat4::translation(translation) * transformationL;
		LTetromino->setModel(transformation);

		if (translation == LTranslation2)
		{
			currState++;
			currentTime = 0;
			transformationL = transformation;
		}
	}
	else if (currState == 2) {
		Vec3 translation = LTranslation3 * normalizedTime;
		Mat4 transformation = Mat4::translation(translation) * transformationL;
		LTetromino->setModel(transformation);

		if (translation == LTranslation3)
		{
			currState++;
			currentTime = 0;
			transformationL = transformation;
		}
	}
	else if (currState == 3) {

	}

	
	cameraController->processInputAndMove(static_cast<float>(getElapsedTime()));
	sceneGraph->draw();

	// Closing window if ESC is pressed
	if (glfwGetKey(getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(getWindow(), true);
	}


}

void AppCGJ::end() { 
	delete sceneGraph;
	delete camera;
	delete cameraController;
	delete sp;
	delete cube;
	delete cube2;
}