#include "material.h"

#include <glm/gtc/type_ptr.hpp>

using namespace jkps::gl;

jkps::gl::MaterialUniformBlock::MaterialUniformBlock(const Descriptor& descriptor)
    : _descriptor(descriptor)
{
    auto variables = _descriptor.first;
    for (const auto& v : variables)
    {
        _offsets[v.first] = v.second;
    }

    _buffer.resize(_descriptor.second);

    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, _descriptor.second, _buffer.data(), GL_DYNAMIC_DRAW);
}

void jkps::gl::MaterialUniformBlock::uploadData()
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
#ifdef USE_WASM
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _buffer.size(), _buffer.data());
#else
    GLvoid* p = glMapBufferRange(GL_UNIFORM_BUFFER, 0, 1, GL_MAP_WRITE_BIT);
    memcpy(p, _buffer.data(), _buffer.size());
    glUnmapBuffer(GL_UNIFORM_BUFFER);

#endif
}

void jkps::gl::MaterialUniformBlock::bind(int binding)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, _ubo);
}

void jkps::gl::MaterialUniformBlock::setValue(const std::string & key, const void * data, const size_t size)
{
    uint32_t offset = _offsets[key];
    printf("offset for %s: %d \n", key.c_str(), offset);
    memcpy(_buffer.data() + offset, data, size);
}

void jkps::gl::MaterialUniformBlock::setValue(const void * data, const size_t byteOffset, const size_t size)
{
    memcpy(_buffer.data() + byteOffset, data, size);
}

jkps::gl::Material::Material(std::shared_ptr<ShaderProgram> program)
    : _program(program)
{

}

void jkps::gl::Material::addUniformBlock(uint32_t binding, uint32_t location, std::shared_ptr<MaterialUniformBlock> uniformBlock)
{
    _program->bindUBO(location, binding);
    _uniformBlocks.push_back(std::make_pair(std::make_pair(location, uniformBlock), binding));
}


GLint jkps::gl::Material::getUniformLocation(const std::string & name)
{
    return _program->getUniformLocation(name);
}

void jkps::gl::Material::setUniform(GLint location, Uniform value)
{
    _uniforms[location] = value;
}

void jkps::gl::Material::setUniform(GLint location, const glm::mat4 & value)
{
    _uniforms[location].setValue(value);
}

void jkps::gl::Material::setUniform(GLint location, const glm::vec4 & value)
{
    _uniforms[location].setValue(value);
}

void jkps::gl::Material::setUniform(GLint location, const glm::vec3 & value)
{
    _uniforms[location].setValue(value);
}

void jkps::gl::Material::setUniform(GLint location, const glm::vec2 & value)
{
    _uniforms[location].setValue(value);
}

void jkps::gl::Material::setUniform(GLint location, std::shared_ptr<Texture> tex)
{
    _textures[location] = tex;
}

void jkps::gl::Material::bind()
{
    _program->bind();

    if (_blended)
    {
        glEnable(GL_BLEND);
        glBlendFunc(_blendSrc, _blendDst);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    for (auto& block : _uniformBlocks)
    {
        block.first.second->bind(block.second);
    }

    for (auto& uniform : _uniforms)
    {
        uniform.second.bind(uniform.first);
    }

    int texCnt = 0;
    for (auto& tex : _textures)
    {
        glActiveTexture(GL_TEXTURE0 + texCnt);
        tex.second->bind();
        glUniform1i(tex.first, texCnt);
        texCnt++;
    }
}

void jkps::gl::Material::unbind()
{
    for (auto& tex : _textures)
    {
        tex.second->unbind();
    }
}


void jkps::gl::Uniform::bind(GLint location)
{
    switch (_type)
    {
    case Mat4:
    {
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(_value.m));
    }
    break;

    case Vec4:
    {
        glUniform4fv(location, 1, glm::value_ptr(_value.v4));
    }
    break;

    case Vec3:
    {
        glUniform3fv(location, 1, glm::value_ptr(_value.v3));
    }
    break;

    case Vec2:
    {
        glUniform2fv(location, 1, glm::value_ptr(_value.v2));
    }
    break;

    case Float:
    {
        glUniform1fv(location, 1, &_value.f);
    }
    break;

    case Int:
    {
        glUniform1i(location, _value.i);
    }
    break;

    case UInt:
    {
        glUniform1ui(location, _value.u);
    }
    break;
    }
}
