#include "shader.h"

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace jkps::gl;


std::map<std::string, GLuint> jkps::gl::Shader::StandardAttrLocations = {
	{"POSITION", 0},
	{"TEXCOORD_0", 1},
	{"NORMAL", 2}
};

Shader::Shader(const std::string & source, Type type)
{
	_shaderID = glCreateShader(typeToNative(type));
	const GLchar *src = source.c_str();
	glShaderSource(_shaderID, 1, &src, 0);
	glCompileShader(_shaderID);

	GLint isCompiled = 0;
	glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(_shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << errorLog.data() << '\n';

		glDeleteShader(_shaderID); // Don't leak the shader.
		return;
	}
}

void Shader::attach(GLuint program)
{
	glAttachShader(program, _shaderID);
}

std::shared_ptr<Shader> Shader::loadFromFile(const std::string & filePath, Type type)
{
	std::ifstream t(filePath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string src = buffer.str();
	return std::make_shared<Shader>(src, type);
}

GLenum Shader::typeToNative(Type type)
{
	switch (type)
	{
	case Type::Vertex:
		return GL_VERTEX_SHADER;

	case Type::Fragment:
		return GL_FRAGMENT_SHADER;
	}

	std::runtime_error("non valid shader type");
	return -1;
}

ShaderProgram::ShaderProgram(std::initializer_list<std::shared_ptr<Shader>> shaders)
{
	_programID = glCreateProgram();
	for (auto& s : shaders)
	{
		s->attach(_programID);
	}

	//Link program
	glLinkProgram(_programID);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(_programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", _programID);
	}

}

ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs)
	: ShaderProgram({ vs, fs })
{
}

void ShaderProgram::bind()
{
	glUseProgram(_programID);
}

GLint ShaderProgram::getUniformLocation(const std::string & name)
{
	return glGetUniformLocation(_programID, name.c_str());
}
