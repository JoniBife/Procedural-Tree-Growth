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

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch (error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 Matrix;\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = Matrix * in_Position;\n"
	"	ex_Color = in_Color;\n"
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 ex_Color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = ex_Color;\n"
	"}\n"
};

void createShaderProgram()
{
	/*VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");

	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);*/

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create shaders.");
#endif
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

const Vec4 avtVertices[] = {
	{ 0.01f, 0.58f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f},
	{ 0.01f, 0.34f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f},
	{ -0.605f, -0.48f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f, 1.0f},
	{ -0.54f, -0.605f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f, 1.0f},
	{ 0.56f, -0.37f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f, 1.0f},
	{ 0.42f, -0.37f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f, 1.0f},
};

const GLubyte avtIndices[] = { 
	0,2,3, // left-most triangle
	0,3,1,
	0,1,5,
	0,5,4, // right-most triangle
};

void createBufferObjectsAvt()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(avtVertices), avtVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4) * 2, 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4) * 2, (GLvoid*)sizeof(avtVertices[1]));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(avtIndices), avtIndices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene(ShaderProgram& sp, Shape &shape)
{
	// Drawing directly in clip space
	/*glBindVertexArray(VaoId);
	sp.use();

	sp.setUniform(UniformId, Mat4::IDENTITY);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);

	Vec4 dir1 = avtVertices[0] - avtVertices[8];
	Vec4 dir2 = avtVertices[4]- avtVertices[0];
	//Vec4 dir1 = avtVertices[0] - avtVertices[4];
	//Vec4 dir2 = avtVertices[8] - avtVertices[0];
	dir1.w = 0;
	dir2.w = 0;

	float cosine = dot(dir1, dir2) / (dir1.magnitude() * dir2.magnitude());
	float angleRad = acosf(cosine);

	Mat4 T1 = Mat4::rotation(-(2*M_PI/3), Vec3::Z);
	Mat4 T2 = Mat4::rotation((2*M_PI/3), Vec3::Z);

	Vec4 translationGreen = avtVertices[8] - (T1 * avtVertices[2]);
	Vec4 translationBlue = avtVertices[6] - (T2* avtVertices[0]);

	T1 = Mat4::translation(translationGreen.x, translationGreen.y, 0.0f) * T1;
	T2 = Mat4::translation(translationBlue.x, translationBlue.y, 0.0f) * T2;

	sp.setUniform(UniformId, T1);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);

	sp.setUniform(UniformId, T2);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);

	//glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	sp.stopUsing();
	glBindVertexArray(0);*/

	shape.bind();
	sp.use();

	sp.setUniform(UniformId, Mat4::IDENTITY);
	shape.draw();

	sp.stopUsing();
	shape.unBind();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	destroyShaderProgram();
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

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec, ShaderProgram &sp, Shape& shape)
{
	
	//glBindAttribLocation(sp.getProgramId(), VERTICES, "in_Position");
	//glBindAttribLocation(sp.getProgramId(), COLORS, "in_Color");

	drawScene(sp, shape);
}

void setupAVT() {
	/*std::vector<Vec4> vertices = {
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

	Shape semiTriangleRed(vertices, colorsRed);
	semiTriangleRed.init();

	Shape semiTriangleBlue = semiTriangleRed;
	semiTriangleBlue.colors = colorsBlue;
	semiTriangleBlue.init();

	Shape semiTriangleGreen = semiTriangleRed;
	semiTriangleBlue.colors = colorsGreen;
	semiTriangleGreen.init();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader.glsl");
	ShaderProgram sp(vs, fs);

	Mat4 transformationBlue = Mat4::rotation(-(2 * M_PI / 3), Vec3::Z);
	Mat4 transformationGreen = Mat4::rotation((2 * M_PI / 3), Vec3::Z);

	Vec4 translationBlue = semiTriangleRed.vertices[6] - (transformationBlue * semiTriangleRed.vertices[0]);
	Vec4 translationGreen = semiTriangleRed.vertices[8] - (transformationGreen * semiTriangleRed.vertices[2]);
	

	transformationBlue = Mat4::translation(translationBlue.x, translationBlue.y, translationBlue.z) * transformationBlue;
	transformationGreen = Mat4::translation(translationGreen.x, translationGreen.y, translationGreen.z) * transformationGreen;

	drawAVT(sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen)`;*/
}

void drawAVT(ShaderProgram& sp, Shape& semiTriangleRed, Shape& semiTriangleGreen, Shape& semiTriangleBlue, Mat4& transformationBlue, Mat4& transformationGreen) {
	sp.use();

	float uniformLocation = sp.getUniformLocation("Matrix");

	// Red triangle
	semiTriangleRed.bind();
	sp.setUniform(uniformLocation, Mat4::IDENTITY);
	semiTriangleRed.draw();
	semiTriangleRed.unBind();

	// Blue triangle
	semiTriangleBlue.bind();
	sp.setUniform(uniformLocation, transformationBlue);
	semiTriangleBlue.draw();
	semiTriangleBlue.unBind();

	// Green 
	semiTriangleGreen.bind();
	sp.setUniform(uniformLocation, transformationGreen);
	semiTriangleGreen.draw();
	semiTriangleGreen.unBind();

	sp.stopUsing();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	/*Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader.glsl");
	ShaderProgram sp(vs, fs);

	Shape triangle = Shape::square();
	triangle.init();

	/*Shape square = Shape::square();
	square.init();*/

	/*UniformId = sp.getUniformLocation("Matrix");*/

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

	Shape semiTriangleRed(vertices, colorsRed);
	semiTriangleRed.init();

	Shape semiTriangleBlue = semiTriangleRed;
	semiTriangleBlue.colors = colorsBlue;
	semiTriangleBlue.init();

	Shape semiTriangleGreen = semiTriangleRed;
	semiTriangleGreen.colors = colorsGreen;
	semiTriangleGreen.init();

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader.glsl");
	ShaderProgram sp(vs, fs);

	Mat4 transformationBlue = Mat4::rotation(-(2 * M_PI / 3), Vec3::Z);
	Mat4 transformationGreen = Mat4::rotation((2 * M_PI / 3), Vec3::Z);

	Vec4 translationBlue = semiTriangleRed.vertices[3] - (transformationBlue * semiTriangleRed.vertices[0]);
	Vec4 translationGreen = semiTriangleRed.vertices[4] - (transformationGreen * semiTriangleRed.vertices[1]);


	transformationBlue = Mat4::translation(translationBlue.x, translationBlue.y, translationBlue.z) * transformationBlue;
	transformationGreen = Mat4::translation(translationGreen.x, translationGreen.y, translationGreen.z) * transformationGreen;

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display(win, elapsed_time, sp, triangle);
		drawAVT(sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: MAIN/RUN");
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
		640, 480, "Hello Modern 2D World", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END