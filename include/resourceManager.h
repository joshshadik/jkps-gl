#pragma once

#include "framebuffer.h"
#include "geometry.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "nodeList.h"
#include "transform.h"

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
#define STACK_BYTE_COUNT 8192
#endif

using namespace jkps::gl;
using namespace jkps::util;

namespace jkps
{
    namespace engine
    {
        class ResourceManager : public INodeAllocator
        {
        public:
            ResourceManager();

            static ResourceManager* default();

            Framebuffer* getNextFramebuffer();
            Geometry* getNextGeometry();
            Material* getNextMaterial();
            MaterialUniformBlock* getNextUniformBlock();
            Mesh* getNextMesh();
            Shader* getNextShader();
            ShaderProgram* getNextShaderProgram();
            Texture* getNextTexture();
            Transform* getNextTransform();

            void* allocStack(size_t size);
            void* allocNode(size_t size);

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

            std::array<uint8_t, STACK_BYTE_COUNT> _stackBytes;

            uint32_t _nextFrameBuffer = 0;
            uint32_t _nextGeometry = 0;
            uint32_t _nextMaterial = 0;
            uint32_t _nextMesh = 0;
            uint32_t _nextShader = 0;
            uint32_t _nextShaderProgram = 0;
            uint32_t _nextTexture = 0;
            uint32_t _nextUbo = 0;
            uint32_t _nextTransform = 0;
            uint32_t _nextNode = 0;

            uint32_t _nextStackByte = 0;
        };

    }
}
