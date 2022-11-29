#include "catch2/catch_all.hpp"
#include "RleString.h"

#include <stdexcept>

class OriginalAndExpectedFixture {
public:
	const std::string original;
	const std::string expected;
	RleString rlestr;

	OriginalAndExpectedFixture(std::string original, std::string expected)
		: original(original), expected(expected), rlestr(original)
	{}

	void checkRleStringIsAsExpected()
	{
		CHECK(rlestr == expected);
		CHECK(rlestr.size() == expected.size());
		CHECK(rlestr.toString() == expected);
	}
};

void checkEmpty(const RleString& rlestr)
{
	CHECK(rlestr.size() == 0);
	CHECK(rlestr == "");
	CHECK(rlestr.toString() == "");
}

TEST_CASE("RleString::RleString constructs an empty string")
{
	checkEmpty(RleString());
}

TEST_CASE("RleString::RleString(const std::string&) constructs an empty string when passed the empty string")
{
	checkEmpty(RleString(""));
}

TEST_CASE("RleString::RleString(const std::string&) constructs a string with proper length")
{
	OriginalAndExpectedFixture fx("abc", "abc");
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::operator==(const RleString&) The empty RleString string is equal to the empty RleString string")
{
	REQUIRE(RleString() == RleString());
}

TEST_CASE("RleString::operator==(const RleString&) Two RleStrings created from the same string are equal")
{
	std::string original("abc");
	RleString a(original), b(original);
	REQUIRE(a == b);
}

TEST_CASE("RleString::operator==(const RleString&) Two RleStrings with equal contents, but created differently, are equal")
{
	RleString a("aabc");
	a.removeAt(0);

	RleString b("bc");
	b.insertAt(0, 'a');

	REQUIRE(a == b);
}

TEST_CASE("RleString::operator==(const std::string&) The empty RleString string is equal to an empty std::string")
{
	REQUIRE(RleString() == std::string());
}

TEST_CASE("RleString::operator==(const std::string&) A non-empty RleString created with several steps is equal to an std::string with the same contents")
{
	OriginalAndExpectedFixture fx("bcd", "abc");
	
	fx.rlestr.removeAt(fx.original.size() - 1); // remove the trailing character
	fx.rlestr.insertAt(0, 'a'); // insert an 'a' at the beginning

	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::RleString(const RleString&) correctly copies the empty string")
{
	RleString empty;
	RleString copy(empty);
	checkEmpty(copy);
}

TEST_CASE("RleString::RleString(const RleString&) correctly copies a non-empty string")
{
	std::string original("abc");
	RleString rlestr(original);
	RleString copy(rlestr);

	REQUIRE(rlestr == copy);
	REQUIRE(rlestr.toString() == original);
	REQUIRE(rlestr.size() == original.size());
}

TEST_CASE("RleString::operator=(const RleString&) correctly copies the empty string")
{
	RleString empty;
	RleString copy;
	copy = empty;
	checkEmpty(copy);
}

TEST_CASE("RleString::RleString(const std::string&) correctly copies a non-empty string")
{
	std::string original("abc");
	RleString rlestr(original);
	RleString copy;

	copy = rlestr;
	
	REQUIRE(rlestr == copy);
	REQUIRE(rlestr.toString() == original);
	REQUIRE(rlestr.size() == original.size());
}

TEST_CASE("RleString::insertAt() correctly inserts elements in the empty string")
{
	RleString rlestr;
	rlestr.insertAt(0, 'a');
	REQUIRE(rlestr == "a");
	REQUIRE(rlestr.size() == 1);
}

TEST_CASE("RleString::insertAt() throws when called for an index greater than zero in the empty string")
{
	RleString empty;
	REQUIRE_THROWS_AS(empty.insertAt(1, 'a'), std::out_of_range);
}

TEST_CASE("RleString::insertAt() throws when called for an index greater than the size of the string")
{
	RleString rlestr("abcd");
	REQUIRE_THROWS_AS(rlestr.insertAt(rlestr.size() + 1, 'a'), std::out_of_range);
}

TEST_CASE("RleString::insertAt() throws when the index is greater than the size of the string")
{
	std::string original("abc");
	RleString rlestr(original);
	REQUIRE_THROWS_AS(rlestr.insertAt(original.size()+1, 'x'), std::out_of_range);
}

TEST_CASE("RleString::insertAt() works correctly for the empty string")
{
	OriginalAndExpectedFixture fx("", "a");
	fx.rlestr.insertAt(0, 'a');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts at the beginning of the string when a new node should be added")
{
	OriginalAndExpectedFixture fx("bc", "abc");
	fx.rlestr.insertAt(0, 'a');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts at the beginning of the string when a new node should NOT be added")
{
	OriginalAndExpectedFixture fx("abc", "aabc");
	fx.rlestr.insertAt(0, 'a');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts at the end of the string when a new node should be added")
{
	OriginalAndExpectedFixture fx("ab", "abc");
	fx.rlestr.insertAt(fx.original.size(), 'c');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts at the end of the string when a new node should NOT be added")
{
	OriginalAndExpectedFixture fx("abc", "abcc");
	fx.rlestr.insertAt(fx.original.size(), 'c');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts in the middle of the string when a new node should be added")
{
	OriginalAndExpectedFixture fx("ac", "abc");
	fx.rlestr.insertAt(1, 'b');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::insertAt() correctly inserts in the middle of the string when a new node should NOT be added")
{
	OriginalAndExpectedFixture fx("abc", "abbc");
	fx.rlestr.insertAt(1, 'b');
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() throws when called for the empty string")
{
	RleString empty;
	REQUIRE_THROWS_AS(empty.removeAt(0), std::out_of_range);
}

TEST_CASE("RleString::removeAt() throws when given an index outside of the string")
{
	const std::string original("abc");
	RleString rlestr(original);
	REQUIRE_THROWS_AS(rlestr.removeAt(original.size()), std::out_of_range);
}

TEST_CASE("RleString::removeAt() removing the last character from a RleString makes it empty")
{
	RleString rlestr("a");
	rlestr.removeAt(0);
	checkEmpty(rlestr);
}

TEST_CASE("RleString::removeAt() properly removes symbols at the beginning of the string when a node should be deleted")
{
	OriginalAndExpectedFixture fx("abc", "bc");
	fx.rlestr.removeAt(0);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() properly removes symbols at the beginning of the string when a node should NOT be deleted")
{
	OriginalAndExpectedFixture fx("aabc", "abc");
	fx.rlestr.removeAt(0);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() properly removes symbols at the end of the string when a node should be deleted")
{
	OriginalAndExpectedFixture fx("abc", "ab");
	fx.rlestr.removeAt(fx.original.size()-1);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() properly removes symbols at the end of the string when a node should NOT be deleted")
{
	OriginalAndExpectedFixture fx("abcc", "abc");
	fx.rlestr.removeAt(fx.original.size() - 1);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() properly removes symbols in the middle of the string when a node should be deleted")
{
	OriginalAndExpectedFixture fx("abc", "ac");
	fx.rlestr.removeAt(1);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::removeAt() properly removes symbols  in the middle of the string when a node should NOT be deleted")
{
	OriginalAndExpectedFixture fx("abbc", "abc");
	fx.rlestr.removeAt(1);
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::reverse() does not alter the empty string")
{
	RleString empty;
	empty.reverse();
	checkEmpty(empty);
}

TEST_CASE("RleString::reverse() does not alter one-symbol strings")
{
	OriginalAndExpectedFixture fx("a", "a");
	fx.rlestr.reverse();
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::reverse() properly reverses strings")
{
	OriginalAndExpectedFixture fx("aaaabbccaa", "aaccbbaaaa");
	fx.rlestr.reverse();
	fx.checkRleStringIsAsExpected();
}

TEST_CASE("RleString::contains() The empty string is contained within the empty string")
{
	RleString empty;
	REQUIRE(empty.contains(RleString()));
}

TEST_CASE("RleString::contains() The empty string is contained within any non-empty string")
{
	RleString empty;
	RleString nonempty("test");
	REQUIRE(nonempty.contains(empty));
}

TEST_CASE("RleString::contains() The empty string does NOT contain a non-empty string")
{
	RleString empty;
	RleString nonempty("test");
	REQUIRE_FALSE(empty.contains(nonempty));
}

TEST_CASE("RleString::contains() A non-empty string is contained within itself")
{
	RleString haystack("test");
	RleString needle("test");
	REQUIRE(haystack.contains(needle));
}

TEST_CASE("RleString::contains() Proper prefixes are recognized correctly")
{
	RleString haystack("aabbcc");
	RleString needle("aab");
	REQUIRE(haystack.contains(needle));
}

TEST_CASE("RleString::contains() Proper suffixes are recognized correctly")
{
	RleString haystack("aabbcc");
	RleString needle("bcc");
	REQUIRE(haystack.contains(needle));
}

TEST_CASE("RleString::contains() Proper infixes are recognized correctly")
{
	RleString haystack("aabbcc");
	RleString needle("abbc");
	REQUIRE(haystack.contains(needle));
}

TEST_CASE("RleString::contains() 'scattered' characters are NOT considered as a substring")
{
	RleString haystack("abc");
	RleString needle("ac");
	REQUIRE_FALSE(haystack.contains(needle));
}