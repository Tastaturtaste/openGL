#include "Renderer.h"

void Renderer::Clear()
{
	GLAssertError(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{
	vao.Bind();
	ibo.Bind();
	shader.Bind();
	GLAssertError(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}