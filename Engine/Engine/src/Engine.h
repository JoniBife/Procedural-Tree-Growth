#ifndef ENGINE_H
#define ENGINE_H

// TODO Create Engine.cpp that implements all this functions

// Sets up everything related to glew
void setupGlew();

// Sets up everything related to glfw
void setupGLFW();

// Sets up everything related to glfw
void setupOpenGL();

// Sets up everything related to the current scene (shaders, vbos etc...)
void setup();

// Starts the application
void run();

/*
* Called every frame
* 
* @param elapsedSec is elapsed time from last frame
*/
void update(double elapsedSec);

#endif
