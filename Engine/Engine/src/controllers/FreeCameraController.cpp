#include "FreeCameraController.h"
#include "../view/Transformations.h"
#include "../Configurations.h"
#include "../math/MathAux.h"


FreeCameraController::FreeCameraController(const float movementSpeed, const Vec3& position, const Vec3& front, const Vec3& up, const float yawRad, const float pitchRad, GLFWwindow* win, const std::function<void(Mat4&)>& onMovement)
	: movementSpeed(movementSpeed), position(position), front(front), up(up), yawRad(yawRad - 90), pitchRad(pitchRad), win(win), onMovement(onMovement) {

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	lastXpos = SCREEN_WIDTH / 2;
	lastYpos = SCREEN_HEIGHT / 2;

	glfwSetCursorPos(win, lastXpos, lastYpos);
}

/*
* Receives the user input and updates the view matrix accordingly
*/
void FreeCameraController::processInputAndMove(float elapsedTime) {
	bool moved = processMouseInput();
	moved = processKeyboardInput(elapsedTime) || moved;
	if (moved) {
		Mat4 view = lookAt(position, position + front, up);
		onMovement(view);
	}
}


/*
* Checks for keyboard input, if there is input return true
* else returns false
*/
bool FreeCameraController::processKeyboardInput(float elapsedTime) {

	bool moved = false;

	// Converting from units per second to units per frame
	float ms = movementSpeed * elapsedTime;

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		position += (ms) * front;
		moved = true;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= (ms) * front;
		moved = true;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= cross(front, up).normalize() * (ms);
		moved = true;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += cross(front, up).normalize() * (ms);
		moved = true;
	}		
	
	return moved;
}

bool firstMove = true;


/*
* Checks for mouse input, if there is input return true
* else returns false
*/
bool FreeCameraController::processMouseInput() {

	// Getting mouse position
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	/*if (firstMove) {
		firstMove = false;
		lastXpos = xpos;
		lastYpos = ypos;
	}*/

	std::cout << "X: " << xpos << " Y: " << ypos << std::endl;

	double mouseSensitivity = 0.05;

	double xOffset = xpos - lastXpos;
	double yOffset = lastYpos - ypos; // Y axis is inverted (increases downward)
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	lastXpos = xpos;
	lastYpos = ypos;

	std::cout << "Xoffset: " << xOffset << " Yoffset: " << yOffset << std::endl;

	if (xOffset == 0.0 && yOffset == 0.0) {
		return false;
	}

	yawRad += xOffset;
	pitchRad += yOffset;

	if (pitchRad > 89.0f)
		pitchRad = 89.0f;
	if (pitchRad < -89.0f)
		pitchRad = -89.0f;

	Vec3 direction;
	direction.x = cosf(degreesToRadians(yawRad)) * cosf(degreesToRadians(pitchRad));
	direction.y = sinf(degreesToRadians(pitchRad));
	direction.z = sinf(degreesToRadians(yawRad)) * cosf(degreesToRadians(pitchRad));
	front = direction.normalize();

	std::cout << direction << std::endl;

	return true;
}