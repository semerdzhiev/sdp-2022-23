#include "catch2/catch_all.hpp"
#include "Allocator.h"

TEST_CASE("DebugAllocator allocates and releases correctly", "[allocator]")
{
    DebugAllocator<int> da;
    std::vector<int*> allocations;
    const int count = 5;

    CHECK(da.allocationsCount() == 0);

    for(int i = 1; i <= count; ++i) {
        allocations.push_back(da.buy());
        CHECK(da.allocationsCount() == i);
        CHECK(da.totalAllocationsCount() == i);
    }

    for(int i = count-1; i >= 0; --i) {
        CHECK_NOTHROW(da.release(allocations[i]));
        CHECK(da.allocationsCount() == i); // The number of active allocations decreasesclear
        
        CHECK(da.totalAllocationsCount() == count); // No new allocations are made
    }
}

TEST_CASE("DebugAllocator::release() throws when trying to release a pointer not allocated by it", "[allocator]")
{
    DebugAllocator<int> da;
    int* ptr = new int;
    CHECK_THROWS_AS(da.release(ptr), std::invalid_argument);
    delete ptr;
}

TEST_CASE("DebugAllocator::release() does nothing when releasing null", "[allocator]")
{
    DebugAllocator<int> da;
    int* ptr = da.buy();

    CHECK_NOTHROW(da.release(nullptr));
    
    CHECK(da.allocationsCount() == 1);
    CHECK_NOTHROW(da.release(ptr));
}