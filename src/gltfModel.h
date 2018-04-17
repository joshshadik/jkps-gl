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
        struct PbrUniforms
        {
            glm::vec4 _baseColorFactor;
            float _metallicFactor;
            float _roughnessFactor;
        };

        class GLTFModel
        {
        public:
            GLTFModel(std::unique_ptr<tinygltf::Model> model, std::shared_ptr<ShaderProgram> overrideShader);

            static std::shared_ptr<GLTFModel> loadFromFile(const std::string&& filename, std::shared_ptr<ShaderProgram> overrideShader);

            void render();
            void setMatrix(const glm::mat4& mtx) { _matrix = mtx; }

        private:
            void importNode(const tinygltf::Node& node);

            void renderTreeFromNode(int nId, const glm::mat4& parentMtx );

        private:
            typedef std::vector<std::shared_ptr<Mesh>> MeshGroup;

            glm::mat4 _matrix;
            std::unique_ptr<tinygltf::Model> _model;
            PbrUniforms _pbr;
           
            std::vector<MeshGroup> _meshGroups;
            std::vector<std::shared_ptr<Geometry>> _geometries;
            std::vector<std::shared_ptr<Material>> _materials;
            std::vector<std::shared_ptr<Texture>> _textures;
        };

    }
}