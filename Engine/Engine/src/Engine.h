#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene/SceneGraph.h"

class Engine {

private:
	// Only derived classed have access to this members
	GLFWwindow* window = nullptr;
	SceneGraph* sceneGraph = nullptr;
	Camera* camera = nullptr;
	int windowWidth = 0;
	int windowHeight = 0;
	double elapsedTime = 0.0;

	/* Sets up everything related to glfw */
	void setupGLFW();

	/* Sets up everything related to glew */
	void setupGLEW();

	/* Sets up everything related to glfw */
	void setupOpenGL();

	/* Sets up the camera and sceneGraph */
	void setupScene();

	/* Frees any memory allocated on the heap */
	void freeResources();

	/**
	* Called before main loop
	*/
	virtual void start() {/*EMPTY*/ }

	/*
	* Called every frameMesh
	*/
	virtual void update() {/*EMPTY*/ }

	/*
	* Called after main loop
	* Should be used to free any memory allocated in start or update
	*/
	virtual void end() {/*EMPTY*/ }

protected:
	GLFWwindow* getWindow(); 
	SceneGraph* getSceneGraph();
	Camera* getCamera();
	int getWindowWidth();
	int getWindowHeight();
	double getElapsedTime();

public:
	void run();
};


#endif
