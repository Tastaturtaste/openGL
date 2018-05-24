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
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"


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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			 100.0f, 100.0f, 0.0f, 0.0f,
			 200.0f, 100.0f, 1.0f, 0.0f,
			 200.0f, 200.0f, 1.0f, 1.0f,
			 100.0f, 200.0f, 0.0f, 1.0f
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

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

		Renderer renderer;
		VertexArray vao;
		VertexBuffer vbo(vertex, sizeof(vertex));
		VertexBufferLayout layout;
		layout.Push<float>(2); // Vertex position
		layout.Push<float>(2); // Texture position
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(indices, 6);
		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]);
		shader.SetUniform1i("u_Texture", 0);

		float rInc = 0.03f;
		float gInc = 0.05f;
		float bInc = 0.07f;

		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();
		shader.Unbind();

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

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

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * view * model;

			shader.SetUniform4f("u_Color", color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]);
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(vao, ibo, shader);

			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	
	return 0;
}