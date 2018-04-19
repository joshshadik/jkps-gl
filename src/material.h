#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/GL.h>

#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace jkps
{
    namespace gl
    {
        class Uniform
        {
        public:
            typedef std::variant< glm::mat4, glm::vec4, glm::vec3, glm::vec2, float, int, uint32_t> Value;
            enum Type {
                Mat4,
                Vec4,
                Vec3,
                Vec2,
                Float,
                Int,
                UInt,
            };

            void setValue(Value value, Type type)
            {
                _value = value;
                _type = type;
            }

            void setValue(glm::mat4 value) { setValue(value, Mat4); }
            void setValue(glm::vec4 value) { setValue(value, Vec4); }
            void setValue(glm::vec3 value) { setValue(value, Vec3); }
            void setValue(glm::vec2 value) { setValue(value, Vec2); }
            void setValue(float value) { setValue(value, Float); }
            void setValue(int value) { setValue(value, Int); }
            void setValue(uint32_t value) { setValue(value, UInt); }

            void bind(GLint location);

        private:
            Value _value;
            Type _type;
        };

        class MaterialUniformBlock
        {
        public:
            typedef std::pair<std::vector<std::pair<std::string, uint32_t>>, size_t> Descriptor;

        public:
            MaterialUniformBlock(const Descriptor& descriptor);

            void uploadData();
            void bind(int index);

            void setValue(const std::string& key, const void* data, const size_t size);
            void setValue(const void* data, const size_t offset, const size_t size);

        private:
            GLuint _ubo;

            Descriptor _descriptor;
            std::vector<uint8_t> _buffer;
            std::map<std::string, uint32_t> _offsets;
        };



        class Material
        {
        public:
            typedef std::variant< glm::mat4, glm::vec4, glm::vec3, glm::vec2, float, int, uint32_t> UniformValue;

            Material(std::shared_ptr<ShaderProgram> program);
            void addUniformBlock(uint32_t binding, std::shared_ptr<MaterialUniformBlock> uniformBlock);

            GLint getUniformLocation(const std::string& name);
            void setUniform(GLint location, Uniform value);
            void setUniform(GLint location, const glm::mat4& value);
            void setUniform(GLint location, const glm::vec4& value);
            void setUniform(GLint location, const glm::vec3& value);
            void setUniform(GLint location, const glm::vec2& value);
            void setUniform(GLint location, std::shared_ptr<Texture> tex);

            void setBlended(bool blend) { _blended = blend; }
            void setBlendFunction(GLenum src, GLenum dst) { _blendSrc = src; _blendDst = dst; }

            void bind();
            void unbind();

        private:
            std::shared_ptr<ShaderProgram> _program;
            std::vector <std::pair<uint32_t, std::shared_ptr<MaterialUniformBlock>>> _uniformBlocks;          
            std::map<GLint, Uniform> _uniforms;
            std::map<GLint, std::shared_ptr<Texture>> _textures;

            bool _blended = false;
            GLenum _blendSrc;
            GLenum _blendDst;

            //std::vector<std::pair<GLint, std::shared_ptr<Texture>>> _textures;
        };

    }
}