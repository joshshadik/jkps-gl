#include "particleSystem.h"
#include "resourceManager.h"

#include "primitives.h"

using namespace jkps::gl;

jkps::engine::ParticleSystem::ParticleSystem()
{
}

void jkps::engine::ParticleSystem::init(gl::Geometry* geometry, gl::Material* renderMaterial, gl::Material* updateMaterial, int maxCountSqRoot)
{
    _renderMaterial = renderMaterial;
    _updateMaterial = updateMaterial;
    _maxCountSqRoot = maxCountSqRoot;

    _mesh = ResourceManager::getNextMesh();
    *_mesh = Mesh(geometry, _renderMaterial);

    _mesh->setInstances(maxCountSqRoot * maxCountSqRoot);

    _fbos[0] = ResourceManager::getNextFramebuffer();
    _fbos[1] = ResourceManager::getNextFramebuffer();

    Texture* tex0 = ResourceManager::getNextTexture();
    Texture* tex1 = ResourceManager::getNextTexture();
    Texture* tex2 = ResourceManager::getNextTexture();
    Texture* tex3 = ResourceManager::getNextTexture();

    glm::ivec2 texSize = glm::ivec2(_maxCountSqRoot, _maxCountSqRoot);
    *tex0 = Texture(texSize, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);
    *tex1 = Texture(texSize, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);
    *tex2 = Texture(texSize, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);
    *tex3 = Texture(texSize, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);

    *_fbos[0] = Framebuffer({ tex0, tex1 }, nullptr, texSize);
    *_fbos[1] = Framebuffer({ tex2, tex3 }, nullptr, texSize);

    _posTexLoc = _updateMaterial->getUniformLocation("posTex");
    _velTexLoc = _updateMaterial->getUniformLocation("velTex");
    _deltaTimeLoc = _updateMaterial->getUniformLocation("deltaTime");
    _currentTimeLoc = _updateMaterial->getUniformLocation("currentTime");
    _lifetimeLoc = _updateMaterial->getUniformLocation("lifetime");
    _originLoc = _updateMaterial->getUniformLocation("origin");
    _directionLoc = _updateMaterial->getUniformLocation("direction");
    _magnitudeLoc = _updateMaterial->getUniformLocation("magnitude");
    _randomnessLoc = _updateMaterial->getUniformLocation("randomness");

    _renderPosTexLoc = _renderMaterial->getUniformLocation("posTex");
    _renderTexSizeLoc = _renderMaterial->getUniformLocation("texSize");

    _renderMaterial->setUniform(_renderTexSizeLoc, _maxCountSqRoot);
}

void jkps::engine::ParticleSystem::update(double dt)
{
    _updateMaterial->setUniform(_posTexLoc, _fbos[_front]->color(0));
    _updateMaterial->setUniform(_velTexLoc, _fbos[_front]->color(1));

    dt = glm::min(dt, 0.034);
    _currentTime += (float)dt;

    _updateMaterial->setUniform(_deltaTimeLoc, (float)dt);
    _updateMaterial->setUniform(_currentTimeLoc, _currentTime);
    _updateMaterial->setUniform(_lifetimeLoc, _lifetime);
    _updateMaterial->setUniform(_originLoc, _origin);
    _updateMaterial->setUniform(_directionLoc, _direction);
    _updateMaterial->setUniform(_magnitudeLoc, _magnitude);
    _updateMaterial->setUniform(_randomnessLoc, _randomness);

	_fbos[_back]->bind();

    _updateMaterial->bind();

    Primitives::quad()->render();

    _updateMaterial->unbind();

	Framebuffer::bindDefault();

    _back = (_back + 1) % 2;
    _front = (_front + 1) % 2;

	_renderMaterial->setUniform(_renderPosTexLoc, _fbos[_front]->color(0));
}

void jkps::engine::ParticleSystem::render()
{

    _mesh->render(glm::mat4());
}

