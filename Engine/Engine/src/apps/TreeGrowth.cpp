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
#include "Morphospace.h"
#include "Tree.h"
#include "../utils/OpenGLUtils.h"

static ShaderProgram* sp;
static ShaderProgram* spSimple;
static FreeCameraController* cameraController;
static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static Tree* tree;
static Mesh* sphere;
static Mesh* plane;
static SceneNode* sceneNode;

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
	spSimple = new ShaderProgram(vs2, fs2);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex2 = sp->getUniformBlockIndex("SharedMatrices");
	spSimple->bindUniformBlock(sharedMatricesIndex2, camera->getUboBindingPoint());
}

static void setupTextures() {
	// Loading textures
	woodTexture = new Texture2D("../Engine/textures/barkTexture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/barkNormalMap.jpg");

	GLint textureID = sp->getUniformLocation("diffuseMap");
	GLint normalMapID = sp->getUniformLocation("normalMap");

	sp->use();
	sp->setUniform(textureID, 0);
	sp->setUniform(normalMapID, 1);
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

	Vec3 cameraPos(0.0f, 0.0f, 50.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 50.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 1000.0f);
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, float(windowWidth / windowHeight), 0.001f, 1000.0f);

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, window);

	//cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -5.0f);
	camera->addCameraController(cameraController);
	camera->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	camera->setProjection(perspectiveProj);
}

static void setupTree(SceneGraph* sceneGraph) {

	GrowthParameters* growthParameters = new GrowthParameters();
	growthParameters->gP = 0.12f;
	growthParameters->scalingCoefficient = 1.29f;
	growthParameters->vRootMax = 900;
	growthParameters->thickeningFactor = 0.5f; // original 1.41
	growthParameters->pMax = 950;
	growthParameters->vMin = 0.0f;
	growthParameters->vMax = (float)growthParameters->vRootMax;
	growthParameters->determinacy = 0.93f; 
	growthParameters->apicalControl = 0.87f;
	growthParameters->tropismAngle = 0.66f;
	growthParameters->w1 = 0.14f;
	growthParameters->w2 = 0.14f;
	growthParameters->g1 = 0.2f; 
	growthParameters->g2 = 0.2f;
	GrowthParameters::instance = growthParameters;

	float scaleLength = 1.5f;
	Morphospace::instance = new Morphospace(scaleLength);

	tree = new Tree({ 0.0f, -10.0f, 0.0f }, sceneGraph, woodTexture, woodTextureNormalMap);
}

void TreeGrowth::start() {

	setupShaders(getSceneGraph(), getCamera());
	setupTextures();
	setupTree(getSceneGraph());
	setupCamera(getCamera(), getWindow(),getWindowWidth(), getWindowHeight());
	setupLight();

	// Bounding sphere
	sphere = Mesh::loadFromFile("../Engine/objs/sphere.obj");
	sphere->paint(ColorRGBA::GREEN);

	sceneNode = getSceneGraph()->getRoot()->createChild(sphere, Mat4::IDENTITY, spSimple);
	sceneNode->setBeforeDrawFunction([=](ShaderProgram* sp) {
		GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	});
	sceneNode->setAfterDrawFunction([]() {
		GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	});
};

static bool drawSphere = true;

void TreeGrowth::update() {

	static bool isBReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_B) == GLFW_PRESS && isBReleased) {
		isBReleased = false;
		drawSphere = !drawSphere;
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_B) == GLFW_RELEASE) {
		isBReleased = true;
	}

	if (drawSphere)
		sceneNode->setModel(Mat4::translation(tree->root->boundingSphere.center) * Mat4::scaling(tree->root->boundingSphere.radius * 2));
	else
		sceneNode->setModel(Mat4::ZERO);

	tree->grow(float(getElapsedTime()));

	sp->use();
	sp->setUniform(cameraPosL, cameraController->position);

	static bool isNReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_PRESS && isNReleased) {
		isNReleased = false;
		normalMapping = (normalMapping == GL_TRUE) ? GL_FALSE : GL_TRUE;
		sp->setUniform(normalMappingL, normalMapping);
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_RELEASE) {
		isNReleased = true;
	}
	sp->stopUsing();
}

void TreeGrowth::end() {
	delete cameraController;
	delete sp;
	delete spSimple;
	delete woodTexture;
	delete woodTextureNormalMap;
	delete tree;
	delete sphere;
}
