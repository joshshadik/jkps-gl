#pragma once

#include "framebuffer.h"
#include "geometry.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include <array>

#ifdef CUSTOM_ALLOC

#else
#define FRAMEBUFFER_COUNT 8
#define GEOMETRY_COUNT 64
#define MATERIAL_COUNT 64
#define UBO_COUNT 64
#define MESH_COUNT 64
#define SHADER_COUNT 32
#define SHADERPROGRAM_COUNT 16
#define TEXTURE_COUNT 64
#endif

namespace jkps
{
    namespace gl
    {
        class ResourceManager
        {
        public:
            ResourceManager();

            static Framebuffer* getNextFramebuffer();
            static Geometry* getNextGeometry();
            static Material* getNextMaterial();
            static MaterialUniformBlock* getNextUniformBlock();
            static Mesh* getNextMesh();
            static Shader* getNextShader();
            static ShaderProgram* getNextShaderProgram();
            static Texture* getNextTexture();

        private:

            static ResourceManager* _instance;

            std::array<Framebuffer, FRAMEBUFFER_COUNT> _framebuffers;
            std::array<Geometry, GEOMETRY_COUNT> _geometries;
            std::array<Material, MATERIAL_COUNT> _materials;
            std::array<MaterialUniformBlock, UBO_COUNT> _ubos;
            std::array<Mesh, MESH_COUNT> _meshes;
            std::array<Shader, SHADER_COUNT> _shaders;
            std::array<ShaderProgram, SHADERPROGRAM_COUNT> _shaderPrograms;
            std::array<Texture, TEXTURE_COUNT> _textures;

            uint32_t _nextFrameBuffer = 0;
            uint32_t _nextGeometry = 0;
            uint32_t _nextMaterial = 0;
            uint32_t _nextMesh = 0;
            uint32_t _nextShader = 0;
            uint32_t _nextShaderProgram = 0;
            uint32_t _nextTexture = 0;
            uint32_t _nextUbo = 0;
        };
    }
}
