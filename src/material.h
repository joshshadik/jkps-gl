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

        class Material
        {
        public:
            typedef std::variant< glm::mat4, glm::vec4, glm::vec3, glm::vec2, float, int, uint32_t> UniformValue;

            Material(std::shared_ptr<ShaderProgram> program);
            void addUniformBlock(uint32_t binding, std::shared_ptr<MaterialUniformBlock> uniformBlock);

            void addTexture(GLint uniformLocation, std::shared_ptr<Texture> tex);

            GLint getUniformLocation(const std::string& name);
            void setUniform(GLint location, Uniform value);
            void setUniform(GLint location, const glm::mat4& value);
            void setUniform(GLint location, const glm::vec4& value);
            void setUniform(GLint location, const glm::vec3& value);
            void setUniform(GLint location, const glm::vec2& value);

            void bind();
            void unbind();

        private:
            std::shared_ptr<ShaderProgram> _sp;
            std::vector <std::pair<uint32_t, std::shared_ptr<MaterialUniformBlock>>> _uniformBlocks;

            std::vector<std::pair<GLint, glm::mat4>> _matrices;
            std::vector<std::pair<GLint, glm::vec4>> _vec4Uniforms;
            std::vector<std::pair<GLint, glm::vec3>> _vec3Uniforms;
            
            std::map<GLint, Uniform> _uniforms;

            std::vector<std::pair<GLint, std::shared_ptr<Texture>>> _textures;
        };

    }
}