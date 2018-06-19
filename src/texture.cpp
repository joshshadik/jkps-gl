#include "texture.h"

#include <vector>
#include <glm/glm.hpp>

#include <stb_image.h>


using namespace jkps::gl;

Texture::Texture(const uint8_t* data, const int dataSize, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType, GLint magFilter, GLint minFilter)
{
    glGenTextures(1, &_textureID);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, layout, dataType, dataSize > 0 ? data : GL_NONE);

    unbind();
}

jkps::gl::Texture::Texture(const glm::ivec2 & size, GLuint format, GLuint layout, GLenum dataType, GLint magFilter, GLint minFilter)
    :Texture(nullptr, 0, size, format, layout, dataType, magFilter, minFilter)
{

}

jkps::gl::Texture::Texture()
    : _valid(false)
{

}

Texture::~Texture()
{
    if (_valid)
    {
        glDeleteTextures(1, &_textureID);
    }
}

jkps::gl::Texture::Texture(Texture && tex)
    : _textureID(tex._textureID)
    , _valid(tex._valid)
{
    tex._valid = false;
}

Texture & jkps::gl::Texture::operator=(Texture && tex)
{
    if (_valid)
    {
        glDeleteTextures(1, &_textureID);
    }

    _textureID = tex._textureID;
    _valid = tex._valid;
    tex._valid = false;

    return *this;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void jkps::gl::Texture::loadFromFile(Texture * texture, const std::string & filePath, int channels)
{
	int width, height, bpp;
	unsigned char* rgb = stbi_load(filePath.c_str(), &width, &height, &bpp, channels);

	*texture = Texture(rgb, width*height*channels, glm::ivec2(width, height), GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR);


	stbi_image_free(rgb);
}
