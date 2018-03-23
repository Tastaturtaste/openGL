#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	GLAssertError(glGenBuffers(1, &m_RendererID));
	GLAssertError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLAssertError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLAssertError(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLAssertError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLAssertError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}
