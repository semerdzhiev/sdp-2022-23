#pragma once
#include "Node.h"
#include <array>

//
// Sample tree
//
// The values are picked in such way, as to have a difference of at least 2
// between a node and all other nodes in its left or right subtrees.
// This is used by tests that need to generate values to insert in the tree
// at appropriate positions.
//
//    A:50
//   /   \
//  /     \
// B:-3    C:70
//  \     / \
//  D:30 /   \
//      E:60 F:90
//           /
//         G:80
//
class SampleTree
{
public:
	using ValueType = int;

	Node<int> g = Node<int>(80);
	Node<int> e = Node<int>(60);
	Node<int> d = Node<int>(30);
	Node<int> f = Node<int>(90, &g, nullptr);
	Node<int> c = Node<int>(70, &e, &f);
	Node<int> b = Node<int>(-3, nullptr, &d);
	Node<int> a = Node<int>(50, &b, &c);
	
	Node<int>& root = a;
	Node<int>* rootptr = &a;

	const int valueNotInTheTree = 1'000'000;

	std::array<int,7> values{-3, 30, 50, 60, 70, 80, 90};
};