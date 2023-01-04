#include "catch2/catch_all.hpp"
#include "Node.h"

// IMPORTANT: Do not modify this file.
// If you need to add your own unit tests,
// place them in `TestAdditional.cpp`.

// A template test-case. It will run with all specified types.
// They will be available inside the test body as `TestType`. 
TEMPLATE_TEST_CASE(
	"Node::Node() constructs a leaf node with a default data",
	"[tree]",
	int, double)
{
	Node<TestType> n;

	REQUIRE(n.data == TestType());
	REQUIRE(n.left == nullptr);
	REQUIRE(n.right == nullptr);
	REQUIRE(n.isLeaf());
}

TEST_CASE("Node::Node(const T&) constructs a leaf node with a default data", "[tree]")
{
	int data;
	Node<int> n(data);

	REQUIRE(n.data == data);
	REQUIRE(n.left == nullptr);
	REQUIRE(n.right == nullptr);
	REQUIRE(n.isLeaf());
}

TEST_CASE("Node::Node(T, Node*, Node*) constructs a node with correct values", "[tree]")
{
	// Arrange
	const int someRandomValue = 123;
	Node<int> left, right;
	
	// Act
	Node<int> n(someRandomValue, &left, &right);

	// Assert
	REQUIRE(n.data == someRandomValue);
	REQUIRE(n.left == &left);
	REQUIRE(n.right == &right);
}

TEST_CASE("Node::isLeaf() returns true for leaf nodes", "[tree]")
{
	Node<int> n;
	REQUIRE(n.isLeaf());
}

TEST_CASE("Node::isLeaf() returns false for non-leaf nodes", "[tree]")
{
	Node<int> left, right;

	SECTION("Node with a single successor on the left") {
		REQUIRE_FALSE(Node<int>(42, &left, nullptr).isLeaf());
	}
	SECTION("Node with a single successor on the right") {
		REQUIRE_FALSE(Node<int>(42, nullptr, &right).isLeaf());
	}
	SECTION("Node with two successors") {
		REQUIRE_FALSE(Node<int>(42, &left, &right).isLeaf());
	}
}

TEST_CASE("Node::hasLeftSuccessor() return true when a node has a left successor", "[tree]")
{
	Node<int> left;
	Node<int> node(42, &left, nullptr);

	SECTION("For nodes with only a left successor"){
		REQUIRE(node.hasLeftSuccessor());
	}
	SECTION("For nodes with both successors") {
		Node<int> right;
		node.right = &right;
		REQUIRE(node.hasLeftSuccessor());
	}
}

TEST_CASE("Node::hasLeftSuccessor() return false when a node DOES NOT have a left successor", "[tree]")
{
	Node<int> node;

	SECTION("For leaf nodes") {
		REQUIRE_FALSE(node.hasLeftSuccessor());
	}
	SECTION("For nodes with only a right successor") {
		Node<int> right;
		node.right = &right;
		REQUIRE_FALSE(node.hasLeftSuccessor());
	}	
}

TEST_CASE("Node::hasRightSuccessor() return true when a node has a right successor", "[tree]")
{
	Node<int> right;
	Node<int> node(42, nullptr, &right);

	SECTION("For nodes with only a right successor"){
		REQUIRE(node.hasRightSuccessor());
	}
	SECTION("For nodes with both successors") {
		Node<int> left;
		node.left = &left;
		REQUIRE(node.hasRightSuccessor());
	}
}

TEST_CASE("Node::hasRightSuccessor() return false when a node DOES NOT have a right successor", "[tree]")
{
	Node<int> node;

	SECTION("For leaf nodes") {
		REQUIRE_FALSE(node.hasRightSuccessor());
	}
	SECTION("For nodes with only a left successor") {
		Node<int> left;
		node.left = &left;
		REQUIRE_FALSE(node.hasRightSuccessor());
	}	
}

TEST_CASE("Node::whichSuccessorWouldStore() returns the correct successor", "[tree]")
{
	int rootValue = 5;

	Node<int> left(rootValue - 10);
	Node<int> right(rootValue + 10);
	Node<int> root(rootValue, &left, &right);

	CHECK(root.whichSuccessorWouldStore(rootValue - 1) == &left);
	CHECK(root.whichSuccessorWouldStore(rootValue) == &right);
	CHECK(root.whichSuccessorWouldStore(rootValue + 1) == &right);
}

TEST_CASE("Node::detachSuccessors() sets both successor pointers to null", "[tree]")
{
	Node<int> left;
	Node<int> right;
	Node<int> node(42, &left, &right);

	node.detachSuccessors();

	CHECK(node.left == nullptr);
	CHECK(node.right == nullptr);
}