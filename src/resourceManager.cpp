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

ResourceManager * jkps::engine::ResourceManager::global()
{
    return _instance;
}

Framebuffer * jkps::engine::ResourceManager::getNextFramebuffer()
{
#ifdef _DEBUG
    if( _nextFrameBuffer >= FRAMEBUFFER_COUNT)
    {
        printf("not enough framebuffer objects allocated \n");
    }
#endif
    return &_framebuffers[_nextFrameBuffer++];
}

Geometry * jkps::engine::ResourceManager::getNextGeometry()
{
#ifdef _DEBUG
    if( _nextGeometry >= GEOMETRY_COUNT)
    {
        printf("not enough geometry objects allocated \n");
    }
#endif
    return &_geometries[_nextGeometry++];
}

Material * jkps::engine::ResourceManager::getNextMaterial()
{
#ifdef _DEBUG
    if( _nextMaterial >= MATERIAL_COUNT)
    {
        printf("not enough material objects allocated \n");
    }
#endif
    return &_materials[_nextMaterial++];
}

MaterialUniformBlock * jkps::engine::ResourceManager::getNextUniformBlock()
{
#ifdef _DEBUG
    if( _nextUbo >= UBO_COUNT)
    {
        printf("not enough uniform buffer objects allocated \n");
    }
#endif
    return &_ubos[_nextUbo++];
}

Mesh * jkps::engine::ResourceManager::getNextMesh()
{
#ifdef _DEBUG
    if( _nextMesh >= MESH_COUNT)
    {
        printf("not enough mesh objects allocated \n");
    }
#endif
    return &_meshes[_nextMesh++];
}

Shader * jkps::engine::ResourceManager::getNextShader()
{
#ifdef _DEBUG
    if( _nextShader >= SHADER_COUNT)
    {
        printf("not enough shader objects allocated \n");
    }
#endif
    return &_shaders[_nextShader++];
}

ShaderProgram * jkps::engine::ResourceManager::getNextShaderProgram()
{
#ifdef _DEBUG
    if( _nextShaderProgram >= SHADERPROGRAM_COUNT)
    {
        printf("not enough shader program objects allocated \n");
    }
#endif
    return &_shaderPrograms[_nextShaderProgram++];
}

Texture * jkps::engine::ResourceManager::getNextTexture()
{
#ifdef _DEBUG
    if( _nextTexture >= TEXTURE_COUNT)
    {
        printf("not enough texture objects allocated \n");
    }
#endif
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
#ifdef _DEBUG
    if( _nextStackByte + size > STACK_BYTE_COUNT)
    {
        printf("not enough stack bytes allocated \n");
    }
#endif
    uint32_t currIndex = _nextStackByte;
    _nextStackByte += size;
    return &_stackBytes[currIndex];
}

void * jkps::engine::ResourceManager::allocNode(size_t size)
{
    return allocStack(size);
}
