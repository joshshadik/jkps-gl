#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <optional>

namespace jkps
{
    namespace gl
    {
        class Texture
        {
        public:
            Texture(std::optional<std::vector<uint8_t>> data, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE);
            ~Texture();

            void bind();
            void unbind();

            GLuint id() { return _textureID; }

        private:
            GLuint _textureID;

        };

    }
}