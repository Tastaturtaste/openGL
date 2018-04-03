#pragma once

#include "Renderer.h"


class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline std::string GetPath() const { return m_filepath; }
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeigth() const { return m_Height; }

};