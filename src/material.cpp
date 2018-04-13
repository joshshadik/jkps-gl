#include "material.h"

using namespace jkps::gl;

MaterialUniformBlock::MaterialUniformBlock(const void* buffer, const size_t size)
	: _buffer(buffer)
	, _size(size)
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, _size, _buffer, GL_DYNAMIC_DRAW);
}

void MaterialUniformBlock::uploadData()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, _buffer, _size);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void MaterialUniformBlock::bind(int index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, _ubo);
}

Material::Material(std::shared_ptr<ShaderProgram> program)
	: _sp(program)
{
}

void Material::addUniformBlock(uint32_t binding, std::shared_ptr<MaterialUniformBlock> uniformBlock)
{
	_uniformBlocks.push_back(std::make_pair(binding, uniformBlock));
}

void Material::addTexture(GLint uniformLocation, std::shared_ptr<Texture> tex)
{
	_textures.push_back(std::make_pair(uniformLocation, tex));
}

GLint Material::getUniformLocation(const std::string & name)
{
	return _sp->getUniformLocation(name);
}

void Material::bind()
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
