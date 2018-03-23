#pragma once

#include <GL/glew.h>

#ifdef _DEBUG
#define AssertBreak(x) if (x) __debugbreak();
#define GLAssertError(x) GLClearError(); x; AssertBreak(GLLogError(#x, __FILE__, __LINE__));
#else
#define GLAssertError(x) x
#endif

void GLClearError();

bool GLLogError(const char* function, const char* file, int line);