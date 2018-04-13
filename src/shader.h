#pragma once

#include <GL/glew.h>
#include <GL/GL.h>

#include <memory>
#include <string>

namespace jkps 
{
	namespace gl 
	{

		class Shader
		{
		public:
			enum Type
			{
				Vertex,
				Fragment
			};

			Shader(const std::string& source, Type type);

			void attach(GLuint program);

			static std::shared_ptr<Shader> loadFromFile(const std::string& filePath, Type type);

		private:
			static GLenum typeToNative(Type);

			GLuint _shaderID;
			Type _type;
		};


		class ShaderProgram
		{
		public:
			ShaderProgram(std::initializer_list<std::shared_ptr<Shader>> shaders);
			ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs);

			void bind();
			GLint getUniformLocation(const std::string& name);


		private:
			GLuint _programID;
		};

	}
}