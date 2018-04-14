#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/GL.h>

#include <memory>
#include <string>
#include <vector>

namespace jkps
{
    namespace gl
    {
        class MaterialUniformBlock
        {
        public:
            MaterialUniformBlock(const void* buffer, const size_t size);

            void uploadData();
            void bind(int index);

            void setValue(void* data, size_t offset, size_t size);

        private:
            GLuint _ubo;

            void* _buffer;
            const size_t _size;
        };

        class Material
        {
        public:
            Material(std::shared_ptr<ShaderProgram> program);
            void addUniformBlock(uint32_t binding, std::shared_ptr<MaterialUniformBlock> uniformBlock);

            void addTexture(GLint uniformLocation, std::shared_ptr<Texture> tex);
            GLint getUniformLocation(const std::string& name);

            void bind();
            void unbind();

        private:
            std::shared_ptr<ShaderProgram> _sp;
            std::vector <std::pair<uint32_t, std::shared_ptr<MaterialUniformBlock>>> _uniformBlocks;

            std::vector<std::pair<GLint, std::shared_ptr<Texture>>> _textures;
        };

    }
}