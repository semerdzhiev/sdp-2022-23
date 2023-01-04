#include "catch2/catch_all.hpp"
#include "NodeOperations.h"
#include "SampleTree.h"

using TreeOperationTypes = std::tuple<
	IterativeNodeOperations<int>,
	RecursiveNodeOperations<int>
>;

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::sameTrees() returns true for two empty trees",
	"[tree]",
	TreeOperationTypes)
{
	REQUIRE(TestType::sameTrees(nullptr, nullptr));
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::sameTrees() returns false when comparing an empty and a non-empty tree",
	"[tree]",
	TreeOperationTypes)
{
	Node<int> n;
	SECTION("Left is non-empty, right is empty") {
		REQUIRE_FALSE(TestType::sameTrees(&n, nullptr));
	}
	SECTION("Left is empty, right is non-empty") {
		REQUIRE_FALSE(TestType::sameTrees(nullptr, &n));
	}
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::sameTrees() returns true when comparing equivalent trees",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t1, t2;
	REQUIRE(TestType::sameTrees(&t1.root, &t2.root));
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::sameTrees() returns false when comparing different trees",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t1, t2;

	int value = t1.g.data - 1; // A value which is slightly less than that of G, so that it should be placed to its left
	Node<int> x(value); // Create a new node to store the value
	t2.g.left = &x; // Attach the new node to the left of G

	REQUIRE_FALSE(TestType::sameTrees(&t1.root, &t2.root));
}

template <typename T>
inline bool areSameObject(const T& a, const T&b)
{
	return &a == &b;
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerTo() returns the starting pointer when searching in an empty tree",
	"[tree]",
	TreeOperationTypes)
{
	Node<int>* rootptr = nullptr;
	CHECK( areSameObject(rootptr, TestType::findPointerTo(0, rootptr)) );
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerTo() returns the starting pointer when searching in a single-node tree",
	"[tree]",
	TreeOperationTypes)
{
	int someRandomValue = 42; // A random value
	Node<int> node(someRandomValue);
	Node<int>* rootptr = nullptr;

	CHECK( areSameObject(rootptr, TestType::findPointerTo(someRandomValue, rootptr)) );
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerTo() locates the pointer to a node with specific value in the tree.",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;

	CHECK( areSameObject(t.rootptr, TestType::findPointerTo(t.a.data, t.rootptr)));
	CHECK( areSameObject(t.a.left,  TestType::findPointerTo(t.b.data, t.rootptr)));
	CHECK( areSameObject(t.a.right, TestType::findPointerTo(t.c.data, t.rootptr)));
	CHECK( areSameObject(t.b.right, TestType::findPointerTo(t.d.data, t.rootptr)));
	CHECK( areSameObject(t.c.left,  TestType::findPointerTo(t.e.data, t.rootptr)));
	CHECK( areSameObject(t.c.right, TestType::findPointerTo(t.f.data, t.rootptr)));
	CHECK( areSameObject(t.f.left,  TestType::findPointerTo(t.g.data, t.rootptr)));
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerTo() returns the correct position where a node should be inserted when it is not present in the tree",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;

	// Values are being inserted around each node which has one or more missing successors.
	// The values being inserted are ±1 from the value stored in the respective node.
	// Thus they should be attached exactly under the said node, to the left or to the right.
	CHECK( areSameObject(t.b.left,  TestType::findPointerTo(t.b.data-1, t.rootptr)));
	CHECK( areSameObject(t.d.left,  TestType::findPointerTo(t.d.data-1, t.rootptr)));
	CHECK( areSameObject(t.d.right, TestType::findPointerTo(t.d.data+1, t.rootptr)));
	CHECK( areSameObject(t.e.left,  TestType::findPointerTo(t.e.data-1, t.rootptr)));
	CHECK( areSameObject(t.e.right, TestType::findPointerTo(t.e.data+1, t.rootptr)));
	CHECK( areSameObject(t.f.right, TestType::findPointerTo(t.f.data+1, t.rootptr)));
	CHECK( areSameObject(t.g.left,  TestType::findPointerTo(t.g.data-1, t.rootptr)));
	CHECK( areSameObject(t.g.right, TestType::findPointerTo(t.g.data+1, t.rootptr)));
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::insert() correctly inserts into an empty tree",
	"[tree]",
	TreeOperationTypes)
{
	Node<int>* rootptr = nullptr;
	Node<int> node;

	TestType::insert(rootptr, node);

	REQUIRE(rootptr == &node);
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::insert() correctly inserts into a non-empty tree",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;
	SampleTree expected;

	SECTION("Left neighbour of D") {
		Node<int> node(t.d.data-1);
		expected.d.left = &node;

		TestType::insert(t.rootptr, node);

		REQUIRE(t.d.left == &node); // Check whether is is exactly the same node that is attached
		REQUIRE(TestType::sameTrees(t.rootptr, expected.rootptr)); // Check that the tree structure remains the same
	}
	SECTION("Right neighbour of F") {
		Node<int> node(t.f.data+1);
		expected.f.right = &node;

		TestType::insert(t.rootptr, node);

		REQUIRE(t.f.right == &node); // Check whether is is exactly the same node that is attached
		REQUIRE(TestType::sameTrees(t.rootptr, expected.rootptr)); // Check that the tree structure remains the same
	}

	// TODO: Add more sections to cover additional cases
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerToLargest() returns the root pointer for an empty tree",
	"[tree]",
	TreeOperationTypes)
{
	Node<int>* rootptr = nullptr;
	REQUIRE( areSameObject(rootptr, TestType::findPointerToLargest(rootptr)) );
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerToLargest() returns the root pointer for a single-node tree",
	"[tree]",
	TreeOperationTypes)
{
	Node<int> node;
	Node<int>* rootptr = &node;
	REQUIRE( areSameObject(rootptr, TestType::findPointerToLargest(rootptr)) );

}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::findPointerToLargest() correctly locates the largest element",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;
	REQUIRE( areSameObject(t.c.right, TestType::findPointerToLargest(t.rootptr)) );
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::extract() does nothing and returns nullptr for an empty tree",
	"[tree]",
	TreeOperationTypes)
{
	int someRandomValue = 0;
	Node<int>* rootptr = nullptr;
	Node<int>* result = TestType::extract(rootptr, someRandomValue);

	REQUIRE(rootptr == nullptr);
	REQUIRE(result == nullptr);
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::extract() results in an empty tree when removing the only element from a single-node tree",
	"[tree]",
	TreeOperationTypes)
{
	const int someRandomValue = 42;
	Node<int> node(someRandomValue);
	Node<int> *rootptr = &node;

	Node<int>* extracted = TestType::extract(rootptr, someRandomValue);

	REQUIRE(extracted == &node);
	REQUIRE(rootptr == nullptr);
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::extract() does not alter the tree when the value is not present in it",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;
	SampleTree expected;

	Node<int>* extracted = TestType::extract(t.rootptr, t.valueNotInTheTree);

	REQUIRE(extracted == nullptr);
	REQUIRE(TestType::sameTrees(expected.rootptr, t.rootptr));
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::extract() correctly extracts nodes and leaves the tree with the expected structure",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree t;
	SampleTree expected;

	SECTION("Removing the root") {
		expected.rootptr = &expected.d;
		expected.d.left = &expected.b;
		expected.d.right = &expected.c;
		expected.b.right = nullptr;

		Node<int>* extracted = TestType::extract(t.rootptr, t.a.data);
		
		CHECK(areSameObject(*extracted, t.a)); // The correct node is extracted
		CHECK(extracted->left == nullptr);     // The left pointer of the extracted node has been set to null
		CHECK(extracted->right == nullptr);    // The right pointer of the extracted node has been set to null
		CHECK(areSameObject(*t.rootptr, t.d)); // The correct node has been promoted to root
		CHECK(areSameObject(*t.d.left,  t.b)); // The left pointer of the promoted node is correct
		CHECK(areSameObject(*t.d.right, t.c)); // The right pointer of the promoted node is correct
		CHECK(TestType::sameTrees(expected.rootptr, t.rootptr)); // The structure of the tree is as expected
	}
	SECTION("Removing a leaf") {
		expected.f.left = nullptr;

		Node<int>* extracted = TestType::extract(t.rootptr, t.g.data);
		
		REQUIRE(areSameObject(*extracted, t.g)); // The correct node is extracted
		REQUIRE(extracted->left == nullptr);     // The left pointer of the extracted node has been set to null
		REQUIRE(extracted->right == nullptr);    // The right pointer of the extracted node has been set to null
		REQUIRE(areSameObject(*t.rootptr, t.a)); // The root remains the same
		REQUIRE(t.f.left == nullptr); // The old parent of the extracted node has its successor pointer set to null
		REQUIRE(TestType::sameTrees(expected.rootptr, t.rootptr)); // The structure of the tree is as expected
	}
	SECTION("Removing a node in the middle") {
		expected.a.right = &expected.e;
		expected.e.right = &expected.f;
		
		Node<int>* extracted = TestType::extract(t.rootptr, t.c.data);
		
		REQUIRE(areSameObject(*extracted, t.c)); // The correct node is extracted
		REQUIRE(extracted->left == nullptr);     // The left pointer of the extracted node has been set to null
		REQUIRE(extracted->right == nullptr);    // The right pointer of the extracted node has been set to null
		REQUIRE(areSameObject(*t.rootptr, t.a)); // The root remains the same
		REQUIRE(areSameObject(*t.a.right, t.e)); // check that the correct node is promoted in the place of the extracted one
		REQUIRE(t.e.left == nullptr);            // The left pointer of the promoted element is correct
		REQUIRE(areSameObject(*t.e.right, t.f)); // The right pointer of the promoted element is correct
		REQUIRE(TestType::sameTrees(expected.rootptr, t.rootptr)); // The structure of the tree is as expected
	}
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::release() does not throw on nullptr",
	"[tree]",
	TreeOperationTypes)
{
	DebugAllocator<Node<int>> da;
	Node<int>* rootptr = nullptr;

	CHECK_NOTHROW(TestType::release(rootptr, da));
	CHECK(da.allocationsCount() == 0);
	CHECK(da.totalAllocationsCount() == 0);
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::release() correctly releases memory",
	"[tree]",
	TreeOperationTypes)
{
	DebugAllocator<Node<int>> da;
	
	//
	// Builds a tree of the following type:
	//
	//      *
	//     / \
	//    *   *
	//   /     \
	//  *       *
	//
	Node<int>* rootptr = da.buy();
	rootptr->left = da.buy();
	rootptr->right = da.buy();
	rootptr->left->left = da.buy();
	rootptr->right->right = da.buy();

	size_t totalElements = 5;

	REQUIRE(da.allocationsCount() == totalElements);
	REQUIRE(da.totalAllocationsCount() == totalElements);

	TestType::release(rootptr, da);

	CHECK(da.allocationsCount() == 0); // All nodes have been released
	CHECK(da.totalAllocationsCount() == totalElements); // No new allocations were made
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::clone() returns nullptr for an empty tree and no allocations are made",
	"[tree]",
	TreeOperationTypes)
{
	DebugAllocator<Node<int>> da;
	Node<int>* rootptr = TestType::clone(nullptr, da);

	CHECK(rootptr == nullptr);
	CHECK(da.allocationsCount() == 0);
	CHECK(da.totalAllocationsCount() == 0);
}

TEMPLATE_LIST_TEST_CASE(
	"TreeOperation::clone() correctly clones a tree and makes only the necessary number of allocations",
	"[tree]",
	TreeOperationTypes)
{
	SampleTree tree;
	DebugAllocator<Node<int>> da;
	Node<int>* cloned = TestType::clone(tree.rootptr, da);

	CHECK(TestType::sameTrees(cloned, tree.rootptr));
	CHECK(da.allocationsCount() == tree.values.size());
	CHECK(da.totalAllocationsCount() == tree.values.size());
}