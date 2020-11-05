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
#include "math/Qtrn.h"



//////////////////////////////////////////////////////////////////// INITIAL CAMERA SETUP

Mat4 orthographicProj = ortho(-2, 2, -2, 2, 0, 100);
Mat4 perspectiveProj = perspective(M_PI / 2, SCREEN_WIDTH / SCREEN_HEIGHT, 0, 100);

Mat4 currProjection = orthographicProj;

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
void processInput(GLFWwindow* window, ShaderProgram& sp, Camera& camera, FreeCameraController &cameraController)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraController.snapToPosition(cameraPos, cameraFront, -90.0f, 0.0f);
		
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	static Vec2 lastPos;
	static bool isPressed = false;
	static Qtrn rot(1,0,0,0);
	static Vec3 rot2(0, 0, 0);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (state == GLFW_PRESS)
	{
		if (isPressed) {

			Vec2 currPos((float)xpos, (float)ypos);

			// Direction of mouse movement
			Vec2 dir = (currPos - lastPos);

			if (dir != Vec2(0, 0)) {

				float sensitivity = 0.01f;
				dir = dir * sensitivity;
#define QUAT
#ifdef QUAT
				rot = (Qtrn(dir.x, Vec3::Y) * Qtrn(dir.y,Vec3::X)).normalize() * rot;
				camera.setView(Mat4::translation(0.0f, 0.0f, -10.0f) * rot.toRotationMatrix());
#else
				rot2.x += dir.x;
				rot2.y += dir.y;

				camera.setView(Mat4::translation(0.0f, 0.0f, -10.0f) * Mat4::rotation(rot2.y, Vec3::X) * (Mat4::rotation(rot2.x, Vec3::Y)));
#endif

				
			}
		} else {
			isPressed = true;
		}
		lastPos = Vec2((float)xpos, (float)ypos);
	}
	else if (state == GLFW_RELEASE) {
		isPressed = false;
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

void drawSquare(GLint modelUniform, ShaderProgram& sp, Shape& square) {
	// Red triangle
	square.bind();
	sp.setUniform(modelUniform, Mat4::IDENTITY);
	square.draw();
	square.unBind();
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

	// Setting the inital z to be further away so that when we change from otho to persp there is a larger difference
	/*for (auto& vec : vertices) {
		vec.z = -3.0f;
	}*/

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

	const Vec4 BLUE2 = { 25.5f / 255.0f, 25.5f / 255.0f, 127.5f / 255.0f, 1.0f };
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

	/*const std::vector<Vec4> Vertices = // no indices?
	{
	{ 0.0f, 0.0f, 1.0f, 1.0f },// 0 - FRONT
	{ 1.0f, 0.0f, 1.0f, 1.0f },// 1
	{ 1.0f, 1.0f, 1.0f, 1.0f },// 2
	{ 1.0f, 1.0f, 1.0f, 1.0f },// 2	
	{ 0.0f, 1.0f, 1.0f, 1.0f },// 3
	{ 0.0f, 0.0f, 1.0f, 1.0f },// 0

	{ 1.0f, 0.0f, 1.0f, 1.0f },// 1 - RIGHT
	{ 1.0f, 0.0f, 0.0f, 1.0f },// 5
	{ 1.0f, 1.0f, 0.0f, 1.0f },// 6
	{ 1.0f, 1.0f, 0.0f, 1.0f },// 6	
	{ 1.0f, 1.0f, 1.0f, 1.0f },// 2
	{ 1.0f, 0.0f, 1.0f, 1.0f },// 1

	{ 1.0f, 1.0f, 1.0f, 1.0f },// 2 - TOP
	{ 1.0f, 1.0f, 0.0f, 1.0f },// 6
	{ 0.0f, 1.0f, 0.0f, 1.0f },// 7
	{ 0.0f, 1.0f, 0.0f, 1.0f },// 7	
	{ 0.0f, 1.0f, 1.0f, 1.0f },// 3
	{ 1.0f, 1.0f, 1.0f, 1.0f },// 2

	{ 1.0f, 0.0f, 0.0f, 1.0f },// 5 - BACK
	{ 0.0f, 0.0f, 0.0f, 1.0f },// 4
	{ 0.0f, 1.0f, 0.0f, 1.0f },// 7
	{ 0.0f, 1.0f, 0.0f, 1.0f },// 7	
	{ 1.0f, 1.0f, 0.0f, 1.0f },// 6
	{ 1.0f, 0.0f, 0.0f, 1.0f },// 5

	{ 0.0f, 0.0f, 0.0f, 1.0f },// 4 - LEFT
	{ 0.0f, 0.0f, 1.0f, 1.0f },// 0
	{ 0.0f, 1.0f, 1.0f, 1.0f },// 3
	{ 0.0f, 1.0f, 1.0f, 1.0f },// 3	
	{ 0.0f, 1.0f, 0.0f, 1.0f },// 7
	{ 0.0f, 0.0f, 0.0f, 1.0f },// 4

	{ 0.0f, 0.0f, 1.0f, 1.0f },// 0 - BOTTOM
	{ 0.0f, 0.0f, 0.0f, 1.0f },// 4
	{ 1.0f, 0.0f, 0.0f, 1.0f },// 5
	{ 1.0f, 0.0f, 0.0f, 1.0f },// 5	
	{ 1.0f, 0.0f, 1.0f, 1.0f },// 1
	{ 0.0f, 0.0f, 1.0f, 1.0f }// 0
};

	const std::vector<Vec4> Colors = {
{ 0.9f, 0.0f, 0.0f, 1.0f },
{ 0.9f, 0.0f, 0.0f, 1.0f },
{ 0.9f, 0.0f, 0.0f, 1.0f },
{ 0.9f, 0.0f, 0.0f, 1.0f },
{ 0.9f, 0.0f, 0.0f, 1.0f },
{ 0.9f, 0.0f, 0.0f, 1.0f },

{ 0.0f, 0.9f, 0.0f, 1.0f },
{ 0.0f, 0.9f, 0.0f, 1.0f },
{ 0.0f, 0.9f, 0.0f, 1.0f },
{ 0.0f, 0.9f, 0.0f, 1.0f },
{ 0.0f, 0.9f, 0.0f, 1.0f },
{ 0.0f, 0.9f, 0.0f, 1.0f },

{ 0.0f, 0.0f, 0.9f, 1.0f },
{ 0.0f, 0.0f, 0.9f, 1.0f },
{ 0.0f, 0.0f, 0.9f, 1.0f },
{ 0.0f, 0.0f, 0.9f, 1.0f },
{ 0.0f, 0.0f, 0.9f, 1.0f },
{ 0.0f, 0.0f, 0.9f, 1.0f },

{ 0.0f, 0.9f, 0.9f, 1.0f },
{ 0.0f, 0.9f, 0.9f, 1.0f },
{ 0.0f, 0.9f, 0.9f, 1.0f },
{ 0.0f, 0.9f, 0.9f, 1.0f },
{ 0.0f, 0.9f, 0.9f, 1.0f },
{ 0.0f, 0.9f, 0.9f, 1.0f },

{ 0.9f, 0.0f, 0.9f, 1.0f },
{ 0.9f, 0.0f, 0.9f, 1.0f },
{ 0.9f, 0.0f, 0.9f, 1.0f },
{ 0.9f, 0.0f, 0.9f, 1.0f },
{ 0.9f, 0.0f, 0.9f, 1.0f },
{ 0.9f, 0.0f, 0.9f, 1.0f },

{ 0.9f, 0.9f, 0.0f, 1.0f },
{ 0.9f, 0.9f, 0.0f, 1.0f },
{ 0.9f, 0.9f, 0.0f, 1.0f },
{ 0.9f, 0.9f, 0.0f, 1.0f },
{ 0.9f, 0.9f, 0.0f, 1.0f },
{ 0.9f, 0.9f, 0.0f, 1.0f }
	};

	Shape square(Vertices, Colors);
	square.transform(Mat4::translation(-0.5f, -0.5f, -0.5f));
	square.init();*/

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

	// Initializing the camera controller
	FreeCameraController cameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, win);

	//lookAt(cameraPos, cameraPos + cameraFront, cameraUp)
	// Initializing the camera and adding the controller
	Camera camera(Mat4::translation(0.0f,0.0f,-10.0f), currProjection, uboBp);
	//camera.addCameraController(cameraController);

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
		//cameraController.processInputAndMove(float(elapsed_time));

		// the projection might have been changed
		camera.setProjection(currProjection);
		// Finally updating the view matrix and projection matrices with their new values
		camera.update();

		drawAVT(modelUniform, sp, semiTriangleRed, semiTriangleBlue, semiTriangleGreen, transformationBlue, transformationGreen);
		//drawSquare(modelUniform, sp,square);
		glfwSwapBuffers(win);
		glfwPollEvents();
		checkForOpenGLErrors("ERROR: MAIN/RUN");
	}

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
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END
