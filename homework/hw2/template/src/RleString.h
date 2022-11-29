#pragma once

#include <string>

class RleString {

private:

	/// Represents one node in the linked list
	class Node {
	public:
		char symbol;  /// Symbol that is repeated one or more times
		size_t count; /// Number of repetitions of the symbol
		Node* next;   /// Next node in the linked list
	};

private:
	// TODO: Add data members here

public:
	/// Create an empty string, which does not allocate any resources
	RleString();

	///
	/// Create an RLE-encoded string from the contents of str
	///
	/// If str is the empty string, this constructor creates an empty object.
	///
	RleString(const std::string& str);

	/// Copy constructor
	RleString(const RleString& other);

	/// Copy assignment
	RleString& operator=(const RleString& other);

	/// Destructor
	~RleString();

	/// Returns the contents of the RleString as a std::string object
	std::string toString() const;

	/// Checks whether two RleStrings objects represent the same string
	bool operator==(const RleString& other) const;

	/// Checks whether this object represents the same string as that of str
	bool operator==(const std::string& str) const;

	///
	/// Returns the length of the string represented by this object
	/// 
	/// For example, size() returns 10 for the string "aaaaaaaaaa".
	///
	size_t size() const noexcept;

	///
	/// Insert value at the given index in the string
	///
	/// @param index
	///   Index of the position where the insertion should take place.
	///   Must be a value between 0 and size().
	///   If index == size(), then value is appended to the string
	///
	/// @exception std::out_of_range if index > size()
	///
	void insertAt(size_t index, char value);

	/// Remove the character at index
	/// @exception std::out_of_range if index >= size()
	void removeAt(size_t index);

	/// Reverses the contents of the string
	///
	/// For example, if the object represents the string
	/// "abcdef", after reversing, it will become "fedcba".
	void reverse();

	/// Checks whether rle is a substring of this object
	///
	/// For example:
	///   - "", "a", "aa", "ab", "aab" and "aabb" are all substrings of "aabb"
	///   - "" is a substring of ""
	///   - "abc" is a substring of "abc"
	///   - "ac" is NOT a substring of "abc"
	///   - "abc" is NOT a substring of "ab"
	bool contains(const RleString& rle) const;
};