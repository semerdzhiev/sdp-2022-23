#pragma once

#include <cassert>
#include <stack>

#include "Allocator.h"
#include "Node.h"
#include "NodeIterator.h"

///
/// Recursive implementation of basic BST operations
///
template <typename T>
class RecursiveNodeOperations {
public:
    using NodeType = Node<T>;

    ///
    /// Checks whether two trees have the same structure and node values
    ///
    static bool sameTrees(const Node<T>* a, const Node<T>* b)
    {
        if(a == nullptr || b == nullptr)
            return a == b;

        return
            a->data == b->data &&
            sameTrees(a->left, b->left) &&
            sameTrees(a->right, b->right);
    }

    ///
    /// Returns a reference to the pointer to which a given value is (or would be) attached
    ///
    /// If `value` is present in a node N in the tree, the function returns
    /// a reference to the pointer, which refers to N.
    /// If `value` is stored in the root element, `startFrom` itself will be returned.
    /// Otherwise the return value is a reference to a `left` or `right` member
    /// of a `Node` object,
    /// If `value` is not present in the tree, the function returns a reference
    /// to that position in the tree, where it should be inserted.
    /// 
    static Node<T>*& findPointerTo(const T& value, Node<T>*& startFrom)
    {
        if(startFrom == nullptr || startFrom->data == value)
            return startFrom;

        return findPointerTo(value, startFrom->whichSuccessorWouldStore(value));
    }

    ///
    /// Insert `node` into the tree with root pointed by `rootptr`
    ///
    /// `node` itself will be inserted and no copy will be created.
    ///
    static void insert(Node<T>*& rootptr, Node<T>& node)
    {
        findPointerTo(node.data, rootptr) = &node;
    }

    ///
    /// Returns a reference to the pointer which refers the largest element in the tree
    ///
    /// If the largest element is in the root, then the function may return
    /// `startFrom` itself.
    /// Otherwise the value returned is either a `left`, or a `right` member
    /// of a `Node` object in the tree.
    ///
    /// @note
    ///   The node N that contains the largest element is either a leaf
    ///   or a node with only a left successor.
    ///   If it had a right successor, then its value would have been
    ///   greater than that in N and thus N could not have been the
    ///   largest one in the tree.
    ///
    static Node<T>*& findPointerToLargest(Node<T>*& startFrom)
    {
        return
            (startFrom == nullptr || startFrom->right == nullptr) ?
            startFrom :
            findPointerToLargest(startFrom->right);
    }    

    ///
    /// Extracts the node which contains a given value from a tree
    ///
    /// If there is no such node in the tree it will not be modified.
    /// Otherwise the first such node is located and extracted.
    /// The extracted node will have both its successor pointers
    /// set to null.
    ///
    static Node<T>* extract(Node<T>*& rootptr, const T& data)
    {
        Node<T>* result = nullptr;
        Node<T>*& parentPtr = findPointerTo(data, rootptr);

        if(parentPtr == nullptr) {
            // No such node is present in the tree. Nothing to do.
        }
        else if (parentPtr->hasLeftSuccessor()) {
            // The extracted nodes has a left successor and maybe
            // a right one too.
            result = parentPtr;
            Node<T>*& ptrToPromoted = findPointerToLargest(parentPtr->left);
            Node<T>* promoted = ptrToPromoted;

            // Cannot be null, because (1) parentPtr is not a leaf node
            // and also (2) has at least one successor on the left
            assert(promoted != nullptr);

            ptrToPromoted = promoted->left;
            parentPtr = promoted;
            promoted->left = result->left;
            promoted->right = result->right;

            result->detachSuccessors();
        }
        else {
            // Handles both the cases when the extracted node
            // is a leaf and when it has only a right successor
            result = parentPtr;
            parentPtr = parentPtr->right;
            result->detachSuccessors();
        }

        return result;
    }

    ///
    /// Release the memory allocated for a given tree
    /// @tparam AllocatorType 
    /// @param startFrom Pointer to the root element of the tree
    /// @param allocator Allocator which will be used to release the occupied memory
    ///
    template <typename AllocatorType>
    static void release(NodeType* startFrom, AllocatorType& allocator)
    {
        if(startFrom) {
            release(startFrom->left, allocator);
            release(startFrom->right, allocator);
            allocator.release(startFrom);
        }
    }

    ///
    /// Clone a tree
    /// @tparam AllocatorType 
    /// @param startFrom Pointer to the root element of the tree to be cloned
    /// @param allocator Allocator which will be used to obtain memory for the nodes of the new tree
    /// @return A pointer to the root of the newly cloned tree
    /// @exception std::bad_alloc if memory allocation fails
    ///
    template <typename AllocatorType>
    static NodeType* clone(NodeType* startFrom, AllocatorType& allocator)
    {
        NodeType* result = nullptr;

        if(startFrom) {
            result = allocator.buy();
            result->data = startFrom->data;

            NodeType* leftTree  = nullptr;
            NodeType* rightTree = nullptr;

            try {
                leftTree  = clone(startFrom->left, allocator);
                rightTree = clone(startFrom->right, allocator);
            }
            catch(std::bad_alloc&) {
                release(leftTree, allocator);
                release(rightTree, allocator);
                throw;
            }

            result->left  = leftTree;
            result->right = rightTree;
        }

        return result;
    }    
};

///
/// Iterative implementation of basic BST operations
///
template <typename T>
class IterativeNodeOperations {
public:
    using NodeType = Node<T>;

    /// @copydoc RecursiveNodeOperations::sameTrees
    static bool sameTrees(const Node<T>* a, const Node<T>* b)
    {
        std::stack<const Node<T>*> st;
        st.push(a);
        st.push(b);

        while(!st.empty())
        {
            assert(st.size() %2 == 0);

            const Node<T>* nodeFromB = st.top();
            st.pop();
            const Node<T>* nodeFromA = st.top();
            st.pop();

            if(nodeFromA == nullptr && nodeFromB == nullptr) {
                continue;
            }
            else if((nodeFromA == nullptr) ^ (nodeFromB == nullptr)) {
                return false;
            }
            else if(nodeFromA->data != nodeFromB->data) {
                return false;
            }
            else {
                st.push(nodeFromA->left);
                st.push(nodeFromB->left);
                st.push(nodeFromA->right);
                st.push(nodeFromB->right);
            }
        }
        return true;
    }

    /// @copydoc RecursiveNodeOperations::findPointerTo
    static Node<T>*& findPointerTo(const T& value, Node<T>*& startFrom)
    {
        Node<T>** result = &startFrom;

        while(*result != nullptr && (*result)->data != value)
            result = &(*result)->whichSuccessorWouldStore(value);

        return *result;
    }

    /// @copydoc RecursiveNodeOperations::insert
    static void insert(Node<T>*& rootptr, Node<T>& node)
    {
        findPointerTo(node.data, rootptr) = &node;
    }

    /// @copydoc RecursiveNodeOperations::findPointerToLargest
    static Node<T>*& findPointerToLargest(Node<T>*& startFrom)
    {
        Node<T>** result = &startFrom;

        while(*result != nullptr && (*result)->right != nullptr)
            result = &(*result)->right;

        return *result;
    }

    /// @copydoc RecursiveNodeOperations::extract
    static Node<T>* extract(Node<T>*& rootptr, const T& data)
    {
        Node<T>* result = nullptr;
        Node<T>*& parentPtr = findPointerTo(data, rootptr);

        if(parentPtr == nullptr) {
            // No such node is present in the tree. Nothing to do.
        }
        else if (parentPtr->hasLeftSuccessor()) {
            // The extracted nodes has a left successor and maybe
            // a right one too.
            result = parentPtr;
            Node<T>*& ptrToPromoted = findPointerToLargest(parentPtr->left);
            Node<T>* promoted = ptrToPromoted;

            // Cannot be null, because (1) parentPtr is not a leaf node
            // and also (2) has at least one successor on the left
            assert(promoted != nullptr);

            ptrToPromoted = promoted->left;
            parentPtr = promoted;
            promoted->left = result->left;
            promoted->right = result->right;

            result->detachSuccessors();
        }
        else {
            // Handles both the cases when the extracted node
            // is a leaf and when it has only a right successor
            result = parentPtr;
            parentPtr = parentPtr->right;
            result->detachSuccessors();
        }

        return result;
    }

    /// @copydoc RecursiveNodeOperations::release
    template <typename AllocatorType>
    static void release(NodeType* startFrom, AllocatorType& allocator)
    {
        std::stack<NodeType*> st;
        
        if(startFrom)
            st.push(startFrom);

        while(! st.empty() ) {
            NodeType* ptr = st.top();
            st.pop();

            if(ptr->left)
                st.push(ptr->left);
            if(ptr->right)
                st.push(ptr->right);

            allocator.release(ptr);
        }
    }

    /// @copydoc RecursiveNodeOperations::clone
    template <typename AllocatorType>
    static NodeType* clone(NodeType* startFrom, AllocatorType& allocator)
    {
        //TODO implement iterative cloning
        throw std::exception();
    }
};


