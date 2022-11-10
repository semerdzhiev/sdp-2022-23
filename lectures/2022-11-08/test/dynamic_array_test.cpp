#include "catch2/catch_all.hpp"

#include "dynamic_array.h"

TEST_CASE("DynamicArray() constructs an array with size and capcity equal to 0")
{
  DynamicArray<int> obj;
  REQUIRE(obj.size() == 0);
  REQUIRE(obj.capacity() == 0);
}

TEST_CASE("DynamicArray(size_t) constructs an object with correct size and capacity")
{
  size_t size = 100;
  DynamicArray<int> obj(size);

  REQUIRE(obj.size() == size);
  REQUIRE(obj.capacity() == size);
}

TEST_CASE("DynamicArray(size_t) throws when memory allocation fails")
{
  REQUIRE_THROWS_AS(DynamicArray<int>(1'000'000'000'000), std::bad_alloc);
}