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
#include "Configurations.h"
#include "view/Camera.h"

///////////////////////////////////////////////////////////////////// CALLBACKS

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

void processInput(GLFWwindow* window, ShaderProgram& sp, Camera& camera, FreeCameraController &cameraController)
{
	Vec3 cameraPos(0.0f, 0.0f, 5.0f);
	Vec3 cameraFront(0.0f, 0.0f, -1.0f);
	Vec3 cameraUp(0.0f, 1.0f, 0.0f);

	static bool orthoProjection = true;

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !orthoProjection) {
		camera.setProjection(ortho(-2, 2, -2, 2, 1, 100));
		orthoProjection = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && orthoProjection) {
		camera.setProjection(perspective(M_PI / 6, SCREEN_WIDTH / SCREEN_HEIGHT, 1, 100));
		orthoProjection = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraController.snapToPosition(cameraPos, cameraFront);
		
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

////////////////////////////////////////////////////////////////////////// RUN AVT

void drawAVT(GLint uniformLocation, ShaderProgram& sp, Shape& semiTriangleRed, Shape& semiTriangleBlue, Shape& semiTriangleGreen, Mat4& transformationBlue, Mat4& transformationGreen) {


	// Red triangle
	semiTriangleRed.bind();
	sp.setUniform(uniformLocation, Mat4::IDENTITY);
	semiTriangleRed.draw();

	// Blue triangle
	semiTriangleBlue.bind();
	sp.setUniform(uniformLocation, transformationBlue);
	semiTriangleBlue.draw();

	// Green 
	semiTriangleGreen.bind();
	sp.setUniform(uniformLocation, transformationGreen);
	semiTriangleGreen.draw();

	semiTriangleGreen.unBind();

}

void runAVT(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	std::vector<Vec4> vertices = {
		//FRONT
		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ -0.605f, -0.48f, 0.0f, 1.0f }, //2
		{ -0.54f, -0.605f, 0.0f, 1.0f }, //3

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ -0.54f, -0.605f, 0.0f, 1.0f }, //3
		{ 0.01f, 0.34f, 0.0f, 1.0f }, //1

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ 0.01f, 0.34f, 0.0f, 1.0f }, //1
		{ 0.42f, -0.37f, 0.0f, 1.0f }, //5

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ 0.42f, -0.37f, 0.0f, 1.0f }, //5
		{ 0.56f, -0.37f, 0.0f, 1.0f }, //4

		//BACK
		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ -0.54f, -0.605f, 0.0f, 1.0f }, //3
		{ -0.605f, -0.48f, 0.0f, 1.0f }, //2

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ 0.01f, 0.34f, 0.0f, 1.0f }, //1
		{ -0.54f, -0.605f, 0.0f, 1.0f }, //3

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ 0.42f, -0.37f, 0.0f, 1.0f }, //5
		{ 0.01f, 0.34f, 0.0f, 1.0f }, //1

		{ 0.01f, 0.58f, 0.0f, 1.0f }, //0
		{ 0.56f, -0.37f, 0.0f, 1.0f }, //4
		{ 0.42f, -0.37f, 0.0f, 1.0f }, //5
	};

	const Vec4 RED2 = { 127.5f / 255.0f, 25.5f / 255.0f,  25.5f / 255.0f, 1.0f };
	const Vec4 RED2_DARK = { 25.5f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };

	std::vector<Vec4> colorsRed = {
		ColorRGBA::RED, //0
		{ 0.3f, 0.0f, 0.0f, 1.0f }, //2
		{ 0.3f, 0.0f, 0.0f, 1.0f }, //3

		ColorRGBA::RED, //0
		{ 0.3f, 0.0f, 0.0f, 1.0f }, //3
		ColorRGBA::RED, //1

		ColorRGBA::RED, //0
		ColorRGBA::RED, //1
		{ 0.3f, 0.0f, 0.0f, 1.0f },  //5

		ColorRGBA::RED, //0
		{ 0.3f, 0.0f, 0.0f, 1.0f },  //5
		{ 0.3f, 0.0f, 0.0f, 1.0f }, //4
		
		RED2, //0
		RED2_DARK, //3
		RED2_DARK, //2

		RED2, //0
		RED2, //1
		RED2_DARK, //3

		RED2, //0
		RED2_DARK,  //5
		RED2, //1

		RED2, //0
		RED2_DARK, //4
		RED2_DARK  //5

	};

	const Vec4 BLUE2 = {  25.5f / 255.0f, 25.5f / 255.0f, 127.5f / 255.0f, 1.0f };
	const Vec4 BLUE2_DARK = { 0.0f / 255.0f, 0.0f / 255.0f, 25.5f / 255.0f, 1.0f };

	std::vector<Vec4> colorsBlue = {

		ColorRGBA::BLUE, //0
		{ 0.0f, 0.0f, 0.3f, 1.0f }, //2
		{ 0.0f, 0.0f, 0.3f, 1.0f }, //3

		ColorRGBA::BLUE, //0
		{ 0.0f, 0.0f, 0.3f, 1.0f }, //3
		ColorRGBA::BLUE, //1

		ColorRGBA::BLUE, //0
		ColorRGBA::BLUE, //1
		{ 0.0f, 0.0f, 0.3f, 1.0f },  //5

		ColorRGBA::BLUE, //0
		{ 0.0f, 0.0f, 0.3f, 1.0f },  //5
		{ 0.0f, 0.0f, 0.3f, 1.0f }, //4

		BLUE2, //0
		BLUE2_DARK, //3
		BLUE2_DARK, //2

		BLUE2, //0
		BLUE2, //1
		BLUE2_DARK, //3

		BLUE2, //0
		BLUE2_DARK,  //5
		BLUE2, //1

		BLUE2, //0
		BLUE2_DARK, //4
		BLUE2_DARK  //5
	};

	const Vec4 GREEN2 = { 25.5f / 255.0f, 127.5f / 255.0f, 25.5f / 255.0f, 1.0f };
	const Vec4 GREEN2_DARK = { 0.0f / 255.0f, 25.5f / 255.0f, 0.0f / 255.0f, 1.0f };
	
	std::vector<Vec4> colorsGreen = {
		ColorRGBA::GREEN, //0
		{ 0.0f, 0.3f, 0.0f, 1.0f }, //2
		{ 0.0f, 0.3f, 0.0f, 1.0f }, //3

		ColorRGBA::GREEN, //0
		{ 0.0f, 0.3f, 0.0f, 1.0f }, //3
		ColorRGBA::GREEN, //1

		ColorRGBA::GREEN, //0
		ColorRGBA::GREEN, //1
		{ 0.0f, 0.3f, 0.0f, 1.0f },  //5

		ColorRGBA::GREEN, //0
		{ 0.0f, 0.3f, 0.0f, 1.0f },  //5
		{ 0.0f, 0.3f, 0.0f, 1.0f }, //4

		GREEN2, //0
		GREEN2_DARK, //3
		GREEN2_DARK, //2

		GREEN2, //0
		GREEN2, //1
		GREEN2_DARK, //3

		GREEN2, //0
		GREEN2_DARK,  //5
		GREEN2, //1

		GREEN2, //0
		GREEN2_DARK, //4
		GREEN2_DARK  //5
	};
	
	Shape semiTriangleRed(vertices, colorsRed);

	Shape semiTriangleBlue = semiTriangleRed;
	semiTriangleBlue.colors = colorsBlue;

	Shape semiTriangleGreen = semiTriangleRed;
	semiTriangleGreen.colors = colorsGreen;

	semiTriangleRed.init();
	semiTriangleBlue.init();
	semiTriangleGreen.init();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderAVT.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderAVT.glsl");
	ShaderProgram sp(vs, fs);

	// Uniform buffer object binding point
	const float uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp.getUniformBlockIndex("SharedMatrices");
	sp.bindUniformBlock(sharedMatricesIndex, uboBp);

	// Obtaining the model uniform location
	GLint modelUniform = sp.getUniformLocation("model");

	// Initial camera positions
	Vec3 cameraPos(0.0f, 0.0f, 5.0f);
	Vec3 cameraFront(0.0f, 0.0f, -1.0f);
	Vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Initializing the camera controller
	FreeCameraController cameraController(2.5f, cameraPos, cameraFront, cameraUp, -90.0f, 0.0f, win);

	// Initializing the camera and adding the controller
	Camera camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), ortho(-2, 2, -2, 2, 1, 100), uboBp);
	camera.addCameraController(cameraController);

	sp.use();

	Mat4 transformationBlue = Mat4::rotation(float((2 * M_PI) / 3), Vec3::Z);
	Mat4 transformationGreen = Mat4::rotation(float(-(2 * M_PI) / 3), Vec3::Z);

	Vec4 translationBlue = semiTriangleRed.vertices[2] - (transformationBlue * semiTriangleRed.vertices[0]);
	Vec4 translationGreen = semiTriangleRed.vertices[11] - (transformationGreen * semiTriangleRed.vertices[5]);

	float translationOffsetX = 0.006f;

	transformationBlue = Mat4::translation(translationBlue.x, translationBlue.y, translationBlue.z) * transformationBlue;
	transformationGreen = Mat4::translation(translationGreen.x - translationOffsetX, translationGreen.y, translationGreen.z) * transformationGreen;

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Processing input (P key, Space key and ESC key)
		processInput(win, sp, camera, cameraController);

		// Updating the camera position according to the keyboard input and mouse input
		cameraController.processInputAndMove(float(elapsed_time));
		// Finally updating the view matrix and projection matrices with their new values
		camera.update();

		drawAVT(modelUniform, sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkForOpenGLErrors("ERROR: MAIN/RUN");
#endif
	}

	sp.stopUsing();
	glfwDestroyWindow(win);
	glfwTerminate();

}

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

	Vec4 cyan2(40.8f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
	Vec4 purple2(121.12f / 255.0f, 40.8f / 255.0f, 204.0f / 255.0f, 1.0f);
	Vec4 red2(204.0f/255.0f, 40.8f/255.0f, 40.8f/255.0f, 1.0f);
	Vec4 orange2(204.0f/255.0f, 121.12f/255.0f, 40.8f/255.0f, 1.0f);

	drawLineTetromino(sp, squareBack, colorUniform, modelUniform, transformationLine,cyan2);
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
	const float uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp.getUniformBlockIndex("SharedMatrices");
	sp.bindUniformBlock(sharedMatricesIndex, uboBp);

	// Obtaining the model uniform location
	GLint modelUniform = sp.getUniformLocation("model");

	// Initial camera positions
	Vec3 cameraPos(0.0f, 0.0f, 5.0f);
	Vec3 cameraFront(0.0f, 0.0f, -1.0f);
	Vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Initializing the camera controller
	FreeCameraController cameraController(2.5f, cameraPos, cameraFront, cameraUp, -90.0f, 0.0f, win);

	// Initializing the camera and adding the controller
	Camera camera(lookAt(cameraPos, cameraPos + cameraFront, cameraUp), ortho(-2, 2, -2, 2, 1, 100), uboBp);
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
		
		// Finally updating the view matrix and projection matrices with their new values
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
	runAVT(win);
	//runCGJ(win);
	//runTest(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
