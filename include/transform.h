#pragma once

#include "nodeList.h"

#include <glm/glm.hpp>
#include <vector>

using namespace jkps::util;

namespace jkps
{
    namespace engine
    {
        class Transform
        {
        public:
            Transform(INodeAllocator* allocator);
            //Transform(Transform& transform);
            //Transform& operator=(Transform&& transform);

            void setLocal(const glm::mat4& matrix);
            void addChild(Transform* transform);

            void setChildren(util::NodeList<Transform*> children);

            void update();

            glm::mat4& worldMatrix() { return _worldMatrix; }
            glm::mat4& localMatrix() { return _localMatrix; }

        private:
            enum Flags
            {
                Dirty = 1
            };

            void update(bool updated);

            glm::mat4 _localMatrix;
            glm::mat4 _worldMatrix;

            uint32_t _flags = 1;

            util::NodeList<Transform*> _children;
            Transform* _parent = nullptr;

        };
    }
}