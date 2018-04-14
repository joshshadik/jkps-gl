#pragma once

#include <GL/glew.h>
#include <GL/GL.h>

#include <memory>
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
				Vertex,
				Fragment
			};

			Shader(const std::string& source, Type type);
			~Shader();

			void attach(GLuint program);
			void detach(GLuint program);

			static std::shared_ptr<Shader> loadFromFile(const std::string& filePath, Type type);

		private:
			static GLenum typeToNative(Type);

			GLuint _shaderID;
			Type _type;
		};


		class ShaderProgram
		{
		public:
			ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders);
			ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs);
			~ShaderProgram();

			void bind();
			GLint getUniformLocation(const std::string& name);


		private:
			GLuint _programID;

			std::vector<std::shared_ptr<Shader>> _shaders;
		};

	}
}