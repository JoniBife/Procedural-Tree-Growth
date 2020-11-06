#define _USE_MATH_DEFINES

#include <Math.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/Vec4.h"
#include "math/Mat4.h"
#include "utils/ColorRGBA.h"
#include "shaders/ShaderProgram.h"
#include "shapes/Shape.h"
#include "shapes/ShapeGroup.h"
#include "utils/OpenGLUtils.h"
#include "view/Transformations.h"
#include "controllers/FreeCameraController.h"
#include "controllers/OrbitCameraController.h"
#include "Configurations.h"
#include "view/Camera.h"
#include "math/Qtrn.h"


//////////////////////////////////////////////////////////////////// INITIAL CAMERA SETUP

Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.0f, 10.0f);
Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.0f, 10.0f);

Mat4 currProjection = perspectiveProj;

float cameraMovementSpeed = 2.5f;

// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
float initialYaw = -90.0f;
float initialPitch = 0.0f;

Vec3 cameraPos(0.0f, 0.0f, 2.0f); // eye
Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
Vec3 cameraFront = cameraTarget - cameraPos;
Vec3 cameraUp(0.0f, 1.0f, 0.0f); // up

///////////////////////////////////////////////////////////////////// CALLBACKS

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool orthoProjection = false;

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (orthoProjection) {
			currProjection = orthographicProj;
			orthoProjection = false;
		}
		else {
			currProjection = perspectiveProj;
			orthoProjection = true;
		}
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
void processInput(GLFWwindow* window, ShaderProgram& sp, Camera& camera, CameraController &cameraController)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//cameraController.snapToPosition(cameraPos, cameraFront, -90.0f, 0.0f);
		
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

////////////////////////////////////////////////////////////////////////// RUN CGJ

////////////////////////////////////////////////////////////////////////// CGJ
const float offset = 0.02f;
const float width = 0.3f;

void drawLineTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {

	sp.setUniform(colorUniform, color);

	// TEACHER: (IMPORTANT INFORMATION) ///////////////////////////////////////////////////////////////
	// Ideally, all translation matrix used in the tetromino functions should be placed outside the functions
	// so that they are only calculated once. We placed them inside for simplicity. 
	// This also applies to all operations with width and offset (ex: width + offset).
	///////////////////////////////////////////////////////////////////////////////////////////////////
	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -1.5f * (width + offset), 0.0f));
	square.draw();
}
void drawSquareTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawLTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), 0.0f, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();
}
void drawReverseLTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), width + offset, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), 0.0f, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();
}
void drawTTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(-1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawSTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-1.0f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawReverseSTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform, Vec4& color) {
	sp.setUniform(colorUniform, color);

	sp.setUniform(modelUniform, transform * Mat4::translation(1.0f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();
}

float orientation = float(M_PI / 4.0);
Mat4 rotation = Mat4::rotation(orientation, { 0, 0, 1 }); //to rotate the group of tetrominos

Mat4 transformationLine = rotation * Mat4::translation(0.0f, 1.5f * (width + offset), 0.0f) * Mat4::rotation(float(-M_PI_2), { 0, 0, 1 });
Mat4 transformationT1 = rotation * Mat4::translation(0.5f * (width + offset), 0.0f, 0.0f);
Mat4 transformationT2 = rotation * Mat4::translation(-1.0f * (width + offset), -0.5f * (width + offset), 0.0f) * Mat4::rotation(float(M_PI_2), { 0, 0, 1 });
Mat4 transformationL = rotation * Mat4::translation(0.5f * (width + offset), -1.0f * (width + offset), 0.0f) * Mat4::rotation(float(M_PI_2), { 0, 0, 1 });

void drawCGJ(GLint colorUniform, GLint modelUniform, ShaderProgram& sp, Shape& squareFront, Shape& squareBack, float elapsed_time) {

	squareFront.bind();

	drawLineTetromino(sp, squareFront, colorUniform, modelUniform, transformationLine, ColorRGBA::CYAN);
	drawTTetromino(sp, squareFront, colorUniform, modelUniform, transformationT1, ColorRGBA::PURPLE);
	drawTTetromino(sp, squareFront, colorUniform, modelUniform, transformationT2, ColorRGBA::RED);
	drawLTetromino(sp, squareFront, colorUniform, modelUniform, transformationL, ColorRGBA::ORANGE);

	squareBack.bind();

	Vec4 cyan2(40.8f / 255.0f, 204.0f / 255.0f, 204.0f / 255.0f, 1.0f);
	Vec4 purple2(121.12f / 255.0f, 40.8f / 255.0f, 204.0f / 255.0f, 1.0f);
	Vec4 red2(204.0f / 255.0f, 40.8f / 255.0f, 40.8f / 255.0f, 1.0f);
	Vec4 orange2(204.0f / 255.0f, 121.12f / 255.0f, 40.8f / 255.0f, 1.0f);

	drawLineTetromino(sp, squareBack, colorUniform, modelUniform, transformationLine, cyan2);
	drawTTetromino(sp, squareBack, colorUniform, modelUniform, transformationT1, purple2);
	drawTTetromino(sp, squareBack, colorUniform, modelUniform, transformationT2, red2);
	drawLTetromino(sp, squareBack, colorUniform, modelUniform, transformationL, orange2);

	squareBack.unBind();
}

void runCGJ(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	// Creating and compiling the shaders
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderCGJ.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderCGJ.glsl");
	ShaderProgram sp(vs, fs);

	// Uniform buffer object binding point
	const GLuint uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp.getUniformBlockIndex("SharedMatrices");
	sp.bindUniformBlock(sharedMatricesIndex, uboBp);

	// Obtaining the model uniform location
	GLint modelUniform = sp.getUniformLocation("model");

	// Initializing the camera controller
	//FreeCameraController cameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, win);
	OrbitCameraController cameraController(Vec3(0.0f, 0.0f, 0.0f), Qtrn(1.0f, 0.0f, 0.0f, 0.0f), win);

	// Initializing the camera and adding the controller
	Camera camera(Mat4::translation(0.0f, 0.0f, -2.0f), currProjection, uboBp);
	camera.addCameraController(cameraController);

	// Obtaining the color uniform that will be different for each tetromino
	GLint colorUniform = sp.getUniformLocation("color");

	// Square that will be used to represent the front
	Shape squareFront = Shape::square(width);
	squareFront.init();

	// Square that will be used to represent the back
	Shape squareBack({
		{ -width / 2, -width / 2, 0.0f, 1.0f }, // bottom left vertex
		{ width / 2, width / 2, 0.0f, 1.0f }, // top right vertex
		{ width / 2, -width / 2, 0.0f, 1.0f }, // bottom right vertex);
		{ -width / 2, width / 2, 0.0f, 1.0f }, // top left vertex
		{ width / 2, width / 2, 0.0f, 1.0f }, // top right vertex
		{ -width / 2, -width / 2, 0.0f, 1.0f } // bottom left vertex
		});
	squareBack.init();

	// We only need to start using the shader program once since we are always using the same
	sp.use();

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Processing input (P key, Space key and ESC key)
		processInput(win, sp, camera, cameraController);
		// Updating the camera position according to the keyboard input and mouse input
		cameraController.processInputAndMove(float(elapsed_time));

		// the projection might have been changed
		camera.setProjection(currProjection);
		camera.update();

		// Drawing the shapes
		drawCGJ(colorUniform, modelUniform, sp, squareFront, squareBack, float(elapsed_time));

		glfwSwapBuffers(win);
		glfwPollEvents();
		checkForOpenGLErrors("ERROR: MAIN/RUN");
	}

	// No longer need the shader program
	sp.stopUsing();

	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	GLFWwindow* win = setup(OPEN_GL_MAJOR, OPEN_GL_MINOR,
		SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, FULLSCREEN, VSYNC);
	//runAVT(win);
	runCGJ(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
