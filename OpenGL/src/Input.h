#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include <utility>


class Input
{
private:
	std::pair<double, double> mouse_pos;
	std::array<int, GLFW_KEY_LAST> keys;
	std::array<int, GLFW_MOUSE_BUTTON_LAST> buttons;

	friend class Window;

public:
	bool isKeyPressed(int key);
	bool isButtonPressed(int button);
	std::pair<double, double> getMousePosition();
	

private:
	Input();
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

public:
	~Input();
};

