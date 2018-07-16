#include "model.h"

jkps::engine::Model::Model(INodeAllocator * allocator, Transform* root)
    : _meshGroups(allocator)
    , _root(root)
{
}

jkps::engine::Model::Model(std::initializer_list<std::pair<Layer, MeshList>> meshGroupNodes, INodeAllocator * allocator, Transform* root)
    : _meshGroups(meshGroupNodes, allocator)
    , _root(root)
{
}

void jkps::engine::Model::update(float dt)
{
    _root->update();
}

void jkps::engine::Model::render(uint32_t layerFlags)
{
    auto iter = _meshGroups.iter();
    if (iter->begin())
    {
        do
        {
            auto& curr = iter->current();
            if (((uint32_t)curr.first & layerFlags) != 0)
            {
                auto meshIter = curr.second.iter();
                if (meshIter->begin())
                {
                    do
                    {
                        meshIter->current().render();
                    } while (meshIter->next());
                }
            }
            
        } while (iter->next());
    }
}

void jkps::engine::Model::addMesh(MeshNode & meshNode, Layer layer)
{
    auto iter = _meshGroups.iter();
    if (iter->begin())
    {
        do
        {
            auto& curr = iter->current();
            if (curr.first == layer)
            {
                curr.second.insertBack(meshNode);
                return;
            }

        } while (iter->next());
    }


    _meshGroups.insertBack(std::make_pair(layer, MeshList({ meshNode }, _meshGroups.allocator())));
}
