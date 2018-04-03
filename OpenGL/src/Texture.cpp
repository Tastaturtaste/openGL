#include "Texture.h"
#include "ErrorHandler.h"
#include "stb_image\stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_filepath(path), m_localBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	GLAssertError(glGenTextures(1, &m_RendererID));
	GLAssertError(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(m_filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	GLAssertError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAssertError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLAssertError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLAssertError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLAssertError(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLAssertError(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

Texture::~Texture()
{
	GLAssertError(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /* = 0 */) const
{
	GLAssertError(glActiveTexture(GL_TEXTURE0 + slot));
	GLAssertError(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLAssertError(glBindTexture(GL_TEXTURE_2D, 0));
}