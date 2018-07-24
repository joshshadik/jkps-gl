#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <string>
#include <vector>

namespace jkps
{
    namespace gl
    {
        class Uniform
        {
        public:
            //typedef std::variant< glm::mat4, glm::vec4, glm::vec3, glm::vec2, float, int, uint32_t> Value;

            union Value
            {
                Value() {}
                glm::mat4 m;
                glm::vec4 v4;
                glm::vec3 v3;
                glm::vec2 v2;
                float f;
                int i;
                uint32_t u;
            };

            enum Type {
                Mat4,
                Vec4,
                Vec3,
                Vec2,
                Float,
                Int,
                UInt,
            };

            Uniform() {
                _value.u = 0;
                _type = UInt;
            }

            void setValue(Value value, Type type)
            {
                _value = value;
                _type = type;
            }

            void setValue(glm::mat4 value)  { _value.m = value; _type = Mat4; }
            void setValue(glm::vec4 value)  { _value.v4 = value; _type = Vec4; }
            void setValue(glm::vec3 value)  { _value.v3 = value; _type = Vec3; }
            void setValue(glm::vec2 value)  { _value.v2 = value; _type = Vec2; }
            void setValue(float value)      { _value.f = value; _type = Float; }
            void setValue(int value)        { _value.i = value; _type = Int; }
            void setValue(uint32_t value)   { _value.u = value; _type = UInt; }

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
            MaterialUniformBlock(uint8_t* buffer,  const Descriptor& descriptor);
            MaterialUniformBlock(const Descriptor& descriptor);
            MaterialUniformBlock();
            ~MaterialUniformBlock();
            MaterialUniformBlock(MaterialUniformBlock&& uniformBlock);
            MaterialUniformBlock& operator=(MaterialUniformBlock&& uniformBlock);

            void uploadData();
            void bind(int index);

            void setValue(const std::string& key, const void* data, const size_t size);
            void setValue(const void* data, const size_t offset, const size_t size);

        private:
            MaterialUniformBlock(const MaterialUniformBlock&) = delete;
            MaterialUniformBlock& operator=(MaterialUniformBlock const&) = delete;

            GLuint _ubo;

            Descriptor _descriptor;
            std::vector<uint8_t> _buffer;
            uint8_t* _bufPtr;
			size_t _bufferSize;
            std::map<std::string, uint32_t> _offsets;

            bool _valid = true;
        };



        class Material
        {
        public:
            //typedef std::variant< glm::mat4, glm::vec4, glm::vec3, glm::vec2, float, int, uint32_t> UniformValue;

            Material(ShaderProgram* program);
            Material();
            void addUniformBlock(uint32_t binding, uint32_t location, MaterialUniformBlock* uniformBlock);

            GLint getUniformLocation(const std::string& name);
            void setUniform(GLint location, Uniform value);
            void setUniform(GLint location, const glm::mat4& value);
            void setUniform(GLint location, const glm::vec4& value);
            void setUniform(GLint location, const glm::vec3& value);
            void setUniform(GLint location, const glm::vec2& value);
            void setUniform(GLint location, const float value);
            void setUniform(GLint location, const int value);
            void setUniform(GLint location, Texture* tex);

            void setBlended(bool blend) { _blended = blend; }
            void setBlendFunction(GLenum src, GLenum dst) { _blendSrc = src; _blendDst = dst; }

			Texture* getTexture(GLint location);

			bool isBlended() { return _blended; }

            void bind(ShaderProgram* replacementProgram = nullptr);
            void unbind();

        private:

            typedef std::pair<uint32_t, MaterialUniformBlock*> IndexedUniformBlock;

            ShaderProgram* _program;
            std::vector <std::pair<IndexedUniformBlock, uint32_t>> _uniformBlocks;
            std::map<GLint, Uniform> _uniforms;
            std::map<GLint, Texture*> _textures;

            bool _blended = false;
            GLenum _blendSrc;
            GLenum _blendDst;
        };

    }
}