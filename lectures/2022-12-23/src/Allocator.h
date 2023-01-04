#pragma once

#include <unordered_set>
#include <stdexcept>

template <typename T>
class SimpleAllocator {
public: 
    T* buy()
    {
        return new T();
    }

    void release(T* ptr)
    {
        delete ptr;
    }
};

template <typename T>
class DebugAllocator {
    std::unordered_set<T*> allocations;
    size_t totalAllocations = 0;

public:
    T* buy()
    {
        T* newItem = new T();
        ++totalAllocations;
        return *allocations.insert(newItem).first;
    }

    void release(T* ptr)
    {
        if( ! ptr ) // do nothing when ptr == nullptr
            return;

        if(allocations.count(ptr) == 0)
            throw std::invalid_argument("Trying to release a pointer not returned by this allocator");

        delete ptr;
        allocations.erase(ptr);
    }

    size_t allocationsCount() const noexcept
    {
        return allocations.size();
    }

    size_t totalAllocationsCount() const noexcept
    {
        return totalAllocations;
    }
};