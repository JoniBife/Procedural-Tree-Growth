#ifndef FREE_CAMERA_CONTROLLER_3D_H
#define FREE_CAMERA_CONTROLLER_3D_H

#include <functional>
#include "../math/Vec3.h"
#include "../shaders/ShaderProgram.h"
#include "GLFW/glfw3.h"

/*
* All the vertex coordinates as seen from the camera's perspective as the origin of the scene,
* the view matrix transforms all the world coordinates into view coordinates that are relative 
* to the camera's position and direction
* 
* Usage example:
* 
* 
* FreeCameraController cameraController(position
* 
*/
class FreeCameraController {

private:
	std::function<void(Mat4&)> onMovement;
	GLFWwindow* win;

public:
	float movementSpeed;
	Vec3 position; 
	Vec3 front; // The location where the camera is looking at (in front of the camera)
	Vec3 up; 
	float yaw;
	float pitch;
	double lastXpos;
	double lastYpos;

public: 
	FreeCameraController(const float movementSpeed, const Vec3& position, const Vec3& front, const Vec3& up, const float yaw, const float pitch, GLFWwindow* win);

	
	/*
	* Sets an OnMovement callback that will be called when the movement event occurs (when the user moves the camera)
	*/
	void setOnMovementListener(const std::function<void(Mat4&)>& onMovement);

	/*
	* Receives the user input and updates the view matrix accordingly
	*/
	void processInputAndMove(const float elapsedTime);

	void snapToPosition(const Vec3 position, const Vec3 front);

private:
	
	/*
	* Checks for keyboard input, if there is input return true
	* else returns false
	*/
	bool processKeyboardInput(const float elapsedTime);


	/*
	* Checks for mouse input, if there is input return true
	* else returns false
	*/
	bool processMouseInput();
};

#endif