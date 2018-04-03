#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	: m_RendererID(0)
{
	GLAssertError(glGenBuffers(1, &m_RendererID));
	GLAssertError(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLAssertError(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLAssertError(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLAssertError(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLAssertError(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
