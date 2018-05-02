#pragma once

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

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
            Texture(const std::vector<uint8_t>& data, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE);
            Texture(const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE);
            ~Texture();

            void bind();
            void unbind();

            GLuint id() { return _textureID; }

        private:
            GLuint _textureID;

        };

    }
}