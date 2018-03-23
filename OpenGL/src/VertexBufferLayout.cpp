#include "VertexBufferLayout.h"
#include "Renderer.h"


unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
	}
	AssertBreak(true);
	return 0;
}

VertexBufferLayout::VertexBufferLayout()
	:m_Stride(0)
{
}

template<typename T>
void VertexBufferLayout::Push(unsigned int count)
{
	static_assert(false);
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
	m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const
{
	return m_Elements;
}

unsigned int VertexBufferLayout::GetStride() const
{
	return m_Stride;
}