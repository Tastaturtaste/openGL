#pragma once
#include "ErrorHandler.h"
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Clear();
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
};