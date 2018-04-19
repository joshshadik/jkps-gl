#pragma once

#include "texture.h"

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace jkps
{
    namespace gl
    {
        class Framebuffer
        {
        public:
            Framebuffer();

            void setup(const std::vector<std::shared_ptr<Texture>>& color, std::shared_ptr<Texture> depth, const glm::ivec2& size);

            std::shared_ptr<Texture> color(int index = 0) { return _color[index]; }
            std::shared_ptr<Texture> depth() { return _depth; }
            GLuint id() { return _fboId; }

            void bind();
            static void bindDefault();
            static void bindDefault(const glm::ivec2& viewport);

        private:
            GLuint _fboId;

            glm::ivec2 _size;
            std::vector<std::shared_ptr<Texture>> _color;
            std::shared_ptr<Texture> _depth;

        };
    }
}