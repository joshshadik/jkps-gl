#include "meshNode.h"
#include "resourceManager.h"

jkps::engine::MeshNode::MeshNode(INodeAllocator* allocator, Transform* transform)
    : _allocator(allocator)
    , _meshes(allocator)
    , _transform(transform)
{
}

jkps::engine::MeshNode::MeshNode(NodeList<Mesh*> meshes, Transform * transform)
    : _meshes(meshes)
    , _transform(transform)
{
}

void jkps::engine::MeshNode::render()
{
    auto iter = _meshes.iter();
    if (iter->begin())
    {
        do
        {
            iter->current()->render(_transform->worldMatrix());
        } while (iter->next());
    }
}

void jkps::engine::MeshNode::addMesh(Mesh * mesh)
{
    _meshes.insertBack(mesh);
}
