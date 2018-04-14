#include "texture.h"

#include <turbojpeg.h>

#include <vector>
#include <glm/glm.hpp>

using namespace jkps::gl;

Texture::Texture(std::vector<uint8_t> data, const glm::ivec2& size, GLuint format, GLuint layout)
{
	glGenTextures(1, &_textureID);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, layout, GL_UNSIGNED_BYTE, data.data());

}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, _textureID);
}
//
//std::shared_ptr<Texture> Texture::loadJPEG(uint8_t const *data, size_t size) {
//	auto handle = std::shared_ptr<void>(tjInitDecompress(), [](tjhandle handle) { tjDestroy(handle); });
//	if (!handle)
//		return nullptr;
//
//	int w, h, subsample, colorspace;
//	if (tjDecompressHeader3(handle.get(), const_cast<uint8_t *>(data), static_cast<unsigned long>(size), &w, &h, &subsample, &colorspace) != 0)
//		return nullptr;
//
//	auto jpegFormat = TJPF_RGB;
//	auto stride = w * tjPixelSize[jpegFormat];
//
//	std::vector<uint8_t> result;
//	result.resize(h * stride, 0);
//	if (tjDecompress2(handle.get(), const_cast<uint8_t *>(data), static_cast<unsigned long>(size), result.data(), w, stride, h, jpegFormat, TJFLAG_BOTTOMUP) != 0)
//		return nullptr;
//
//	auto texture = std::make_shared<Texture>(result, glm::ivec2(w, h), GL_RGB8, GL_RGB);
//	return texture;
//}
