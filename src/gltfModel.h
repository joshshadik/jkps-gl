#pragma once


#include "mesh.h"
#include "texture.h"

#include <vector>
#include <memory>
#include <string>

namespace tinygltf
{
	class Model;
	class Node;
}

namespace jkps
{
	namespace gl
	{
		class GLTFModel
		{
		public:
			GLTFModel(std::unique_ptr<tinygltf::Model> model, std::shared_ptr<ShaderProgram> overrideShader);

			static std::shared_ptr<GLTFModel> loadFromFile(const std::string&& filename, std::shared_ptr<ShaderProgram> overrideShader);

			void render( std::shared_ptr<MaterialUniformBlock> ubo, const size_t modelOffset);
			void setMatrix(const glm::mat4& mtx) { _matrix = mtx; }

		private:
			void importNode(const tinygltf::Node& node);

			void renderTreeFromNode(int nId, const glm::mat4& parentMtx, std::shared_ptr<MaterialUniformBlock> ubo, const size_t modelOffset);

		private:
			glm::mat4 _matrix;
			std::unique_ptr<tinygltf::Model> _model;

			std::vector<std::shared_ptr<Mesh>> _meshes;
			std::vector<std::shared_ptr<Geometry>> _geometries;
			std::vector<std::shared_ptr<Material>> _materials;
			std::vector<std::shared_ptr<Texture>> _textures;
		};

	}
}