#pragma once

#include "material.h"
#include "mesh.h"
#include "gltfModel.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>

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

private:
    std::shared_ptr<Shader> vs;
    std::shared_ptr<Shader> fs;

    std::shared_ptr<ShaderProgram> program;

    std::shared_ptr<Material> material;
    std::shared_ptr<Geometry> box;

    std::shared_ptr<Mesh> boxMesh;

    std::shared_ptr<GLTFModel> _gltfModel;

    GlobalUniforms _gUniforms;
    std::shared_ptr<MaterialUniformBlock> _globalUniformBlock;

};