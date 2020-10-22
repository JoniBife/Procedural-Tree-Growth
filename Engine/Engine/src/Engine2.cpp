///////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

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
	//destroyShaderProgram();
	//destroyBufferObjects();
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
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

void drawAVT(float uniformLocation, ShaderProgram& sp, Shape& semiTriangleRed, Shape& semiTriangleBlue, Shape& semiTriangleGreen, Mat4& transformationBlue, Mat4& transformationGreen) {

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
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
	};
	std::vector<Vec4> colorsBlue = {
		ColorRGBA::BLUE,
		ColorRGBA::BLUE,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
	};
	std::vector<Vec4> colorsGreen = {
		ColorRGBA::GREEN,
		ColorRGBA::GREEN,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
		ColorRGBA::BLACK,
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

	Mat4 transformationBlue = Mat4::rotation((2 * M_PI / 3), Vec3::Z);
	Mat4 transformationGreen = Mat4::rotation(-(2 * M_PI / 3), Vec3::Z);

	Vec4 translationBlue = semiTriangleRed.vertices[3] - (transformationBlue * semiTriangleRed.vertices[0]);
	Vec4 translationGreen = semiTriangleRed.vertices[4] - (transformationGreen * semiTriangleRed.vertices[1]);

	transformationBlue = Mat4::translation(translationBlue.x, translationBlue.y, translationBlue.z) * transformationBlue;
	transformationGreen = Mat4::translation(translationGreen.x, translationGreen.y, translationGreen.z) * transformationGreen;

	const float uniformLocation = sp.getUniformLocation("Matrix");

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display(win, elapsed_time, sp, triangle);
		drawAVT(uniformLocation,sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkForOpenGLErrors("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// RUN CGJ

ShapeGroup createLineTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(0.0f, 1.5f * width + 1.5f * offset, 0.0f));

	Vec4 color = ColorRGBA::CYAN;
	square1.paint(color);
	
	Shape square2 = square1;
	square2.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}
ShapeGroup createSquareTetromino(float width, float offset) {

	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(-(0.5f * width + 0.5f * offset), 0.5f * width + 0.5f * offset, 0.0f));

	Vec4 color = ColorRGBA::YELLOW;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation((width + offset), 0.0f, 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(-(width + offset), 0.0f, 0.0f));

	ShapeGroup squareTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return squareTetromino;
}

ShapeGroup createLTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(-(0.5*width + 0.5*offset), width + offset, 0.0f));

	Vec4 color = ColorRGBA::ORANGE;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(width + offset, 0.0f, 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}
ShapeGroup createReverseLTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(0.5 * width + 0.5 * offset, width + offset, 0.0f));

	Vec4 color = ColorRGBA::BLUE;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(-(width + offset), 0.0f, 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}
ShapeGroup createTTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(0.0f, -(0.5f * width + 0.5f * offset), 0.0f));

	Vec4 color = ColorRGBA::PURPLE;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation(-(width + offset), (width + offset), 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(width + offset, 0.0f, 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(width + offset, 0.0f, 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}
ShapeGroup createSTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(width+offset, 0.5f * width + 0.5f * offset, 0.0f));

	Vec4 color = ColorRGBA::GREEN;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation(-(width + offset), 0.0f, 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(-(width + offset), 0.0f, 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}
ShapeGroup createReverseSTetromino(float width, float offset) {
	Shape square1 = Shape::square(width);
	square1.transform(Mat4::translation(-(width + offset), 0.5f * width + 0.5f * offset, 0.0f));

	Vec4 color = ColorRGBA::RED;
	square1.paint(color);

	Shape square2 = square1;
	square2.transform(Mat4::translation(width + offset, 0.0f, 0.0f));

	Shape square3 = square2;
	square3.transform(Mat4::translation(0.0f, -(width + offset), 0.0f));

	Shape square4 = square3;
	square4.transform(Mat4::translation(width + offset, 0.0f, 0.0f));

	ShapeGroup lineTetromino({
			square1,
			square2,
			square3,
			square4
		});

	return lineTetromino;
}

void drawCGJ(float uniformLocation, ShaderProgram& sp, ShapeGroup& sg) {

	sp.use();

	sp.setUniform(uniformLocation, Mat4::rotation(M_PI_4,Vec3::Z));

	sg.bind();
	sg.draw();
	sg.unBind();

	sp.stopUsing();

}

void runCGJ(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader.glsl");
	ShaderProgram sp(vs, fs);

	const float uniformLocation = sp.getUniformLocation("Matrix");

	float offset = 0.025f;
	float width = 0.25f;

	ShapeGroup squareTetromino = createSquareTetromino(width, offset);
	squareTetromino.init();

	ShapeGroup lineTetromino = createLineTetromino(width, offset);
	lineTetromino.init();

	ShapeGroup LTetromino = createLTetromino(width, offset);
	LTetromino.init();

	ShapeGroup reverseLTetromino = createSquareTetromino(width, offset);
	reverseLTetromino.init();
	
	ShapeGroup square({
		lineTetromino,
		squareTetromino,
		LTetromino,
		reverseLTetromino
		});
	square.init();
	
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display(win, elapsed_time, sp, triangle);
		drawCGJ(uniformLocation, sp, squareTetromino);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkForOpenGLErrors("ERROR: MAIN/RUN");
#endif
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
		640, 640, "Hello Modern 2D World", is_fullscreen, is_vsync);
	//runAVT(win);
	runCGJ(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END