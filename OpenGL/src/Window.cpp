#include "Window.h"
#include <iostream>
#include "ErrorHandler.h"

Window::Window(int width, int heigth, const char* title)
{
	m_Width = width;
	m_Height = heigth;
	m_Title = title;

	if (!init())
	{
		GLAssertError(glfwTerminate());
	}
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

void Window::update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

bool Window::init()
{
	if (!glfwInit())
	{
		std::cerr << "Initalisation of GLFW failed!" << std::endl;
		return false;
	}
	
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if (!m_Window)
	{
		std::cerr << "Creation of GLFW Window failed!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Initialisation of GLEW failed!" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetWindowSizeCallback(m_Window, window_resize_callback);
	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSwapInterval(1);

	return true;
}

void window_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Width = width;
	win->m_Height = height;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	((Window*)glfwGetWindowUserPointer(window))->m_Input.keys[key] = (action != GLFW_RELEASE);
	if(action != GLFW_RELEASE)
		std::cout << glfwGetKeyName(key, scancode) << std::endl;
}

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	((Window*)glfwGetWindowUserPointer(window))->m_Input.mouse_pos.first = xpos;
	((Window*)glfwGetWindowUserPointer(window))->m_Input.mouse_pos.second = ypos;
	std::cout << xpos << ", " << ypos << std::endl;
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	((Window*)glfwGetWindowUserPointer(window))->m_Input.buttons[button] = (action != GLFW_RELEASE);
	if (action != GLFW_RELEASE)
		std::cout << "Click!" << std::endl;
}