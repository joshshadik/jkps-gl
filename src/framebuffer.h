#pragma once

#include "texture.h"

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <vector>

namespace jkps
{
    namespace gl
    {
        class Framebuffer
        {
        public:
            Framebuffer(std::vector<Texture*> color, Texture* depth, const glm::ivec2& size);
            Framebuffer();
            ~Framebuffer();

            Framebuffer(Framebuffer&& fb);
            Framebuffer& operator=(Framebuffer&& fb);

            Texture* color(int index = 0) { return _color.at(index); }
            Texture* depth() { return _depth; }
            GLuint id() { return _fboId; }

            void bind();
            static void bindDefault();
            static void bindDefault(const glm::ivec2& viewport);
            static void bindDefaultVP(const glm::ivec4& viewport);

        private:
            Framebuffer(const Framebuffer&) = delete;
            Framebuffer& operator=(Framebuffer const&) = delete;

            GLuint _fboId;

            glm::ivec2 _size;
            std::vector<Texture*> _color;
            Texture* _depth;

            bool _valid = false;

        };
    }
}