#pragma once

#include <initializer_list>

namespace jkps
{
    namespace util
    {
        template <typename T>
        struct Node
        {
            Node(T& d) : data(d) { }

            T data;
            Node* next = nullptr;
        };

        class INodeAllocator
        {
        public:

            virtual void* allocNode(size_t) = 0;
        };

        template <typename T>
        class NodeList
        {

        public:
            class Iterator
            {
            private:
                Node<T> * _current = nullptr;
                NodeList<T>* _list = nullptr;

            public:
                Iterator(NodeList<T>* list)
                    :   _list(list)
                {
                    _current = _list->_head;
                }

                bool begin()
                {
                    _current = _list->_head;
                    return _current != nullptr;
                }

                bool next()
                {
                    _current = _current->next;
                    
                    return _current != nullptr;
                }

                T& current()
                {
                    return _current->data;
                }


                void assign(T& data)
                {
                    _current->data = data;
                }

            };

        public:
            NodeList(INodeAllocator* allocator)
                : _allocator(allocator)
                , _head(nullptr)
                , _tail(nullptr)
                , _iterator(this)
                , _size(0)
            {

            }

            NodeList(std::initializer_list<T> initList, INodeAllocator* allocator)
                : _allocator(allocator)
                , _head(nullptr)
                , _tail(nullptr)
                , _iterator(this)
                , _size(0)
            {
                for (auto& item : initList)
                {
                    insertBack(item);
                }
            }

            NodeList(const NodeList& list)
                : _allocator(list._allocator)
                , _head(list._head)
                , _tail(list._tail)
                , _iterator(this)
                , _size(list._size)
            {

            }

            NodeList& operator=(const NodeList& list)
            {
                _allocator = list._allocator;
                _head = list._head;
                _tail = list._tail;
                _iterator = Iterator(this);
                _size = list._size;

                return *this;
            }

            void insertFront(T data)
            {
#ifdef _DEBUG
                if (_allocator == nullptr)
                {
                    throw std::runtime_error("allocator is null");
                }
#endif
                Node<T>* node = static_cast<Node<T>*>(_allocator->allocNode(sizeof(Node<T>)));
                *node = Node<T>(data);
                node->next = _head;

                if (_head == nullptr)
                {
                    _tail = node;
                }
                _head = node;
                ++_size;
            }

            void insertBack(T data)
            {
#ifdef _DEBUG
                if (_allocator == nullptr)
                {
                    throw std::runtime_error("allocator is null");
                }
#endif
                Node<T>* node = static_cast<Node<T>*>(_allocator->allocNode(sizeof(Node<T>)));
                *node = Node<T>(data);
                ++_size;
                        
                if (_head == nullptr)
                {
                    _head = node;
                    _tail = node;
                    return;
                }

                _tail->next = node;
                _tail = node;                
            }

            T at(int index)
            {
                if (_iterator.begin())
                {
                    for (int i = 0; i < index; ++i)
                    {
                        if (!_iterator.next())
                        {
                            return nullptr;
                        }
                    }

                    return _iterator.current();
                }

                return nullptr;
            }

            Iterator* iter()
            {
                return &_iterator;
            }

            size_t size()
            {
                return _size;
            }

            INodeAllocator* allocator() { return _allocator; }

        private:
            INodeAllocator* _allocator;
            Node<T>* _head = nullptr;
            Node<T>* _tail = nullptr;
            Iterator _iterator;

            size_t _size;
        };


    }
}