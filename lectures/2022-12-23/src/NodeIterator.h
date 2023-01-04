#pragma once

#include "Node.h"

#include <cassert>
#include <stack>
#include <vector>

template <typename T>
class NodeIterator {
public:
    using NodeType = Node<T>;

private:
    std::stack<NodeType*> backtrack;

private:
    void pushAllTheWayToTheLeft(NodeType* startFrom)
    {
        for(; startFrom; startFrom = startFrom->left)
            backtrack.push(startFrom);
    }


public:
    NodeIterator(NodeType* startFrom)
    {
        pushAllTheWayToTheLeft(startFrom);
    }
    
    bool atEnd() const
    {
        return backtrack.empty();
    }

    NodeType& operator*()
    {
        assert( ! atEnd() );
        return *backtrack.top();
    }

    NodeType* operator->()
    {
        assert( ! atEnd() );
        return backtrack.top();
    }
    
    void operator++()
    {
        assert( ! atEnd() );
        NodeType* p = backtrack.top();
        backtrack.pop();
        pushAllTheWayToTheLeft(p->right);
    }

    bool operator==(const NodeIterator& other) const
    {
        if(atEnd() || other.atEnd())
            return atEnd() == other.atEnd();

        return backtrack.top() == other.backtrack.top();
    }

    bool operator!=(const NodeIterator& other) const
    {
        return ! operator==(other);
    }
};
