#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>


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

			VertexLayout(const std::vector<VertexAttribute>& attributes, const uint16_t stride);
			VertexLayout();

			void bind();

		private:
			uint16_t _stride;
			std::vector<VertexAttribute> _attributes;
		};

		class Geometry
		{
		public:
			typedef std::vector<std::pair<std::vector<float>, VertexLayout>> VertexData;
			typedef std::vector<uint32_t> IndexData;

			Geometry(const VertexData& vertexData, const IndexData& indices);
			~Geometry();

			void bind();

			void render();

		private:


			VertexData	_vertexData;
			IndexData	_indices;

			uint32_t _elementCount;

			std::vector<GLuint> _vbos;
			GLuint _ibo;
			GLuint _vao;

		};
	}
}