#include "resourceManager.h"

using namespace jkps::gl;

ResourceManager* ResourceManager::_instance = nullptr;

jkps::gl::ResourceManager::ResourceManager()
{
    _instance = this;
#ifdef _DEBUG
    int sz = sizeof(ResourceManager);
    printf("allocated resources size: %d bytes \n", sz);
#endif
}

Framebuffer * jkps::gl::ResourceManager::getNextFramebuffer()
{
    return &_instance->_framebuffers[_instance->_nextFrameBuffer++];
}

Geometry * jkps::gl::ResourceManager::getNextGeometry()
{
    return &_instance->_geometries[_instance->_nextGeometry++];
}

Material * jkps::gl::ResourceManager::getNextMaterial()
{
    return &_instance->_materials[_instance->_nextMaterial++];
}

MaterialUniformBlock * jkps::gl::ResourceManager::getNextUniformBlock()
{
    return &_instance->_ubos[_instance->_nextUbo++];
}

Mesh * jkps::gl::ResourceManager::getNextMesh()
{
    return &_instance->_meshes[_instance->_nextMesh++];
}

Shader * jkps::gl::ResourceManager::getNextShader()
{
    return &_instance->_shaders[_instance->_nextShader++];
}

ShaderProgram * jkps::gl::ResourceManager::getNextShaderProgram()
{
    return &_instance->_shaderPrograms[_instance->_nextShaderProgram++];
}

Texture * jkps::gl::ResourceManager::getNextTexture()
{
    return &_instance->_textures[_instance->_nextTexture++];
}
