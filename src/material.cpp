#include "material.h"

using namespace jkps::gl;

jkps::gl::MaterialUniformBlock::MaterialUniformBlock(const void* buffer, const size_t size)
    : _buffer(const_cast<void*>(buffer))
    , _size(size)
{
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, _size, _buffer, GL_DYNAMIC_DRAW);
}

void jkps::gl::MaterialUniformBlock::uploadData()
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, _buffer, _size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void jkps::gl::MaterialUniformBlock::bind(int index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, _ubo);
}

void jkps::gl::MaterialUniformBlock::setValue(void * data, size_t byteOffset, size_t size)
{
    memcpy(reinterpret_cast<unsigned char*>(_buffer) + byteOffset, data, size);
}

jkps::gl::Material::Material(std::shared_ptr<ShaderProgram> program)
    : _sp(program)
{
}

void jkps::gl::Material::addUniformBlock(uint32_t binding, std::shared_ptr<MaterialUniformBlock> uniformBlock)
{
    _uniformBlocks.push_back(std::make_pair(binding, uniformBlock));
}

void jkps::gl::Material::addTexture(GLint uniformLocation, std::shared_ptr<Texture> tex)
{
    _textures.push_back(std::make_pair(uniformLocation, tex));
}

GLint jkps::gl::Material::getUniformLocation(const std::string & name)
{
    return _sp->getUniformLocation(name);
}

void jkps::gl::Material::bind()
{
    _sp->bind();

    for (auto& block : _uniformBlocks)
    {
        block.second->bind(block.first);
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
