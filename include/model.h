#pragma once

#include "meshNode.h"
#include "nodeList.h"

using namespace jkps::util;

namespace jkps
{
    namespace engine
    {
        class Model
        {
        public:
            typedef util::NodeList<MeshNode> MeshList;

            enum Layer
            {
                Opaque = 1,
                Transparent = 2,
                Opaque_Transparent = 3
            };

            Model(INodeAllocator* allocator, Transform* root);
            Model(std::initializer_list<std::pair<Layer, MeshList>> meshGroupNodes, INodeAllocator* allocator, Transform* root);

            void update(float dt);
            void render(uint32_t layerFlags = ~0);

            void addMesh(MeshNode& meshNode, Layer layer);

            Transform* root() { return _root; }

        private:
            util::NodeList<std::pair<Layer, MeshList>> _meshGroups;


            Transform* _root;
        };
    }
}