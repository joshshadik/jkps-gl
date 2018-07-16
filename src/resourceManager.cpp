#include "resourceManager.h"

using namespace jkps::engine;
using namespace jkps::gl;

ResourceManager* ResourceManager::_instance = nullptr;

jkps::engine::ResourceManager::ResourceManager()
{
    // first one created will be default
    if (_instance == nullptr)
    {
        _instance = this;
    }
#ifdef _DEBUG
    int sz = sizeof(ResourceManager);
    printf("allocated resources size: %d bytes \n", sz);
#endif
}

ResourceManager * jkps::engine::ResourceManager::default()
{
    return _instance;
}

Framebuffer * jkps::engine::ResourceManager::getNextFramebuffer()
{
    return &_framebuffers[_nextFrameBuffer++];
}

Geometry * jkps::engine::ResourceManager::getNextGeometry()
{
    return &_geometries[_nextGeometry++];
}

Material * jkps::engine::ResourceManager::getNextMaterial()
{
    return &_materials[_nextMaterial++];
}

MaterialUniformBlock * jkps::engine::ResourceManager::getNextUniformBlock()
{
    return &_ubos[_nextUbo++];
}

Mesh * jkps::engine::ResourceManager::getNextMesh()
{
    return &_meshes[_nextMesh++];
}

Shader * jkps::engine::ResourceManager::getNextShader()
{
    return &_shaders[_nextShader++];
}

ShaderProgram * jkps::engine::ResourceManager::getNextShaderProgram()
{
    return &_shaderPrograms[_nextShaderProgram++];
}

Texture * jkps::engine::ResourceManager::getNextTexture()
{
    return &_textures[_nextTexture++];
}

Transform * jkps::engine::ResourceManager::getNextTransform()
{
    Transform* trans = static_cast<Transform*>(allocStack(sizeof(Transform)));
    *trans = Transform(this);
    return trans;
}


void * jkps::engine::ResourceManager::allocStack(size_t size)
{
    uint32_t currIndex = _nextStackByte;
    _nextStackByte += size;
    return &_stackBytes[currIndex];
}

void * jkps::engine::ResourceManager::allocNode(size_t size)
{
    return allocStack(size);
}
