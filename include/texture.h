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
            Texture(const uint8_t* data, const int dataSize, const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE, GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST);
            Texture(const glm::ivec2& size, GLuint format, GLuint layout, GLenum dataType = GL_UNSIGNED_BYTE, GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST);
            Texture();
            ~Texture();
            Texture(Texture&& tex);
            Texture& operator=(Texture&& tex);

            void bind();
            void unbind();

			const glm::vec2& size() { return _size; }
			GLuint format() { return _format; }
			GLuint layout() { return _layout; }
			GLenum dataType() { return _dataType; }
			GLint magFilter() { return _magFilter; }
			GLint minFilter() { return _minFilter; }

			static void loadFromFile(Texture* texture, const std::string& filePath, int channels);

            GLuint id() { return _textureID; }

        private:
            Texture(const Texture&) = delete;
            Texture& operator=(Texture const &) = delete;

            GLuint _textureID;
            bool _valid = true;

			glm::vec2 _size;
			GLuint _format;
			GLuint _layout;
			GLenum _dataType;
			GLint _magFilter;
			GLint _minFilter;
        };

    }
}