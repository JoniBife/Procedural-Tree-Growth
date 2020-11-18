#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine {

private:
	// Only derived classed have access to this members
	GLFWwindow* window;
	int windowWidth;
	int windowHeight;
	double elapsedTime;

	/* Sets up everything related to glfw */
	void setupGLFW();

	/* Sets up everything related to glew */
	void setupGLEW();

	/* Sets up everything related to glfw */
	void setupOpenGL();

	/**
	* Called before main loop
	*/
	virtual void start() = 0;

	/*
	* Called every frameMesh
	*/
	virtual void update() = 0;

	/*
	* Called after main loop
	* Should be used to free any memory allocated in start or update
	*/
	virtual void end() = 0;

protected:
	GLFWwindow* getWindow();
	int getWindowWidth();
	int getWindowHeight();
	double getElapsedTime();

public:
	void run();
};


#endif
