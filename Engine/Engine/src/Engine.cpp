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
////////////////////////////////////////////////////////////////////////// RUN AVT

void drawAVT(GLint uniformLocation, ShaderProgram& sp, Shape& semiTriangleRed, Shape& semiTriangleBlue, Shape& semiTriangleGreen, Mat4& transformationBlue, Mat4& transformationGreen) {

	sp.use();

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
	sp.stopUsing();

}

void runAVT(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	std::vector<Vec4> vertices = {
		{ 0.01f, 0.58f, 0.0f, 1.0f },
		{ 0.01f, 0.34f, 0.0f, 1.0f },
		{ -0.605f, -0.48f, 0.0f, 1.0f },
		{ -0.54f, -0.605f, 0.0f, 1.0f },
		{ 0.56f, -0.37f, 0.0f, 1.0f },
		{ 0.42f, -0.37f, 0.0f, 1.0f }
	};
	std::vector<Vec4> colorsRed = {
		ColorRGBA::RED,
		ColorRGBA::RED,
		{ 0.3f, 0.0f, 0.0f, 1.0f },
		{ 0.3f, 0.0f, 0.0f, 1.0f },
		{ 0.3f, 0.0f, 0.0f, 1.0f },
		{ 0.3f, 0.0f, 0.0f, 1.0f }
	};
	std::vector<Vec4> colorsBlue = {
		ColorRGBA::BLUE,
		ColorRGBA::BLUE,
		{ 0.0f, 0.0f, 0.3f, 1.0f },
		{ 0.0f, 0.0f, 0.3f, 1.0f },
		{ 0.0f, 0.0f, 0.3f, 1.0f },
		{ 0.0f, 0.0f, 0.3f, 1.0f }
	};
	std::vector<Vec4> colorsGreen = {
		ColorRGBA::GREEN,
		ColorRGBA::GREEN,
		{ 0.0f, 0.3f, 0.0f, 1.0f },
		{ 0.0f, 0.3f, 0.0f, 1.0f },
		{ 0.0f, 0.3f, 0.0f, 1.0f },
		{ 0.0f, 0.3f, 0.0f, 1.0f },
	};
	std::vector<GLubyte> indices = {
		0,2,3, // left-most triangle
		0,3,1,
		0,1,5,
		0,5,4, // right-most triangle
	};

	Shape semiTriangleRed(vertices, colorsRed, indices);

	Shape semiTriangleBlue = semiTriangleRed;
	semiTriangleBlue.colors = colorsBlue;

	Shape semiTriangleGreen = semiTriangleRed;
	semiTriangleGreen.colors = colorsGreen;

	semiTriangleRed.init();
	semiTriangleBlue.init();
	semiTriangleGreen.init();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader.glsl");
	ShaderProgram sp(vs, fs);

	Vec4 dir1 = semiTriangleRed.vertices[0] - semiTriangleRed.vertices[4];
	Vec4 dir2 = semiTriangleRed.vertices[2] - semiTriangleRed.vertices[0];
	//Vec4 dir1 = avtVertices[0] - avtVertices[4];
	//Vec4 dir2 = avtVertices[8] - avtVertices[0];
	dir1.w = 0;
	dir2.w = 0;
	float cosine = dot(dir1, dir2) / (dir1.magnitude() * dir2.magnitude());
	float angleRad = acosf(cosine);

	//Mat4 transformationBlue = Mat4::rotation(angleRad, Vec3::Z);
	//Mat4 transformationGreen = Mat4::rotation(-angleRad, Vec3::Z);
	Mat4 transformationBlue = Mat4::rotation(float((2* M_PI)/3), Vec3::Z);
	Mat4 transformationGreen = Mat4::rotation(float(-(2 * M_PI)/3), Vec3::Z);

	Vec4 translationBlue = semiTriangleRed.vertices[3] - (transformationBlue * semiTriangleRed.vertices[0]);
	Vec4 translationGreen = semiTriangleRed.vertices[4] - (transformationGreen * semiTriangleRed.vertices[1]);

	float translationOffsetX = 0.006f;

	transformationBlue = Mat4::translation(translationBlue.x, translationBlue.y, translationBlue.z) * transformationBlue;
	transformationGreen = Mat4::translation(translationGreen.x - translationOffsetX, translationGreen.y, translationGreen.z) * transformationGreen;

	const GLint uniformLocation = sp.getUniformLocation("Matrix");

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display(win, elapsed_time, sp, triangle);
		drawAVT(uniformLocation, sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkForOpenGLErrors("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// CGJ
const float offset = 0.02f;
const float width = 0.3f;

void drawLineTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	
	sp.setUniform(colorUniform, ColorRGBA::CYAN);

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

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset),  0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(-0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset), -0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset),  0.5f * (width + offset), 0.0f));
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

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset), -(width + offset), 0.0f));
	square.draw();
}
void drawReverseLTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::BLUE);

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset), width + offset, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset), 0.0f, 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation( 0.5f * (width + offset), -(width + offset), 0.0f));
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

	sp.setUniform(modelUniform, transform * Mat4::translation( 1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
	square.draw();

	sp.setUniform(modelUniform, transform * Mat4::translation(0.0f, -0.5f * (width + offset), 0.0f));
	square.draw();
}
void drawSTetromino(ShaderProgram& sp, Shape& square, GLint colorUniform, GLint modelUniform, Mat4 transform) {
	sp.setUniform(colorUniform, ColorRGBA::GREEN);

	sp.setUniform(modelUniform, transform * Mat4::translation( 1.0f * (width + offset), 0.5f * (width + offset), 0.0f));
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

const float angularSpeed = float(M_PI / 4);
float orientation = float(M_PI / 4.0);

Mat4 linePos = Mat4::translation(1.5f * (width + offset), 0.0f, 0.0f);

Mat4 LPos = Mat4::translation(-0.5f * (width + offset), width + offset, 0.0f);
Mat4 LOrientation = Mat4::rotation(float(-M_PI_2), { 0, 0, 1 });

Mat4 reverseLPos = Mat4::translation(-0.5f * (width + offset), -(width + offset), 0.0f);

void drawCGJ(GLint colorUniform, GLint modelUniform, ShaderProgram& sp, Shape& square, float elapsed_time) {

	sp.use();

	orientation += angularSpeed*elapsed_time;
	Mat4 rotation = Mat4::rotation(orientation, Vec3::Z); //to rotate the group of tetrominos
	
	square.bind();

	drawLineTetromino(sp, square, colorUniform, modelUniform, rotation * linePos);
	drawSquareTetromino(sp, square, colorUniform, modelUniform, rotation);
	drawLTetromino(sp, square, colorUniform, modelUniform, rotation * LPos * LOrientation);
	drawReverseLTetromino(sp, square, colorUniform, modelUniform, rotation * reverseLPos * LOrientation);

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
		//display(win, elapsed_time, sp, triangle);
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
	runAVT(win);
	//runCGJ(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
