#pragma once

#include "mesh.h"
#include "transform.h"

#include "nodeList.h"

using namespace jkps::gl;

namespace jkps
{
    namespace engine
    {
        class MeshNode
        {
        public:
            MeshNode(INodeAllocator* allocator, Transform* transform);
            MeshNode(NodeList<Mesh*> meshes, Transform* transform);

            void render(Material* replacementMaterial = nullptr);

            void addMesh(Mesh* mesh);

            util::NodeList<Mesh*>* meshes() { return &_meshes; }
            Transform* transform() { return _transform; }

        private:
            INodeAllocator * _allocator;

            util::NodeList<Mesh*> _meshes;
            Transform* _transform;
        };
    }
}
