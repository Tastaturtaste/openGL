#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"


enum class rgba
{
	RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3
};

int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	glfwSwapInterval(1);
	
	{

		float positions[] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,	0.5f,
			-0.5f,	0.5f
		};

		float color[4] =
		{
			1.0f, 1.0f, 1.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};


		Renderer renderer;
		VertexArray vao;
		VertexBuffer vbo(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]);

		float rInc = 0.03f;
		float gInc = 0.05f;
		float bInc = 0.07f;

		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();
		shader.Unbind();


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			vao.Bind();
			shader.Bind();

			if (color[(int)rgba::RED] >= 1 || color[(int)rgba::RED] <= 0)
				rInc = -rInc;
			if (color[(int)rgba::GREEN] >= 1 || color[(int)rgba::GREEN] <= 0)
				gInc = -rInc;
			if (color[(int)rgba::BLUE] >= 1 || color[(int)rgba::BLUE] <= 0)
				bInc = -bInc;

			color[(int)rgba::RED] += rInc;
			color[(int)rgba::GREEN] += gInc;
			color[(int)rgba::BLUE] += bInc;

			shader.SetUniform4f("u_Color", color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]);

			renderer.Draw(vao, ibo, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	
	return 0;
}