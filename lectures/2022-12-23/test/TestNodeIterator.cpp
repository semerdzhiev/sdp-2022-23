#include "catch2/catch_all.hpp"
#include "NodeIterator.h"
#include "SampleTree.h"


TEST_CASE("NodeIterator::NodeIterator(nullptr) creates an iterator that has reached the end", "[tree]")
{
    NodeIterator<int> it(nullptr);
    CHECK(it.atEnd());
}

TEST_CASE("NodeIterator walks the tree in the correct order and returns correct values", "[tree]")
{
    SampleTree tree;
    NodeIterator<SampleTree::ValueType> it(tree.rootptr);
    NodeIterator<SampleTree::ValueType> end(nullptr);

    int i = 0;

    // Values are retrieved in the correct order
    for(; it != end; ++it,++i) {
        CHECK(it->data == tree.values[i]);
        CHECK((*it).data == tree.values[i]);
        CHECK_FALSE(it.atEnd());
    }

    CHECK(i == tree.values.size()); // All elements have been visited
}

TEST_CASE("NodeIterators pointing at the same node are equal", "[tree]")
{
    SampleTree tree;
    NodeIterator<SampleTree::ValueType> it1(tree.rootptr);
    NodeIterator<SampleTree::ValueType> it2(tree.rootptr);
    NodeIterator<SampleTree::ValueType> end(nullptr);

    for(; it1 != end; ++it1,++it2) {
        CHECK(it1 == it2);
        CHECK_FALSE(it1 != it2);
    }

    CHECK(it1 == it2);
    CHECK_FALSE(it1 != it2);
    CHECK(it1 == end);
    CHECK_FALSE(it1 != end);
}