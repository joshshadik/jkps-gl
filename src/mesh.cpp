#include "mesh.h"


using namespace jkps::gl;

jkps::gl::Mesh::Mesh(Geometry* geometry, Material* material)
    : Mesh(std::vector<Geometry*>{ geometry }, material)
{
}

jkps::gl::Mesh::Mesh(std::vector<Geometry*> geometries, Material* material)
    : _geometries(geometries)
    , _material(material)
{
    _modelUniformLocation = _material->getUniformLocation("model");
}

jkps::gl::Mesh::Mesh()
    : _material(nullptr)
{

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

