#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "GLFW/glfw3.h"

#include <string>
#include <map>

class InputManager {

public:
	std::map<int, bool> keysPressed;
	bool receivingTextInput = false;
	std::string textInput;

	static InputManager* instance;

	InputManager(GLFWwindow* window);
	void startTextInput();
	void resumeTextInput(const std::string& oldInput);
	void stopTextInput();
	std::string getCurrTextInput() const;

	bool isKeyPressed(int key);

public:

	static void createInstance(GLFWwindow* window);

	static InputManager* getInstance();

	static void destroyInstance();

};

#endif
