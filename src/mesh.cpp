#include "mesh.h"


using namespace jkps::gl;

jkps::gl::Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : Mesh(std::vector<std::shared_ptr<Geometry>>{ geometry }, material)
{
}

jkps::gl::Mesh::Mesh(std::vector<std::shared_ptr<Geometry>> geometries, std::shared_ptr<Material> material)
    : _geometries(geometries)
    , _material(material)
{
    _modelUniformLocation = _material->getUniformLocation("model");
}

void jkps::gl::Mesh::render()
{
    _material->bind();

    for (auto geo : _geometries)
    {
        geo->bind();
        geo->render();
    }

    _material->unbind();
}

void jkps::gl::Mesh::render(const glm::mat4 & mtx)
{
    _material->setUniform(_modelUniformLocation, mtx);

    render();
}

