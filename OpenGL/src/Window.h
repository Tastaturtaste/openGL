#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "Input.h"

void window_resize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

class Window
{
private:
	int m_Width, m_Height;
	GLFWwindow* m_Window;
	const char* m_Title;
	bool m_Closed;
	Input m_Input;
	friend void window_resize_callback(GLFWwindow* window, int width, int height);
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

public:
	Window(int width, int heigth, const char* title);
	~Window();
	void update();
	inline bool closed() { return (bool)glfwWindowShouldClose(m_Window); }
	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }

	inline GLFWwindow* getGLFWwindow() const { return m_Window; }

private:
	bool init();

};