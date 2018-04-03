#include "ErrorHandler.h"
#include<iostream>
#include <GL/glew.h>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(const char* function, const char* file, int line)
{
	GLenum error;
	if ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "[OPENGL ERROR](" << error << ")" << " from function call " << function << " in " << file << " at " << line << std::endl;
		return true;
	}
	return false;
}