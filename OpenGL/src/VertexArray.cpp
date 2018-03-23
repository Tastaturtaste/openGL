#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	GLAssertError(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLAssertError(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	Bind();
	vbo.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		GLAssertError(glEnableVertexAttribArray(i));
		GLAssertError(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)offset));
		offset += elements[i].count * elements[i].GetSizeOfType(elements[i].type);
	}
}

void VertexArray::Bind() const
{
	GLAssertError(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLAssertError(glBindVertexArray(0));
}
