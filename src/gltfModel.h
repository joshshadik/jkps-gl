#pragma once
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include "mesh.h"
#include "texture.h"

#include <vector>
#include <memory>
#include <string>

namespace jkps
{
	namespace gl
	{
		class GLTFModel
		{
		public:
			GLTFModel(const tinygltf::Model& model);

			static std::shared_ptr<GLTFModel> loadFromFile(const std::string&& filename);

		private:

			std::vector<std::shared_ptr<Mesh>> _meshes;
			std::vector<std::shared_ptr<Geometry>> _geometries;
			std::vector<std::shared_ptr<Material>> _materials;
			std::vector<std::shared_ptr<Texture>> _textures;
		};

	}
}