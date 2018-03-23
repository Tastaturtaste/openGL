#pragma once

#include <vector>



struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:

	VertexBufferLayout();
	template<typename T>
	void Push(unsigned int count);
	const std::vector<VertexBufferElement>& GetElements() const;
	unsigned int GetStride() const;
};