#pragma once

#include "generic_array_container.hpp"

namespace gero {

template <typename PublicType,
          typename InternalType = PublicType,
          std::size_t _Capacity = 0,
          typename Allocator = std::allocator<InternalType>>
class fixed_size_array : public generic_array_container<InternalType, PublicType> {

  using value_type = PublicType;
  using internal_type = InternalType;
  using base_class = generic_array_container<PublicType, InternalType>;

protected:
  using traits_t = std::allocator_traits<Allocator>; // The matching trait

protected:
  // // allocate memory only, will not call any constructor
  // internal_type* allocate_memory(std::size_t size) {
  //   Allocator alloc;
  //   internal_type* newData = alloc.allocate(size);
  //   return newData;
  // }

  // // deallocate memory only, will not call any destructor
  // void deallocate_memory(internal_type* data, std::size_t size) {
  //   Allocator alloc;
  //   alloc.deallocate(data, size);
  // }

  // call the move constructor only, do not allocate memory
  void call_constructor(internal_type* dataPtr) {
    Allocator alloc;
    traits_t::construct(alloc, dataPtr);
  }

  // call the move constructor only, do not allocate memory
  void call_copy_constructor(internal_type* dataPtr, const internal_type& value) {
    Allocator alloc;
    traits_t::construct(alloc, dataPtr, value);
  }

  // call the move constructor only, do not allocate memory
  void call_move_constructor(internal_type* dataPtr, internal_type&& value) {
    Allocator alloc;
    traits_t::construct(alloc, dataPtr, std::move(value));
  }

  // call the move constructor only, do not allocate memory
  template <typename... Args> internal_type& emplace_move_constructor(internal_type* dataPtr, Args&&... args) {
    Allocator alloc;
    traits_t::construct(alloc, dataPtr, std::forward<Args>(args)...);
    return *dataPtr;
  }

  // call the destructor only, do not deallocate memory
  void call_destructor(internal_type* dataPtr) {
    Allocator alloc;
    traits_t::destroy(alloc, dataPtr);
  }

private:
  // InternalType _static_data[_Capacity * sizeof(InternalType)];
  uint8_t* _static_data[_Capacity * sizeof(InternalType)];

public:
  fixed_size_array() {
    this->_size = 0;
    this->_data = reinterpret_cast<InternalType*>(this->_static_data);
  }

  // disable copy
  fixed_size_array(const fixed_size_array& other) = delete;
  fixed_size_array& operator=(const fixed_size_array& other) = delete;
  // disable copy

  // disable move
  fixed_size_array(fixed_size_array&& other) = delete;
  fixed_size_array& operator=(fixed_size_array&& other) = delete;
  // disable move

  virtual ~fixed_size_array() { clear(); }

public:
  // may reallocate
  void push_back(const value_type& value) {
    if (this->_size == _Capacity)
      D_THROW(std::runtime_error, "full capacity reached");

    call_copy_constructor(this->_data + this->_size, value);

    ++this->_size;
  }

  // may reallocate
  void push_back(value_type&& value) {
    if (this->_size == _Capacity)
      D_THROW(std::runtime_error, "full capacity reached");

    call_move_constructor(this->_data + this->_size, std::move(value));

    ++this->_size;
  }

  // may reallocate
  template <typename... Args> value_type& emplace_back(Args&&... args) {
    if (this->_size == _Capacity)
      D_THROW(std::runtime_error, "full capacity reached");

    value_type& result = emplace_move_constructor(this->_data + this->_size, std::forward<Args>(args)...);

    ++this->_size;

    return result;
  }

public:
  void pop_back() {
    if (this->_size == 0)
      D_THROW(std::runtime_error, "empty size");

    --this->_size;

    call_destructor(this->_data + this->_size);
  }

  // no reallocation
  uint32_t unsorted_erase(std::size_t inIndex) {
    if (this->is_out_of_range(inIndex))
      return 0;

    uint32_t totalSwapped = 0;

    // swap data at the end
    if (this->_size > 1 && inIndex < this->_size - 1) {
      std::swap(this->_data[inIndex], this->_data[this->_size - 1]);
      ++totalSwapped;
    }

    // remove the back
    pop_back();
    return totalSwapped;
  }

  // no reallocation
  uint32_t sorted_erase(std::size_t inIndex) {
    if (this->is_out_of_range(inIndex))
      return 0;

    uint32_t totalSwapped = 0;

    if (this->_size > 1 && inIndex + 1 < this->_size) {

      // swap data at the end
      for (std::size_t ii = inIndex; ii + 1 < this->_size; ++ii) {
        std::swap(this->_data[ii], this->_data[ii + 1]);
        ++totalSwapped;
      }
    }

    // remove the back
    pop_back();

    return totalSwapped;
  }

public:
  void clear() {
    for (std::size_t ii = 0; ii < this->_size; ++ii)
      call_destructor(this->_data + ii);

    this->_size = 0;
  }

public:
  constexpr std::size_t capacity() const { return _Capacity; }
};

} // namespace gero
