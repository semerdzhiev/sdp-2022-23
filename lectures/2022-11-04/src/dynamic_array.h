#pragma once

template <typename T>
class DynamicArray {

  T* data = nullptr;
  size_t allocated = 0;
  size_t used = 0;

public:
  /// @brief Constructs an empty array wirth zero capacity
  DynamicArray()
  {}

  /// @brief Constructs an array with size and capacity equal to initialSize
  /// @exception std::bad_alloc Memory allocation failed
  DynamicArray(size_t initialCapacity)
    : data(new T[initialCapacity]),
      used(initialCapacity),
      allocated(initialCapacity)
  {}

  /// @brief Size of the array
  size_t size() const noexcept {
    return used;
  }

  /// @brief Size of the underlying buffer
  size_t capacity() const noexcept {
    return allocated;
  }

};