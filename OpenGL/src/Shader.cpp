#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string & shaderFile)
	: m_RendererID(0), m_Filepath(shaderFile)
{
	ShaderSource source = ParseShader(shaderFile);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLAssertError(glDeleteProgram(m_RendererID));
}

ShaderSource Shader::ParseShader(const std::string& filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

void Shader::Bind() const
{
	GLAssertError(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLAssertError(glUseProgram(0));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLAssertError(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning! Uniform \"" << name << "\" doesn´t exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLAssertError(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int v)
{
	GLAssertError(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLAssertError(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}