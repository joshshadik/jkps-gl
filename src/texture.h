#pragma once

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <glm/glm.hpp>

#include <vector>


namespace jkps
{
    namespace gl
    {
        class Texture
        {
        public:
            Texture(const uint8_t* data, const int dataSize, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE, GLint magFilter = GL_LINEAR, GLint minFilter = GL_LINEAR);
            Texture(const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE, GLint magFilter = GL_LINEAR, GLint minFilter = GL_LINEAR);
            Texture();
            ~Texture();
            Texture(Texture&& tex);
            Texture& operator=(Texture&& tex);

            void bind();
            void unbind();

			static void loadFromFile(Texture* texture, const std::string& filePath, int channels);

            GLuint id() { return _textureID; }

        private:
            Texture(const Texture&) = delete;
            Texture& operator=(Texture const &) = delete;

            GLuint _textureID;
            bool _valid = true;

        };

    }
}