#pragma once

#include <vector>

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif


namespace jkps
{
    namespace gl
    {

        class VertexLayout
        {
        public:
            struct VertexAttribute
            {
                VertexAttribute(uint16_t index, uint16_t size, uint16_t offset)
                {
                    _index = index;
                    _size = size;
                    _offset = offset;
                }
                uint16_t _index;
                uint16_t _size;
                uint16_t _offset;
            };

            VertexLayout(std::vector<VertexAttribute>&& attributes, const uint16_t stride);
            VertexLayout();

            void bind();

        private:
            uint16_t _stride;
            std::vector<VertexAttribute> _attributes;
            //VertexAttribute* _attributes;
            //size_t _attributeCount;
        };

        class Geometry
        {
        public:
            typedef std::vector<std::pair<std::vector<float>, VertexLayout>> VertexData;
            typedef std::vector<uint32_t> IndexData;

            Geometry(const VertexData& vertexData, const IndexData& indices);
            Geometry();
            ~Geometry();
            Geometry(Geometry&& geo);
            Geometry& operator=(Geometry&& geo);

            void bind();

            void render(uint32_t instanceCount = 1);

        private:
            Geometry(const Geometry&) = delete;
            Geometry& operator=(Geometry const &) = delete;

            VertexData	_vertexData;
            IndexData	_indices;

            uint32_t _elementCount;

            std::vector<GLuint> _vbos;
            GLuint _ibo;
            GLuint _vao;

            bool _valid = true;
        };
    }
}