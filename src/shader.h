#pragma once

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <string>
#include <map>
#include <vector>

namespace jkps
{
    namespace gl
    {


        class Shader
        {
        public:
            static std::map<std::string, GLuint> StandardAttrLocations;

            static bool getStandardAttribute(const std::string& key, GLuint& outValue);

        public:
            enum Type
            {
                Invalid,
                Vertex,
                Fragment
            };

            Shader(const std::string& source, Type type);
            Shader();
            ~Shader();
            Shader(Shader&& program);
            Shader& operator=(Shader&& program);

            void attach(GLuint program);
            void detach(GLuint program);

            static void loadFromFile(Shader* shader, const std::string& filePath, Type type);

        private:
            Shader(const Shader&) = delete;
            Shader& operator=(Shader const &) = delete;

            static GLenum typeToNative(Type);

            GLuint _shaderID;
            Type _type = Type::Invalid;
        };


        class ShaderProgram
        {
        public:
            ShaderProgram(std::vector<Shader*> shaders);
            ShaderProgram(Shader* vs, Shader* fs);
            ~ShaderProgram();
            ShaderProgram();
            ShaderProgram(ShaderProgram&& program);
            ShaderProgram& operator=(ShaderProgram&& program);

            void bind();

            void bindUBO(GLuint location, GLuint index);
            GLint getUniformLocation(const std::string& name);
            GLint getUniformBlockLocation(const std::string& name);

        private:
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram& operator=(ShaderProgram const &) = delete;

            GLuint _programID;
            bool _valid = true;
            std::vector<Shader*> _shaders;
        };

    }
}