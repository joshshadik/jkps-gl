#pragma once

#include "material.h"
#include "shader.h"
#include "mesh.h"
#include "framebuffer.h"
#include "gltfModel.h"
#include "particleSystem.h"

#include "controls.h"


#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>

#include <emscripten.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>

using namespace jkps::gl;

struct GlobalUniforms
{
    glm::mat4 _view;
    glm::mat4 _projection;
};


class App
{
public:

    App() {}

    void init();

    void render(const glm::ivec4& viewport);

    void update(double dt);

    void setControls(Controls* controls);

    void resize(const glm::ivec2& size);

    void overrideViewProjection(const glm::mat4& view, const glm::mat4& projection);

private:
    Shader* vs;
    Shader* fs;

    Shader* composeVS;
    Shader* composeFS;

    ShaderProgram* program;
    ShaderProgram* composeProgram;

    Material* material;
    Geometry* _quadGeo;

    Mesh* boxMesh;

    GLTFModel _gltfModel;
    glm::mat4 _modelMtx;
    glm::quat _modelRot;
    glm::vec3 _modelScale;
    glm::vec3 _modelPos;

    Framebuffer* _screenBuffer;

    Material* _composeMaterial;
    Mesh* _composeMesh;

    glm::ivec2 _screenSize;

    GlobalUniforms _gUniforms;
    MaterialUniformBlock* _globalUniformBlock;
    std::vector<Texture*> _colorScreenTextures;
    Texture* _depthTexture;

    Controls* _controls;

    jkps::engine::ParticleSystem _sprayParticles;
    Material* _sprayMaterial;

};