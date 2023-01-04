#pragma once

#include "Allocator.h"
#include "NodeIterator.h"
#include "NodeOperations.h"

template <typename T>
using SimpleNodeAllocator = SimpleAllocator<Node<T>>;

template <typename T>
using DebugNodeAllocator = DebugAllocator<Node<T>>;

template <
    typename ElementType,
    typename AllocatorType = SimpleNodeAllocator<ElementType>,
    typename NodeOperations = RecursiveNodeOperations<ElementType>
    >
class BinarySearchTree {

    using NodeType = Node<ElementType>;

    NodeType* m_rootptr = nullptr;
    size_t m_size = 0;
    AllocatorType m_allocator;

public:
    class Iterator {
        NodeIterator<ElementType> it;

    public:
        Iterator(NodeType* startFrom)
            : it(startFrom)
        {            
        }

        int& operator*()
        {
            return it->data;
        }

        int* operator->()
        {
            return &it->data;
        }

        void operator++()
        {
            ++it;
        }

        bool operator==(const Iterator& other)
        {
            return it == other.it;
        }

        bool operator!=(const Iterator& other)
        {
            return it != other.it;
        }
    };

public:
    BinarySearchTree() = default;

    ~BinarySearchTree()
    {
        clear();
    }

    BinarySearchTree(const BinarySearchTree& other)
    {
        m_rootptr = NodeOperations::clone(other.m_rootptr, m_allocator);
        m_size = other.m_size;
    }

    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        if(this != &other) {
            clear();
            m_rootptr = NodeOperations::clone(other.m_rootptr, m_allocator);
            m_size = other.m_size;
        }
        return *this;
    }

    void clear()
    {
        NodeOperations::release(m_rootptr, m_allocator);
        m_rootptr = nullptr;
        m_size = 0;
    }

    size_t size() const noexcept
    {
        return m_size;
    }

    bool empty() const noexcept
    {
        return m_size == 0;
    }

    const AllocatorType& allocator() const
    {
        return m_allocator;
    }

    bool contains(const ElementType& value)
    {
        return NodeOperations::findPointerTo(value, m_rootptr) != nullptr;
    }

    void insert(const ElementType& value)
    {
        NodeType* ptr = m_allocator.buy();
        ptr->data = value;

        NodeOperations::insert(m_rootptr, *ptr);
        ++m_size;
    }

    void erase(const ElementType& value)
    {
        Node<ElementType>* extracted = NodeOperations::extract(m_rootptr, value);
        
        if(extracted != nullptr) {
            --m_size;
            m_allocator.release(extracted);
        }
    }

    bool operator==(const BinarySearchTree& other) const
    {
        return NodeOperations::sameTrees(this->m_rootptr, other.m_rootptr);
    }

    Iterator beginIterator()
    {
        return Iterator(m_rootptr);
    }

    Iterator endIterator()
    {
        return Iterator(nullptr);
    }
};