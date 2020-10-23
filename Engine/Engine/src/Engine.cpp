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
////////////////////////////////////////////////////////////////////////// CGJ
const float offset = 0.02f;
const float width = 0.3f;

void drawLineTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {

	sp.setUniform(colorUniform, ColorRGBA::CYAN);

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
void drawSquareTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::YELLOW);

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawLTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::ORANGE);

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), 0.0f, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();
}
void drawReverseLTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::BLUE);

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), width + offset, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), 0.0f, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();
}
void drawTTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::PURPLE);

	sp.setUniform(modelUniform, transform * Mat4::translation(-1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawSTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::GREEN);

	sp.setUniform(modelUniform, transform * Mat4::translation(1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-1.0f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawReverseSTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::RED);

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

Mat4 transformationLine = rotation * Mat4::translation(1.5f * (width + offset), 0.0f, 0.0f);
Mat4 transformationSquare = rotation;
Mat4 transformationL = rotation * Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f) * Mat4::rotation(float(-M_PI_2), { 0, 0, 1 });
Mat4 transformationReverseL = rotation * Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f) * Mat4::rotation(float(-M_PI_2), { 0, 0, 1 });

void drawCGJ(GLint colorUniform, GLint modelUniform, ShaderProgram& sp, Shape& square, float elapsed_time) {

	sp.use();

	square.bind();

	drawLineTetromino(sp, square, colorUniform, modelUniform, transformationLine);
	drawSquareTetromino(sp, square, colorUniform, modelUniform, transformationSquare);
	drawLTetromino(sp, square, colorUniform, modelUniform, transformationL);
	drawReverseLTetromino(sp, square, colorUniform, modelUniform, transformationReverseL);

	square.unBind();

	sp.stopUsing();
}

void runCGJ(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderCGJ.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderCGJ.glsl");
	ShaderProgram sp(vs, fs);

	GLint modelUniform = sp.getUniformLocation("Matrix");
	GLint colorUniform = sp.getUniformLocation("Color");

	Shape square = Shape::square(width);
	square.init();

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		drawCGJ(colorUniform, modelUniform, sp, square, float(elapsed_time));
		glfwSwapBuffers(win);
		glfwPollEvents();
		checkForOpenGLErrors("ERROR: MAIN/RUN");
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 0;
	int is_fullscreen = 0;
	int is_vsync = 1;
	GLFWwindow* win = setup(gl_major, gl_minor,
		640, 640, "Engine", is_fullscreen, is_vsync);
	runCGJ(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
