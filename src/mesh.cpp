#include "mesh.h"


using namespace jkps::gl;

jkps::gl::Mesh::Mesh(Geometry* geometry, Material* material)
    : Mesh(std::vector<Geometry*>{ geometry }, material)
{
}

jkps::gl::Mesh::Mesh(std::vector<Geometry*> geometries, Material* material)
    : _geometries(geometries)
    , _renderMaterial(material)
{
    _modelUniformLocation = _renderMaterial->getUniformLocation("model");
}

jkps::gl::Mesh::Mesh()
    : _renderMaterial(nullptr)
{

}

void jkps::gl::Mesh::render(Material* replacementMaterial)
{
	if (replacementMaterial != nullptr)
	{
		replacementMaterial->bind();
	}
	else
	{
		_renderMaterial->bind();
	}

    for (auto geo : _geometries)
    {
        geo->bind();
        geo->render(_instanceCount);
    }

	if (replacementMaterial != nullptr)
	{
		replacementMaterial->unbind();
	}
	else
	{
		_renderMaterial->unbind();
	}
	
}

void jkps::gl::Mesh::render(const glm::mat4 & mtx, Material* replacementMaterial)
{
	if (replacementMaterial != nullptr)
	{
		replacementMaterial->setUniform(replacementMaterial->getUniformLocation("model"), mtx);
	}
	else
	{
		_renderMaterial->setUniform(_modelUniformLocation, mtx);
	}


    render(replacementMaterial);
}

void jkps::gl::Mesh::setInstances(uint32_t instances)
{
    _instanceCount = instances;
}

