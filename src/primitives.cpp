#include "primitives.h"
#include "resourceManager.h"

using namespace jkps::gl;
using namespace jkps::engine;

Geometry* jkps::engine::Primitives::_quad = nullptr;

void jkps::engine::Primitives::init()
{
    _quad = ResourceManager::default()->getNextGeometry();

    std::vector<float> quadVertices{
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
    };

    std::vector<float> quadTexcoords{
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    std::vector<uint32_t> quadVertexIndices{
        0, 1, 2,
        0, 2, 3
    };


    VertexLayout vertLayout{
        { VertexLayout::VertexAttribute(Shader::StandardAttrLocations["POSITION"], 3, 0) },
        3 * sizeof(GLfloat)
    };
    VertexLayout texcoordLayout{
        { VertexLayout::VertexAttribute(Shader::StandardAttrLocations["TEXCOORD_0"], 2, 0) },
        2 * sizeof(GLfloat)
    };

    Geometry::VertexData vertexData = { std::make_pair(quadVertices, vertLayout), std::make_pair(quadTexcoords, texcoordLayout) };
    *_quad = Geometry(vertexData, quadVertexIndices);
}
