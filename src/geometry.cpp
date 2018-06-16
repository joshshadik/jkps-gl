#include "geometry.h"

using namespace jkps::gl;

Geometry::Geometry(const VertexData& vertexData, const IndexData& indices)
    : _vertexData(vertexData)
    , _indices(indices)
{
    for (int i = 0; i < _vertexData.size(); ++i)
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        _vbos.push_back(vbo);
    }

    glGenBuffers(1, &_ibo);

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

jkps::gl::Geometry::Geometry()
    :_valid(false)
{
}

jkps::gl::Geometry::~Geometry()
{
    if (_valid)
    {
        glDeleteVertexArrays(1, &_vao);

        glDeleteBuffers(_vbos.size(), _vbos.data());
        glDeleteBuffers(1, &_ibo);
    }
}

jkps::gl::Geometry::Geometry(Geometry && geo)
    : _vertexData(std::move(geo._vertexData))
    , _indices(std::move(geo._indices))
    , _elementCount(geo._elementCount)
    , _vbos(std::move(geo._vbos))
    , _ibo(geo._ibo)
    , _vao(geo._vao)
    , _valid(geo._valid)
{
    geo._valid = false;
}

Geometry& jkps::gl::Geometry::operator=(Geometry && geo)
{
    if (_valid)
    {
        glDeleteVertexArrays(1, &_vao);

        glDeleteBuffers(_vbos.size(), _vbos.data());
        glDeleteBuffers(1, &_ibo);
    }

    _vertexData = std::move(geo._vertexData);
    _indices = std::move(geo._indices);
    _elementCount = geo._elementCount;
    _vbos = std::move(geo._vbos);
    _ibo = geo._ibo;
    _vao = geo._vao;
    _valid = geo._valid;

    geo._valid = false;

    return *this;
}

void Geometry::bind()
{
    glBindVertexArray(_vao);
}

void Geometry::render(uint32_t instanceCount)
{
    if (instanceCount > 1)
    {
        glDrawElementsInstanced(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, NULL, instanceCount);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, NULL);
    }
}

VertexLayout::VertexLayout(std::vector<VertexAttribute>&& attributes, const uint16_t stride)
    : _attributes(attributes)
    , _stride(stride)
{
}

VertexLayout::VertexLayout()
    : VertexLayout({ VertexAttribute(0, 3, 0) }, 12)
{
}

void VertexLayout::bind()
{
    for (int aa = 0; aa < _attributes.size(); ++aa)
    {
        glVertexAttribPointer(_attributes.begin()[aa]._index, _attributes.begin()[aa]._size, GL_FLOAT, GL_FALSE, _stride, (void*)(_attributes.begin()[aa]._offset));
        glEnableVertexAttribArray(_attributes.begin()[aa]._index);
    }
}
