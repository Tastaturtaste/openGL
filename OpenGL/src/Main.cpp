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

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum class rgba
{
	RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3
};

static ShaderSource GetShaderSource(const std::string& filepath) 
{
	std::ifstream stream(filepath);
	std::stringstream ss[2];
	std::string line;
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) 
	{
		if (line.find("#shader") != std::string::npos) 
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
			continue;
		}

		if (type == ShaderType::NONE)
			continue;
		ss[(int)type] << line << "\n";
	}
	return { ss[0].str(), ss[1].str() };
}

ShaderSource shaderSource = GetShaderSource("res/shader/Basic.shader");

static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
	GLAssertError(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLAssertError(glShaderSource(id, 1, &src, nullptr));
	GLAssertError(glCompileShader(id));
	
	int result;
	GLAssertError(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		char infoLog[512];
		GLAssertError(glGetShaderInfoLog(id, 512, NULL, infoLog));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << infoLog << std::endl;
		GLAssertError(glDeleteShader(id));
		return 0;
	}
	
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	GLAssertError(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLAssertError(glAttachShader(program, vs));
	GLAssertError(glAttachShader(program, fs));
	GLAssertError(glLinkProgram(program));
	
	int result;
	GLAssertError(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE)
	{
		char infoLog[512];
		GLAssertError(glGetProgramInfoLog(program, 512, NULL, infoLog));
		std::cout << "Failed to Link Shader Program" << std::endl;
		std::cout << infoLog << std::endl;
		GLAssertError(glDeleteProgram(program));
		GLAssertError(glDeleteShader(vs));
		GLAssertError(glDeleteShader(fs));
		return 0;
	}

	GLAssertError(glDeleteShader(vs));
	GLAssertError(glDeleteShader(fs));

	return program;
}

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

		VertexArray vao;
		VertexBuffer vbo(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);
		IndexBuffer ibo(indices, 6);

		unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
		GLAssertError(glUseProgram(shader));

		GLAssertError(int u_ColorId = glGetUniformLocation(shader, "u_Color"));
		AssertBreak(u_ColorId == -1);
		GLAssertError(glUniform4f(u_ColorId, color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]));

		float rInc = 0.03f;
		float gInc = 0.05f;
		float bInc = 0.07f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			if (color[(int)rgba::RED] >= 1 || color[(int)rgba::RED] <= 0)
				rInc = -rInc;
			if (color[(int)rgba::GREEN] >= 1 || color[(int)rgba::GREEN] <= 0)
				gInc = -rInc;
			if (color[(int)rgba::BLUE] >= 1 || color[(int)rgba::BLUE] <= 0)
				bInc = -bInc;

			color[(int)rgba::RED] += rInc;
			color[(int)rgba::GREEN] += gInc;
			color[(int)rgba::BLUE] += bInc;

			GLAssertError(glUniform4f(u_ColorId, color[(int)rgba::RED], color[(int)rgba::GREEN], color[(int)rgba::BLUE], color[(int)rgba::ALPHA]));

			GLAssertError(glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		GLAssertError(glDeleteProgram(shader));
	}
	glfwTerminate();
	
	return 0;
}