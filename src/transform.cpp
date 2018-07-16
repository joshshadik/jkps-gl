#include "transform.h"

jkps::engine::Transform::Transform(INodeAllocator* allocator)
    : _children(allocator)
{
}

//jkps::engine::Transform::Transform(Transform & transform)
//    : _children(transform._children)
//    , _flags(transform._flags)
//    , _parent(transform._parent)
//    , _localMatrix(transform._localMatrix)
//    , _worldMatrix( transform._worldMatrix)
//{
//}
//
//jkps::engine::Transform & jkps::engine::Transform::operator=(Transform && transform)
//{
//    this->_children = transform._children;
//    this->_flags = transform._flags;
//    this->_parent = transform._parent;
//    this->_localMatrix = transform._localMatrix;
//    this->_worldMatrix = transform._worldMatrix;
//
//    return *this;
//}

void jkps::engine::Transform::setLocal(const glm::mat4 & matrix)
{
    _localMatrix = matrix;
    _flags |= Flags::Dirty;
}

void jkps::engine::Transform::addChild(Transform * transform)
{
    transform->_parent = this;
    _children.insertBack(transform);
}

void jkps::engine::Transform::setChildren(util::NodeList<Transform*> children)
{
    _children = children;
    
    auto iter = children.iter();
    if (iter->begin())
    {
        do
        {
            iter->current()->_parent = this;
        } while (iter->next());
    }
}

void jkps::engine::Transform::update()
{
    update(false);
}

void jkps::engine::Transform::update(bool updated)
{
    updated = updated || (_flags & Flags::Dirty) == Flags::Dirty;

    if (updated)
    {
        _worldMatrix = _parent == nullptr ? _localMatrix : (_parent->_worldMatrix * _localMatrix);
    }

    auto iter = _children.iter();
    if (iter->begin())
    {
        do
        {
            iter->current()->update(updated);
        } while (iter->next());
    }
}
