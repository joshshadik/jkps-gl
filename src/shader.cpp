#include "shader.h"

#include <fstream>
#include <sstream>
#include <vector>

using namespace jkps::gl;


std::map<std::string, GLuint> jkps::gl::Shader::StandardAttrLocations = {
    {"POSITION", 0},
    {"TEXCOORD_0", 1},
    {"NORMAL", 2},
    {"TANGENT", 3}
};

bool jkps::gl::Shader::getStandardAttribute(const std::string & key, GLuint & outValue)
{
    auto v = StandardAttrLocations.find(key);
    if (v != StandardAttrLocations.end())
    {
        outValue = (*v).second;
        return true;
    }
    return false;
}

Shader::Shader(const std::string & source, Type type)
    : _type(type)
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

        printf("%s\n", errorLog.data());

        _type = Invalid;
        glDeleteShader(_shaderID); // Don't leak the shader.
        return;
    }
}

Shader::Shader()
    : _type(Invalid)
{

}

jkps::gl::Shader::~Shader()
{
    if (_type != Invalid)
    {
        glDeleteShader(_shaderID);
    }
}

Shader::Shader(Shader&& shader)
    : _type(shader._type)
    , _shaderID(shader._shaderID)
{
    shader._type = Invalid;
}

Shader& Shader::operator=(Shader&& shader)
{
    _type = shader._type;
    _shaderID = shader._shaderID;

    shader._type = Invalid;

    return *this;
}

void Shader::attach(GLuint program)
{
    glAttachShader(program, _shaderID);
}

void jkps::gl::Shader::detach(GLuint program)
{
    glDetachShader(program, _shaderID);
}

void Shader::loadFromFile(Shader* shader, const std::string & filePath, Type type)
{
    printf("loading shader from file %s \n", filePath.c_str());

    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string src = buffer.str();
    *shader = Shader(src, type);
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

jkps::gl::ShaderProgram::ShaderProgram(std::vector<Shader*> shaders)
    : _shaders(shaders)
{
    _programID = glCreateProgram();
    for (auto s : _shaders)
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


ShaderProgram::ShaderProgram(Shader* vs, Shader* fs)
    : ShaderProgram(std::vector<Shader*>{ vs, fs })
{
}

jkps::gl::ShaderProgram::ShaderProgram()
    : _valid(false)
{

}

jkps::gl::ShaderProgram::~ShaderProgram()
{
    if (_valid)
    {
        for (auto shader : _shaders)
        {
            shader->detach(_programID);
        }
    }
}

jkps::gl::ShaderProgram::ShaderProgram(ShaderProgram&& program)
    : _programID(program._programID)
    , _shaders(std::move(program._shaders))
    , _valid(program._valid)
{
    program._valid = false;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& program)
{
    _programID = program._programID;
    _shaders = std::move(program._shaders);

    _valid = program._valid;
    program._valid = false;

    return *this;
}

void ShaderProgram::bind()
{
    glUseProgram(_programID);
}

void jkps::gl::ShaderProgram::bindUBO(GLuint location, GLuint binding)
{
    glUniformBlockBinding(_programID, location, binding);
}

GLint ShaderProgram::getUniformLocation(const std::string & name)
{
    return glGetUniformLocation(_programID, name.c_str());
}

GLint jkps::gl::ShaderProgram::getUniformBlockLocation(const std::string & name)
{
    return glGetUniformBlockIndex(_programID, name.c_str());
}
