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
ShaderProgram* sp;
ICameraController* cameraController;
Camera* camera;
SceneGraph* sceneGraph;
SceneNode* ground;
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
	ground = sceneGraph->getRoot()->createChild(cube, Mat4::translation(0.0f, -(2.0f * width + 1.5f*offset + 0.5f * 0.05f * width), 0.0f) * Mat4::scaling({ 10.0f, 0.05f, 10.0f }));
	GLuint colorUniformLocation = sp->getUniformLocation("color");
	ground->setBeforeDrawFunction([=](ShaderProgram* sp) {
		sp->setUniform(colorUniformLocation, ColorRGBA::BLACK + 0.2f);
	});

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

int state = 0;

float totalDuration = 1.0f;
float currentTime = 0.0f;

//Step 1 - L Transformation
Vec3 LTranslation0(0, 0, width + offset);
Vec3 LTranslation1(0, 4 * (width + offset), 0);
Vec3 LTranslation2(0, 0, -(width + offset));
Vec3 LTranslation3((width + offset), 0, 0);
Vec3 LTranslation4(-(width + offset), -(width + offset), 0);

void LAnimation0(float normalizedTime) {

	switch (state) {
		case 0: {
			Vec3 translation = Vec3::lerp(Vec3::ZERO, LTranslation0, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 1: {
			Vec3 translation = Vec3::lerp(Vec3::ZERO, LTranslation1, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 2: {
			Vec3 translation = Vec3::lerp(Vec3::ZERO, LTranslation2, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 3: {
			Vec3 translation = Vec3::lerp(Vec3::ZERO, LTranslation3, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 4: {
			Vec3 translation = Vec3::lerp(Vec3::ZERO, LTranslation4, normalizedTime);
			Mat4 transformationLAux = Mat4::translation(translation) * transformationL;
			Mat4 transformationLineAux = Mat4::translation(translation) * transformationLine;
			LTetromino->setModel(transformationLAux);
			lineTetromino->setModel(transformationLineAux);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformationLAux;
				transformationLine = transformationLineAux;
			}
			break;
		}
		default: return;
	}
}
void LAnimation0Rev(float normalizedTime) {
	switch (state) {
		case 1: {
			Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, LTranslation0, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 2: {
			Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, LTranslation1, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 3: {
			Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, LTranslation2, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 4: {
			Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, LTranslation3, normalizedTime);
			Mat4 transformation = Mat4::translation(translation) * transformationL;
			LTetromino->setModel(transformation);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformation;
			}
			break;
		}
		case 5: {
			Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, LTranslation4, normalizedTime);
			Mat4 transformationLAux = Mat4::translation(translation) * transformationL;
			Mat4 transformationLineAux = Mat4::translation(translation) * transformationLine;
			LTetromino->setModel(transformationLAux);
			lineTetromino->setModel(transformationLineAux);

			if (normalizedTime == 1.0f)
			{
				transformationL = transformationLAux;
				transformationLine = transformationLineAux;
			}
			break;
		}
		default: return;
	}
}

//Step 2 - T Transformations
Vec3 T1Translation0(0, 0, -(width + offset));
Vec3 T1Translation1(-0.5f*(width + offset), 0, -1.5f * (width + offset));
Vec3 T1Translation2(0.5f * (width + offset), 0.5f * (width + offset), 0);
Qtrn T1Initial(1, 0, 0, 0);
Qtrn T1Final(float(M_PI_2), { 0,1,0 });
Qtrn T1Final2(float(-M_PI_2), { 1,0,0 });

Vec3 T2Translation0(0, 0, width + offset);
Vec3 T2Translation1(0, -0.5f* (width + offset), 1.5f * (width + offset));
Qtrn T2Initial(1, 0, 0, 0);
Qtrn T2Final(float(-M_PI_2), { 1,0,0 });

void T1Animation0(float normalizedTime)	
{
	switch (state) {
	case 0: {

		Vec3 translation = Vec3::lerp(Vec3::ZERO, T1Translation0, normalizedTime);
		Mat4 transformation = Mat4::translation(translation) * transformationT1;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	case 1: {
		Vec3 translation = Vec3::lerp(Vec3::ZERO, T1Translation1, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T1Initial, T1Final, normalizedTime).toRotationMatrix();
		Mat4 transformation = Mat4::translation(translation) * rotation * transformationT1;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	case 2: {
		Vec3 translation = Vec3::lerp(Vec3::ZERO, T1Translation2, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T1Initial, T1Final2, normalizedTime).toRotationMatrix();
		Mat4 transformation = Mat4::translation(translation) * transformationT1 * rotation;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	default: return;
	}
}
void T1Animation0Rev(float normalizedTime)
{
	switch (state) {
	case 1: {
		Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, T1Translation0, normalizedTime);
		Mat4 transformation = Mat4::translation(translation) * transformationT1;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	case 2: {
		Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, T1Translation1, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T1Initial, T1Final.inverse(), normalizedTime).toRotationMatrix();
		Mat4 transformation = rotation * Mat4::translation(translation) * transformationT1;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	case 3: {
		Vec3 translation = -1 * Vec3::lerp(Vec3::ZERO, T1Translation2, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T1Initial, T1Final2.inverse(), normalizedTime).toRotationMatrix();
		Mat4 transformation = Mat4::translation(translation) * transformationT1 * rotation;
		TTetromino1->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT1 = transformation;
		}
		break;
	}
	default: return;
	}
}

void T2Animation0(float normalizedTime)
{
	switch (state) {
	case 0: {	
		Vec3 translation = Vec3::lerp(Vec3::ZERO, T2Translation0, normalizedTime);
		Mat4 transformation = Mat4::translation(translation) * transformationT2;
		TTetromino2->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT2 = transformation;
		}
		break;
	}
	case 1: {
		Vec3 translation = Vec3::lerp(Vec3::ZERO, T2Translation1, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T2Initial, T2Final, normalizedTime).toRotationMatrix();
		Mat4 transformation = Mat4::translation(translation) * rotation * transformationT2;
		TTetromino2->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT2 = transformation;
		}
		break;
	}
	default: return;
	}
}
void T2Animation0Rev(float normalizedTime)
{
	switch (state) {
	case 1: {
		Vec3 translation = -1* Vec3::lerp(Vec3::ZERO, T2Translation0, normalizedTime);
		Mat4 transformation = Mat4::translation(translation) * transformationT2;
		TTetromino2->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT2 = transformation;
		}
		break;
	}
	case 2: {
		Vec3 translation = -1* Vec3::lerp(Vec3::ZERO, T2Translation1, normalizedTime);
		Mat4 rotation = Qtrn::slerp(T2Initial, T2Final.inverse(), normalizedTime).toRotationMatrix();
		Mat4 transformation =  rotation * Mat4::translation(translation) * transformationT2;
		TTetromino2->setModel(transformation);

		if (normalizedTime == 1.0f)
		{
			transformationT2 = transformation;
		}
		break;
	}
	default: return;
	}
}

bool playingAnimation = false;
int animationType = 1; // 1 means transforming from square, -1 means transforming to square

int movementSpeed = 2;
void updateGround(float elapsedTime, GLFWwindow* win) {

	Vec3 translation = Vec3::ZERO;
	bool moved = false;

	// Converting from units per second to units per frameMesh
	float ms = movementSpeed * elapsedTime;

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		translation -= (ms)* Vec3::Z;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		translation += (ms)* Vec3::Z;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		translation -= (ms)* Vec3::X;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		translation += (ms)* Vec3::X;
	}

	Mat4 curr = sceneGraph->getRoot()->getModel();

	sceneGraph->getRoot()->setModel(Mat4::translation(translation) * curr);
}

void AppCGJ::update() { 

	float elapsedTime = static_cast<float>(getElapsedTime());

	if (glfwGetKey(getWindow(), GLFW_KEY_T) == GLFW_PRESS) {
		playingAnimation = true;
	}

	if (playingAnimation) {
		currentTime += elapsedTime;

		if (currentTime > totalDuration) {
			currentTime = totalDuration;
		}

		float normalizedTime = currentTime / totalDuration;

		if (animationType == 1) {
			LAnimation0(normalizedTime);
			T1Animation0(normalizedTime);
			T2Animation0(normalizedTime);
		}
		else {
			LAnimation0Rev(normalizedTime);
			T1Animation0Rev(normalizedTime);
			T2Animation0Rev(normalizedTime);
		}

		// We advance animation state when totalDuration passes
		if (normalizedTime == 1.0f) {
			state += animationType;
			currentTime = 0;
			if (state == 5 || state == 0) {
				playingAnimation = false;
				animationType *= -1;
			}
		}
	}

	updateGround(elapsedTime, getWindow());
	
	cameraController->processInputAndMove(elapsedTime);
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
}