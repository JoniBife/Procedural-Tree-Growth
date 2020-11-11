#ifndef ENGINE_H
#define ENGINE_H

class Engine {

	// Sets up everything related to glew
	void setupGlew();

	// Sets up everything related to glfw
	void setupGLFW();

	// Sets up everything related to glfw
	void setupOpenGL();

	/**
	* Called before main loop
	*/
	virtual void start() = 0;

	/*
	* Called every frame
	*
	* @param elapsedSec is elapsed time from last frame
	*/
	virtual void update(double elapsedTime) = 0;
};


#endif
