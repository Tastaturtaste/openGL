#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


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
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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

		float vertex[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,	0.5f, 1.0f, 1.0f,
			-0.5f,	0.5f, 0.0f, 1.0f
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

		//GLAssertError(glEnable(GL_BLEND));
		//GLAssertError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f);

		Renderer renderer;
		VertexArray vao;
		VertexBuffer vbo(vertex, sizeof(vertex));
		VertexBufferLayout layout;
		layout.Push<float>(2); // Vertex position
		layout.Push<float>(2); // Texture position
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(indices, 6);
		Texture texture("res/textures/Fallschirmsprung28.06.16.png");
		texture.Bind();

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj);
		shader.SetUniform4f("u_Color", color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]);
		shader.SetUniform1i("u_Texture", 0);

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