#include "CylinderGeneration.h"
#include "../view/Transformations.h"
#include "../controllers/FreeCameraController.h"
#include "../math/MathAux.h"
#include "../textures/Texture2D.h"
#include "TreeGrowthUI.h"

static ShaderProgram* sp;
static Mesh* points;
static FreeCameraController* cameraController;
static SceneNode* cylinder;
static float numberOfFrames = 0.0f;
static float totalElapsedTime = 0.0f;

static GLint cameraPosL;
static Vec3 lightPosition(-2.0f, 4.0f, -1.0f);

static TreeGrowthUI* treeGrowthUI;


static void setupLight() {
	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.3f;

	float specularStrength = 1.0f;
	unsigned int shininess = 32;

	GLint lightColorL = sp->getUniformLocation("lightColor");
	GLint ambientStrenghtL = sp->getUniformLocation("ambientStrength");
	GLint lightPositionL = sp->getUniformLocation("lightPosition");
	GLint specularStrengthL = sp->getUniformLocation("specularStrength");
	GLint shininessL = sp->getUniformLocation("shininess");
	cameraPosL = sp->getUniformLocation("viewPos");

	sp->use();
	sp->setUniform(lightColorL, lightColor);
	sp->setUniform(ambientStrenghtL, ambientStrength);
	sp->setUniform(lightPositionL, lightPosition);
	sp->setUniform(specularStrengthL, specularStrength);
	sp->setUniform(shininessL, shininess);
	sp->stopUsing();
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

	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/cylinderVertexShader.glsl");
	Shader gs(GL_GEOMETRY_SHADER, "../Engine/shaders/cylinderGeometryShader.geom");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/cylinderFragmentShader.glsl");

	sp = new ShaderProgram(vs, gs, fs);

	std::vector<Vec4> vertices = { 
		{0.0f, -2.0f, 0.0f , 1.5f},
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
		{-2.5f, 6.0f, 0.0f , 0.2f},
		{-1.5f, 4.0f, 0.0f , 0.3f},
		{-0.5f, 6.0f, 0.0f , 0.2f}

	};

	points = new Mesh(vertices);
	points->setPrimitive(GL_LINES);

	cylinder = getSceneGraph()->getRoot()->createChild(points, Mat4::IDENTITY, sp);


	for (int i = 0; i < 150; ++i) {
		cylinder = getSceneGraph()->getRoot()->createChild(points, Mat4::rotation(degreesToRadians(i*2), Vec3::Y), sp);

	}


	//cylinder->addTexture(new Texture2D("../Engine/textures/barkTexture.jpg"));

	setupLight();
}

void Cylinder::update() {
	treeGrowthUI->updateFPSCounter(float(getElapsedTime()));
	sp->use();
	sp->setUniform(cameraPosL, cameraController->position);
	sp->stopUsing();
}

void Cylinder::end() {

}