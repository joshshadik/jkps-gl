#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace jkps 
{
	namespace gl 
	{
		class Texture
		{
		public:
			Texture(std::vector<uint8_t> data, const glm::ivec2& size, GLuint format, GLuint layout);

			void bind();

			static std::shared_ptr<Texture> loadJPEG(uint8_t const *data, size_t size);

		private:
			GLuint _textureID;

		};

	}
}