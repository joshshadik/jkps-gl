#pragma once


#include "mesh.h"
#include "texture.h"

#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"


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
            GLTFModel() {}
            GLTFModel(tinygltf::Model&& model, ShaderProgram* overrideShader);
            static bool loadFromFile( GLTFModel* model, const std::string&& filename, ShaderProgram* overrideShader);

            void render();
            void setMatrix(const glm::mat4& mtx) { _matrix = mtx; }

        private:
            void importNode(const tinygltf::Node& node);

            void renderTreeFromNode(int nId, const glm::mat4& parentMtx );

        private:
            typedef std::vector<Mesh> MeshGroup;

            glm::mat4 _matrix;
            tinygltf::Model _model;
            PbrUniforms _pbr;
           
            std::vector<MeshGroup> _meshGroups;
            std::vector<Geometry> _geometries;
            std::vector<Material> _materials;
            std::vector<Texture> _textures;
            std::vector<MaterialUniformBlock> _ubos;
        };

    }
}