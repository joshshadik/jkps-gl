#include "mesh.h"

#include <iostream>

using namespace jkps::gl;

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material) 
	: _geometry(geometry)
	, _material(material)
{
}

void Mesh::render()
{
	_material->bind();

	_geometry->bind();
	_geometry->render();
}

