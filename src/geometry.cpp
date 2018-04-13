#include "geometry.h"

#include <iostream>

using namespace jkps::gl;

Geometry::Geometry(const VertexData& vertexData, const IndexData& indices)
	: _vertexData(vertexData)
	, _indices(indices)
{
	for (int i = 0; i < _vertexData.size(); ++i)
	{
		GLuint vbo;
		glCreateBuffers(1, &vbo);
		_vbos.push_back(vbo);
	}
	
	glCreateBuffers(1, &_ibo);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	for (int i = 0; i < _vertexData.size(); ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
		glBufferData(GL_ARRAY_BUFFER, _vertexData[i].first.size() * sizeof(GLfloat), _vertexData[i].first.data(), GL_STATIC_DRAW);
		_vertexData[i].second.bind();
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);


	_elementCount = indices.size();
}

void Geometry::bind()
{
	glBindVertexArray(_vao);
}

void Geometry::render()
{
	glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, NULL);
}

VertexLayout::VertexLayout(const std::vector<VertexAttribute>& attributes, const uint16_t stride)
	: _attributes(attributes)
	, _stride(stride)
{
}

VertexLayout::VertexLayout()
	: VertexLayout({VertexAttribute(0, 3, 0)}, 12)
{
}

void VertexLayout::bind()
{
	for (int aa = 0; aa < _attributes.size(); ++aa)
	{	
		glVertexAttribPointer(_attributes[aa]._index, _attributes[aa]._size, GL_FLOAT, GL_FALSE, _stride, (void*)(_attributes[aa]._offset));
		glEnableVertexAttribArray(_attributes[aa]._index);
	}
}
