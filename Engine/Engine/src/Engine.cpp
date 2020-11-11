#define _USE_MATH_DEFINES


#include <math.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/MathAux.h"
#include "math/Vec4.h"
#include "math/Mat4.h"
#include "utils/ColorRGBA.h"
#include "shaders/ShaderProgram.h"
#include "meshes/Mesh.h"
#include "meshes/MeshGroup.h"
#include "utils/OpenGLUtils.h"
#include "view/Transformations.h"
#include "controllers/FreeCameraController.h"
#include "controllers/OrbitCameraController.h"
#include "Configurations.h"
#include "view/Camera.h"
#include "math/Qtrn.h"
#include "scene/SceneGraph.h"


//////////////////////////////////////////////////////////////////// INITIAL CAMERA SETUP

Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.0f, 100.0f);
Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.0f, 100.0f);

Mat4 currProjection = perspectiveProj;

float cameraMovementSpeed = 20.0f;

// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
float initialYaw = -90.0f;
float initialPitch = 0.0f;

Vec3 cameraPos(0.0f, 0.0f, 10.0f); // eye
Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
Vec3 cameraFront = cameraTarget - cameraPos;
Vec3 cameraUp(0.0f, 1.0f, 0.0f); // up

///////////////////////////////////////////////////////////////////// CALLBACKS

bool quat = false;
bool animate = false;
float currTime = 0.0f;

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool orthoProjection = false;

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		currTime = 0.0f;
		animate = !animate;
	} else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if(!animate)
			quat = !quat;
	}
}

void window_close_callback(GLFWwindow* win)
{
	// TODO
	// For now the shaders and shapes are being deleted at the end
	// of the runCGJ and runAVT functions automatically becauses 
	// they go out of scope so the destructor is called automatically
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

///////////////////////////////////////////////////////////////////////// SETUP

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetKeyCallback(win, glfw_key_callback);
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	GL_CALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LEQUAL));
	GL_CALL(glDepthMask(GL_TRUE));
	GL_CALL(glDepthRange(0.0, 1.0));
	GL_CALL(glClearDepth(1.0));
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glCullFace(GL_BACK));
	GL_CALL(glFrontFace(GL_CCW));
	GL_CALL(glViewport(0, 0, winx, winy));
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	/*createShaderProgram();
	createBufferObjectsAvt();*/
	return win;
}

////////////////////////////////////////////////////////////////////////// INPUT
void processInput(GLFWwindow* window, ShaderProgram& sp, Camera& camera)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//cameraController.snapToPosition(cameraPos, cameraFront, -90.0f, 0.0f);
		
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

////////////////////////////////////////////////////////////////////////// RUN 

void draw(GLint uniformLocation, ShaderProgram& sp, std::unique_ptr<Mesh>& cube) {
	
	cube->bind();
	sp.setUniform(uniformLocation, Mat4::IDENTITY);
	cube->draw();
	cube->unBind();
}

const float offset = 0.02f;
const float width = 0.3f;

SceneNode* createLineTetromino(SceneNode* sceneGraph, ShaderProgram* sp, Mesh* cube) {

	SceneNode* base = sceneGraph->createChild();

	base->createChild(cube, Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.0f, -1.5f * (width + offset), 0.0f));

	return base;
}

SceneNode* createLTetromino(SceneNode* sceneGraph, ShaderProgram* sp, Mesh* cube) {

	SceneNode* base = sceneGraph->createChild();

	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f));
	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), 0.0f, 0.0f));
	base->createChild(cube, Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	base->createChild(cube, Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));

	return base;
}

SceneNode* createTTetromino(SceneNode* sceneGraph, ShaderProgram* sp, Mesh* cube) {

	SceneNode* base = sceneGraph->createChild();

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

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	std::unique_ptr<Mesh> cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
	cube->transform(Mat4::scaling(0.15f)); // Blender Cube has a width and height of 2
	//cube->paint(ColorRGBA::BLUE);

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderAVT.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderAVT.glsl");
	ShaderProgram* sp = new ShaderProgram(vs, fs);

	// Uniform buffer object binding point
	const GLuint uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, uboBp);

	// Obtaining the model uniform location
	//GLint modelUniform = sp.getUniformLocation("model");

	FreeCameraController cameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, win);

	// Initializing the camera and adding the controller
	Camera* camera = new Camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), currProjection, uboBp);
	camera->addCameraController(cameraController);

	SceneGraph* sceneGraph = new SceneGraph(camera);
	sceneGraph->getRoot()->setShaderProgram(sp);
	//sceneGraph->getRoot()->setModel(Mat4::rotation(M_PI_2, Vec3::X));

	// Base
	//SceneNode* base = sceneGraph->getRoot()->createChild(cube.get(), Mat4::scaling(10.0f, 0.25f, 10.0f));
	//sceneGraph->getRoot()->createChild(cube.get(), Mat4::IDENTITY);
	//SceneNode* lineTetromino = createLineTetromino(sceneGraph->getRoot(), sp, cube.get());
	//SceneNode* LTetromino = createLTetromino(sceneGraph->getRoot(), sp, cube.get());
	//SceneNode* TTetromino1 = createTTetromino(sceneGraph, sp, cube.get());
	SceneNode* TTetromino2 = createTTetromino(sceneGraph->getRoot(), sp, cube.get());

	sceneGraph->init();

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Processing input (P key, Space key and ESC key)
		//processInput(win, sp, );

		// Updating the camera position according to the keyboard input and mouse input
		cameraController.processInputAndMove(float(elapsed_time));

		// the projection might have been changed
		sceneGraph->getCamera()->setProjection(currProjection);
		// Finally updating the view matrix and projection matrices with their new values
		sceneGraph->draw();

		//draw(modelUniform, sp, cube);
		glfwSwapBuffers(win);
		glfwPollEvents();
		checkForOpenGLErrors("ERROR: MAIN/RUN");
	}

	delete sceneGraph;
	delete camera;
	delete sp;

	//sp.stopUsing();
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	GLFWwindow* win = setup(OPEN_GL_MAJOR, OPEN_GL_MINOR,
		SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, FULLSCREEN, VSYNC);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
