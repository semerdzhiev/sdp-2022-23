#pragma once

template <typename T>
class Node {
public:
    T data = T();
    Node* left = nullptr;
    Node* right = nullptr;

    Node()
    {        
    }

    Node(const T& data)
        : data(data)
    {        
    }

    Node(const T& data, Node* left, Node* right)
        : data(data), left(left), right(right)
    {
    }

    bool isLeaf() const noexcept
    {
        return left == nullptr && right == nullptr;
    }

    bool hasLeftSuccessor() const noexcept
    {
        return left != nullptr;
    }

    bool hasRightSuccessor() const noexcept
    {
        return right != nullptr;
    }

    ///
    /// Returns a reference to the successor pointer to which `value` belongs
    ///
    /// If `value` is less than the value stored in this node,
    /// then the function returns a reference to the `left` data member.
    /// Otherwise it returns a reference to `right`.
    ///
    Node*& whichSuccessorWouldStore(const T& value)
    {
        return (value < data) ? left : right;
    }

    /// Set both successor pointers to null, effectively making this node a leaf
    void detachSuccessors()
    {
        left = nullptr;
        right = nullptr;
    }
};