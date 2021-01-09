#include "CylinderGeneration.h"
#include "../view/Transformations.h"
#include "../controllers/FreeCameraController.h"
#include "../math/MathAux.h"
#include "../textures/Texture2D.h"

static ShaderProgram* sp;
static Mesh* points;
static FreeCameraController* cameraController;
static SceneNode* cylinder;

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

	GLint textureID = sp->getUniformLocation("wood");

	sp->use();
	sp->setUniform(textureID, 0);
	sp->stopUsing();

	cylinder = getSceneGraph()->getRoot()->createChild(points, Mat4::rotation(PI/2, Vec3::X), sp);
	cylinder->addTexture(new Texture2D("../Engine/textures/barkTexture.jpg"));
}

void Cylinder::update() {

}

void Cylinder::end() {

}