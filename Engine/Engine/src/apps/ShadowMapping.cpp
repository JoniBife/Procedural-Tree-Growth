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

static ShaderProgram* sp;
static ShaderProgram* sp2;
static FreeCameraController* cameraController;
static Mesh* cube;
static Mesh* plane;

static SceneNode* sceneNodePlane;

static GLint cameraPosL;
static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

static void setupShaders(SceneGraph* sceneGraph, Camera* camera) {
	// Loading the shaders and creating the shader program
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderNM.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderNM.glsl");
	sp = new ShaderProgram(vs, fs);
	sceneGraph->getRoot()->setShaderProgram(sp);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	// Loading the shaders and creating the shader program
	Shader vs2(GL_VERTEX_SHADER, "../Engine/shaders/simpleVertexShader.glsl");
	Shader fs2(GL_FRAGMENT_SHADER, "../Engine/shaders/simpleFragmentShader.glsl");
	sp2 = new ShaderProgram(vs2, fs2);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex2 = sp->getUniformBlockIndex("SharedMatrices");
	sp2->bindUniformBlock(sharedMatricesIndex2, camera->getUboBindingPoint());
}
static void setupLight() {
	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.1f;

	Vec4 lightPosition(1.5f, 5.0f, 5.0f, 1.0f);

	float specularStrength = 1.0f;
	unsigned int shininess = 4;

	GLint lightColorL = sp->getUniformLocation("lightColor");
	GLint ambientStrenghtL = sp->getUniformLocation("ambientStrength");
	GLint lightPositionL = sp->getUniformLocation("lightPosition");
	GLint specularStrengthL = sp->getUniformLocation("specularStrength");
	GLint shininessL = sp->getUniformLocation("shininess");
	normalMappingL = sp->getUniformLocation("normalMapping");
	cameraPosL = sp->getUniformLocation("viewPos");

	sp->use();
	sp->setUniform(lightColorL, lightColor);
	sp->setUniform(ambientStrenghtL, ambientStrength);
	sp->setUniform(lightPositionL, lightPosition * 2);
	sp->setUniform(specularStrengthL, specularStrength);
	sp->setUniform(shininessL, shininess);
	sp->setUniform(normalMappingL, normalMapping);
	sp->stopUsing();
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

void ShadowMapping::start() {
	setupShaders(getSceneGraph(), getCamera());
	setupLight();
	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());

	cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
	cube->paint(ColorRGBA::GREEN);
	SceneNode* sceneNode = getSceneGraph()->getRoot()->createChild(cube, Mat4::IDENTITY, sp2);
	sceneNode->setModel(Mat4::translation({ 0.0f, 3.0f, 0.0f }));
	
	plane = Mesh::loadFromFile("../Engine/objs/plane.obj");
	plane->paint(ColorRGBA::WHITE);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, sp2);
	sceneNodePlane->setModel(Mat4::translation({ 0.0f, 0.0f, 0.0f }));
}


void ShadowMapping::update() {

}

void ShadowMapping::end() {
	delete cameraController;
	delete sp;
	delete sp2;
	delete cube;
	delete plane;
}