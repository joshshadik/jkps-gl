#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace jkps
{
    namespace gl
    {
        class Texture
        {
        public:
            Texture(std::vector<uint8_t> data, const glm::ivec2& size, GLuint format, GLuint layout);

            void bind();
            void unbind();

        private:
            GLuint _textureID;

        };

    }
}