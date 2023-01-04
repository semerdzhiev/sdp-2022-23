#include "catch2/catch_all.hpp"
#include "Solution.h"
#include <vector>

// IMPORTANT: Do not modify this file.
// If you need to add your own unit tests,
// place them in `TestAdditional.cpp`.

//
//     A:1
//    /   \
//   B:2   C:3
//  /   \    \ 
// D:4  E:5  F:6
//
class SampleNonBstBalancedTree {
public:
    Node<int> f = Node<int>(6); 
    Node<int> e = Node<int>(5); 
    Node<int> d = Node<int>(4); 
    Node<int> c = Node<int>(3, nullptr, &f); 
    Node<int> b = Node<int>(2, &d, &e); 
    Node<int> a = Node<int>(1, &b, &c);
    Node<int>* rootptr = &a; 
};

//
//     A:5
//    /   \
//   B:2   C:7
//        /   \ 
//       D:5  E:9
//           
class SampleUnbalancedBst {
public:
    Node<int> e = Node<int>(9); 
    Node<int> d = Node<int>(5); 
    Node<int> c = Node<int>(7, &d, &e); 
    Node<int> b = Node<int>(2); 
    Node<int> a = Node<int>(5, &b, &c);
    Node<int>* rootptr = &a;

    const int levelsCount = 3;
    std::vector<std::vector<int>> levels { {5}, {2, 7}, {5, 9} };

    std::vector<int> inorder { 2, 5, 5, 7, 9 };    
};

//
//     A:5
//    /   \
//   B:2   C:7
//  /     /   \ 
// D:5   E:5  F:9
//           
class NonBstWithSameElementOnTheLeft {
public:
    Node<int> f = Node<int>(9);
    Node<int> e = Node<int>(5); 
    Node<int> d = Node<int>(5); 
    Node<int> c = Node<int>(7, &e, &f);
    Node<int> b = Node<int>(2, &d, nullptr); 
    Node<int> a = Node<int>(5, &b, &c);
    Node<int>* rootptr = &a;
};

//
//     A:5
//    /   \
//   B:2   C:7
//        /   \ 
//       D:0  E:9
//           
class NonBstWithElementLesserThanTheRootInTheRightSubtree {
public:
    Node<int> e = Node<int>(9); 
    Node<int> d = Node<int>(0); 
    Node<int> c = Node<int>(7, &d, &e); 
    Node<int> b = Node<int>(2); 
    Node<int> a = Node<int>(5, &b, &c);
    Node<int>* rootptr = &a;
};

TEST_CASE("isBalanced() returns true for empty trees")
{
    REQUIRE(isBalanced<int>(nullptr));
}

TEST_CASE("isBalanced() returns true for balanced, non-empty trees")
{
    SampleNonBstBalancedTree t;
    REQUIRE(isBalanced(t.rootptr));
}

TEST_CASE("isBalanced() returns false for non-balanced trees")
{
    SampleUnbalancedBst t;
    REQUIRE_FALSE(isBalanced(t.rootptr));
}

TEST_CASE("isBst() returns true for empty trees")
{
    REQUIRE(isBst<int>(nullptr));
}

TEST_CASE("isBst() returns true for non-empty BSTs")
{
    SampleUnbalancedBst t;
    REQUIRE(isBst(t.rootptr));
}

TEST_CASE("isBst() returns false for trees that are not BSTs")
{
    SECTION("Balanced tree, which is not BST") {
        SampleNonBstBalancedTree t;
        REQUIRE_FALSE(isBst(t.rootptr));
    }
    SECTION("When the left subtree contains an element equal to the root") {
        NonBstWithSameElementOnTheLeft t;
        REQUIRE_FALSE(isBst(t.rootptr));
    }
    SECTION("When there is a value lesser than the root in the right subtree") {
        NonBstWithElementLesserThanTheRootInTheRightSubtree t;
        REQUIRE_FALSE(isBst(t.rootptr));
    }
}

TEST_CASE("toBalanced() returns an empty tree for empty arrays")
{
    Node<int>* result = toBalanced(std::vector<int>());
    REQUIRE(result == nullptr);
}

TEST_CASE("toBalanced() returns a correctly constructed tree")
{
    SECTION("Unsorted array results in a balanced tree") {
        std::vector<int> sample{ 1, 10, 1, 56 };
        Node<int>* result = toBalanced(sample);
        CHECK(result != nullptr);
        CHECK(isBalanced(result));
        CHECK_NOTHROW(release(result));
    }
    SECTION("Sorted array with no repetitions results in a balanced BST") {
        std::vector<int> sample{ -1, 0, 1, 3, 5, 7, 10, 100 };
        Node<int>* result = toBalanced(sample);
        CHECK(result != nullptr);
        CHECK(isBalanced(result));
        CHECK(isBst(result));
        CHECK_NOTHROW(release(result));
    }
}

TEST_CASE("toVector() returns an empty array for empty trees")
{
    std::vector<int> result = toVector<int>(nullptr);
    REQUIRE(result.size() == 0);
}

TEST_CASE("toVector() returns the elements of a tree in correct order")
{
    SampleUnbalancedBst t;
    REQUIRE(toVector(t.rootptr) == t.inorder);
}

TEST_CASE("Applying toBalanced() and toVector() in sequence reproduces the original array (i.e. inverse functions)")
{
    std::vector<int> original{ 1, 10, 1, 56 };
    Node<int>* root = toBalanced(original);
    std::vector<int> restored = toVector(root);
    CHECK(original == restored);
    CHECK_NOTHROW(release(root));
}

TEST_CASE("level() returns an empty array for levels outside of the scope of the tree")
{
    SampleUnbalancedBst t;

    SECTION("Negative levels") {
        REQUIRE(level(t.rootptr, -1).size() == 0);
    }
    SECTION("Level greater than the index of the last valid level in the tree") {
        REQUIRE(level(t.rootptr, t.levelsCount).size() == 0);
    }
}

TEST_CASE("level() correctly returns the levels of a tree")
{
    SampleUnbalancedBst t;

    for(int i = 0; i < t.levels.size(); ++i) {
        std::vector<int> result = level(t.rootptr, i);
        REQUIRE(t.levels[i] == result);
    }
}
