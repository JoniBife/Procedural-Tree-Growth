#include "Blending.h"
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


static ShaderProgram* spBlending;
static ShaderProgram* spSimple;
static FreeCameraController* cameraController;
static Mesh* windowTransparent;
static Mesh* plane;
static Texture2D* windowTexture;

static SceneNode* sceneNodePlane;
static SceneNode* sceneNodeWindows;
static SceneNode* sceneNodeWindows1;
static SceneNode* sceneNodeWindows2;

static GLint cameraPosL;

static void setupShaders(SceneGraph* sceneGraph, Camera* camera) {

	Shader vsB(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderBlending.glsl");
	Shader fsB(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderBlending.glsl");

	spBlending = new ShaderProgram(vsB, fsB);

	Shader vsSimple(GL_VERTEX_SHADER, "../Engine/shaders/SimpleVertexShader.glsl");
	Shader fsSimple(GL_FRAGMENT_SHADER, "../Engine/shaders/SimpleFragmentShader.glsl");

	spSimple = new ShaderProgram(vsSimple, fsSimple);

	GLuint sharedMatricesIndex = spBlending->getUniformBlockIndex("SharedMatrices");
	spBlending->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

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

std::vector<Vec4> transparentVertices = {
	// positions       
	(0.0f,  0.5f,  0.0f, 1.0f),
	(0.0f, -0.5f,  0.0f, 1.0f),
	(1.0f, -0.5f,  0.0f, 1.0f),

	(0.0f,  0.5f,  0.0f, 1.0f),
	(1.0f, -0.5f,  0.0f, 1.0f),
	(1.0f,  0.5f,  0.0f, 1.0f)
};

std::vector<Vec2> transparentTextCoords = {
	// texture Coords (swapped y coordinates because texture is flipped upside down)
	(0.0f,  0.0f),
	(0.0f,  1.0f),
	(1.0f,  1.0f),

	(0.0f,  0.0f),
	(1.0f,  1.0f),
	(1.0f,  0.0f)
};

static void setupTextures() {
	// Loading textures
	windowTexture = new Texture2D("../Engine/textures/leaves2.png");

	GLint textureID = spBlending->getUniformLocation("texture1");

	spBlending->use();
	spBlending->setUniform(textureID, 0);
}

void Blending::start() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT_AND_BACK);

	setupShaders(getSceneGraph(), getCamera());
	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());
	setupTextures();
	/*/
	plane = Mesh::loadFromFile("../Engine/objs/plane.obj");
	plane->paint(ColorRGBA::ORANGE);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, spSimple);
	sceneNodePlane->setModel(Mat4::translation({ 0.0f, -0.5f, 0.0f }));
	sceneNodePlane->setShaderProgram(spSimple);
	/**/
	windowTransparent = Mesh::rectangle(1.0f, 1.0f);

	std::vector<Vec3> windows;
	windows.push_back(Vec3(0.0f, 0.0f, 0.0f));
	windows.push_back(Vec3(1.5f, 0.0f, 0.51f));
	windows.push_back(Vec3(-0.5f, 0.0f, -0.48f));

	sceneNodeWindows = getSceneGraph()->getRoot()->createChild(windowTransparent, Mat4::IDENTITY, spBlending);
	sceneNodeWindows1 = getSceneGraph()->getRoot()->createChild(windowTransparent, Mat4::IDENTITY, spBlending);
	//sceneNodeWindows2 = getSceneGraph()->getRoot()->createChild(windowTransparent, Mat4::IDENTITY, spBlending);

	sceneNodeWindows->setModel(Mat4::rotation(PI / 3, { 1.0f, 0.0f, 0.0f }) * Mat4::translation({ 0.0f, 0.5f, 0.0f }));
	sceneNodeWindows1->setModel(Mat4::rotation( 2* PI / 3, { 1.0f, 0.0f, 0.0f }) * Mat4::translation({ 0.0f, 0.5f, 0.0f }));

	sceneNodeWindows->setShaderProgram(spBlending);
	sceneNodeWindows1->setShaderProgram(spBlending);
	//sceneNodeWindows2->setShaderProgram(spBlending);

	sceneNodeWindows->addTexture(windowTexture);
	sceneNodeWindows1->addTexture(windowTexture);
	//sceneNodeWindows2->addTexture(windowTexture);

}

void Blending::update() {
}

void Blending::end() {
	delete cameraController;
	delete spBlending;
	delete spSimple;
	delete windowTransparent;
	delete plane;
}