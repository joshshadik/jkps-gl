#pragma once

#include "material.h"
#include "mesh.h"
#include "framebuffer.h"
#include "gltfModel.h"

#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>

#include <emscripten.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

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
    void init();

    void render();

    void resize(const glm::ivec2& size);

private:
    std::shared_ptr<Shader> vs;
    std::shared_ptr<Shader> fs;

    std::shared_ptr<ShaderProgram> program;

    std::shared_ptr<Material> material;
    std::shared_ptr<Geometry> _quadGeo;

    std::shared_ptr<Mesh> boxMesh;

    std::shared_ptr<GLTFModel> _gltfModel;

    std::shared_ptr<Framebuffer> _screenBuffer;

    std::shared_ptr<Material> _composeMaterial;
    std::shared_ptr<Mesh> _composeMesh;

    glm::ivec2 _screenSize;

    GlobalUniforms _gUniforms;
    std::shared_ptr<MaterialUniformBlock> _globalUniformBlock;

};