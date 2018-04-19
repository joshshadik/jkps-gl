#include "texture.h"

#include <turbojpeg.h>

#include <vector>
#include <glm/glm.hpp>


using namespace jkps::gl;

Texture::Texture(std::optional<std::vector<uint8_t>> data, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType)
{
    glGenTextures(1, &_textureID);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, layout, dataType, data ? data.value().data() : GL_NONE);

    unbind();

}

Texture::~Texture()
{
    glDeleteTextures(1, &_textureID);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
