
#pragma once

#include "basic_double_linked_list.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <functional>
#include <memory>
#include <string>

// will make it survive a container reallocation/destruction
// #define D_REF_TRACKER_ITERATORS

//
//
//
//
//

//
//
//
//
//

//
//
//
//
//

namespace gero {

template <typename generic_array_container>
class generic_array_container_base_iterator

#ifdef D_REF_TRACKER_ITERATORS
  : public basic_double_linked_list::link
#endif

{

  friend generic_array_container;

public:
  // -> https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/

  using value_type = typename generic_array_container::value_type;

  using iterator_category = std::random_access_iterator_tag;
  // using value_type = int;
  using difference_type = int;
  using pointer = value_type*;
  using reference = value_type&;

  // // using difference_type = typename std::iterator<std::random_access_iterator_tag, value_type>::difference_type;
  // using difference_type = int64_t;

protected:
  generic_array_container* _container;
  int _index;

public:
  generic_array_container_base_iterator(generic_array_container& container, int index)
    : _container(&container), _index(index) {

#ifdef D_REF_TRACKER_ITERATORS
    basic_double_linked_list::add_link_to_list(_container->_iterators_list, *this);
#endif
  }

  // COPY
  generic_array_container_base_iterator(const generic_array_container_base_iterator& other) {
    if (&other == this)
      return;

    _container = other._container;
    _index = other._index;

#ifdef D_REF_TRACKER_ITERATORS
    if (_container != nullptr) {
      basic_double_linked_list::add_link_to_list(_container->_iterators_list, *this);
    }
#endif
  }

  // MOVE
  generic_array_container_base_iterator(generic_array_container_base_iterator&& other) {
    if (&other == this)
      return;

    std::swap(_container, other._container);
    std::swap(_index, other._index);

#ifdef D_REF_TRACKER_ITERATORS
    if (_container != nullptr) {
      basic_double_linked_list::swap_two_links_from_same_list(_container->_iterators_list, other, *this);
    }
#endif
  }

  // COPY
  generic_array_container_base_iterator& operator=(const generic_array_container_base_iterator& other) {
    if (&other == this)
      return *this;

    _container = other._container;
    _index = other._index;

#ifdef D_REF_TRACKER_ITERATORS
    if (_container != nullptr) {
      basic_double_linked_list::add_link_to_list(_container->_iterators_list, *this);
    }
#endif

    return *this;
  }

  // MOVE
  generic_array_container_base_iterator& operator=(generic_array_container_base_iterator&& other) {
    if (&other == this)
      return *this;

    std::swap(_container, other._container);
    std::swap(_index, other._index);

#ifdef D_REF_TRACKER_ITERATORS
    if (_container != nullptr) {
      basic_double_linked_list::swap_two_links_from_same_list(_container->_iterators_list, other, *this);
    }
#endif

    return *this;
  }

  ~generic_array_container_base_iterator() {

#ifdef D_REF_TRACKER_ITERATORS
    if (_container)
      basic_double_linked_list::remove_link_from_list(_container->_iterators_list, *this);
#endif
  }

public:
  bool is_valid() const { return _container != nullptr; }

protected:
  void _ensure_is_valid() const {
    if (!is_valid())
      D_THROW(std::runtime_error, "invalid iterator");
  }

public:
  bool operator==(const generic_array_container_base_iterator& rhs) const {
    return (_container == rhs._container && _index == rhs._index);
  }
  bool operator<(const generic_array_container_base_iterator& rhs) const { return _index < rhs._index; }

#if 0
  bool operator!=(const generic_array_container_base_iterator& rhs) const { return !(*this == rhs); }
  bool operator>(const generic_array_container_base_iterator& rhs) const { return _index > rhs._index; }
  bool operator<=(const generic_array_container_base_iterator& rhs) const { return _index <= rhs._index; }
  bool operator>=(const generic_array_container_base_iterator& rhs) const { return _index >= rhs._index; }
#else
  bool operator!=(const generic_array_container_base_iterator& rhs) const {
    return !generic_array_container_base_iterator::operator==(rhs);
  }
  bool operator>(const generic_array_container_base_iterator& rhs) const {
    return (!generic_array_container_base_iterator::operator<(rhs) &&
            !generic_array_container_base_iterator::operator==(rhs));
  }
  bool operator<=(const generic_array_container_base_iterator& rhs) const {
    return !generic_array_container_base_iterator::operator>(rhs);
  }
  bool operator>=(const generic_array_container_base_iterator& rhs) const {
    return !generic_array_container_base_iterator::operator<(rhs);
  }
#endif

public:
  difference_type operator-(const generic_array_container_base_iterator& rhs) const { return _index - rhs._index; }
  difference_type operator+(const generic_array_container_base_iterator& rhs) const { return _index + rhs._index; }

  generic_array_container_base_iterator& operator+=(difference_type rhs) {
    _index += rhs;
    return *this;
  }
  generic_array_container_base_iterator& operator-=(difference_type rhs) {
    _index -= rhs;
    return *this;
  }
};

//
//
//
//
//

//
//
//
//
//

//
//
//
//
//

template <typename generic_array_container>
class generic_array_container_iterator : public generic_array_container_base_iterator<generic_array_container> {

  friend generic_array_container;

public:
  using base_type = generic_array_container_base_iterator<generic_array_container>;
  using value_type = typename base_type::value_type;

public:
  generic_array_container_iterator(generic_array_container& container, int index) : base_type(container, index) {}

public:
  value_type& operator[](int index) {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index + index];
  }
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index + index];
  }
  value_type* operator->() {
    base_type::_ensure_is_valid();
    return &((*base_type::_container)[base_type::_index]);
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*base_type::_container)[base_type::_index]);
  }
  value_type& operator*() {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index];
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index];
  }

public:
  generic_array_container_iterator operator+(typename base_type::difference_type rhs) const {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = *this;
    copy._index += int(rhs);
    return copy;
  }
  generic_array_container_iterator operator-(typename base_type::difference_type rhs) const {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = *this;
    copy._index -= int(rhs);
    return copy;
  }

  //
  //

  friend inline generic_array_container_iterator operator+(typename base_type::difference_type lhs,
                                                           const generic_array_container_iterator& rhs) {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = rhs;
    copy._index = lhs + rhs._index;
    return copy;
  }

  friend inline generic_array_container_iterator operator-(typename base_type::difference_type lhs,
                                                           const generic_array_container_iterator& rhs) {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = rhs;
    copy._index = lhs - rhs._index;
    return copy;
  }

  //
  //

  generic_array_container_iterator operator++() // ++pre
  {
    base_type::_ensure_is_valid();
    if (base_type::_index < int(base_type::_container->size()))
      ++base_type::_index;
    return *this;
  }

  generic_array_container_iterator operator--() // --pre
  {
    base_type::_ensure_is_valid();
    if (base_type::_index > 0)
      --base_type::_index;
    return *this;
  }

  //
  //

  generic_array_container_iterator operator++(int) // post++
  {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = *this;
    ++(*this); // reuse ++pre
    return copy;
  }

  generic_array_container_iterator operator--(int) // post--
  {
    base_type::_ensure_is_valid();
    generic_array_container_iterator copy = *this;
    --(*this); // reuse --pre
    return copy;
  }

  //
  //

  typename base_type::difference_type operator-(const generic_array_container_iterator& rhs) const {
    base_type::_ensure_is_valid();
    return base_type::_index - rhs._index;
  }
};

//
//
//
//
//

//
//
//
//
//

//
//
//
//
//

template <typename generic_array_container>
class generic_array_container_const_iterator : public generic_array_container_base_iterator<generic_array_container> {

  friend generic_array_container;

public:
  using base_type = generic_array_container_base_iterator<generic_array_container>;
  using value_type = typename base_type::value_type;

public:
  generic_array_container_const_iterator(generic_array_container& container, int index) : base_type(container, index) {}

public:
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index + index];
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*base_type::_container)[base_type::_index]);
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*base_type::_container)[base_type::_index];
  }

public:
  generic_array_container_const_iterator operator+(int value) const {
    generic_array_container_const_iterator copy = *this;
    copy._index += value;
    return copy;
  }
  generic_array_container_const_iterator operator-(int value) const {
    generic_array_container_const_iterator copy = *this;
    copy._index -= value;
    return copy;
  }

  friend inline generic_array_container_const_iterator operator+(typename base_type::difference_type lhs,
                                                                 const generic_array_container_const_iterator& rhs) {
    // generic_array_container_const_iterator copy = *this;
    generic_array_container_const_iterator copy = rhs;
    copy._index = lhs + rhs._index;
    return copy;
    // return generic_array_container_const_iterator(lhs+rhs._ptr);
  }

  friend inline generic_array_container_const_iterator operator-(typename base_type::difference_type lhs,
                                                                 const generic_array_container_const_iterator& rhs) {
    // generic_array_container_const_iterator copy = *this;
    generic_array_container_const_iterator copy = rhs;
    copy._index = lhs - rhs._index;
    return copy;
    // return generic_array_container_const_iterator(lhs-rhs._ptr);
  }

  generic_array_container_const_iterator operator++() // ++pre
  {
    // _ensure_is_valid();
    if (base_type::_index < int(base_type::_container->size()))
      ++base_type::_index;
    return *this;
  }
  generic_array_container_const_iterator operator--() // --pre
  {
    // _ensure_is_valid();
    if (base_type::_index > 0)
      --base_type::_index;
    return *this;
  }

  generic_array_container_const_iterator operator++(int) // post++
  {
    generic_array_container_const_iterator copy = *this;
    ++(*this);
    return copy;
  }

  generic_array_container_const_iterator operator--(int) // post--
  {
    generic_array_container_iterator copy = *this;
    --(*this);
    return copy;
  }
};

//
//
//
//
//

//
//
//
//
//

//
//
//
//
//

template <typename PublicType> class interface_generic_array_container {

public:
  using value_type = PublicType;

public:
  virtual ~interface_generic_array_container() = default;

public:
  virtual bool is_empty() const = 0;
  virtual std::size_t size() const = 0;
  virtual bool is_out_of_range(std::size_t index) const = 0;

public:
  // support out of range index (negative values included)
  virtual const value_type& operator[](int index) const = 0;
  virtual value_type& operator[](int index) = 0;

  virtual const value_type& at(std::size_t index) const = 0;
  virtual value_type& at(std::size_t index) = 0;

  virtual const value_type& front() const = 0;
  virtual value_type& front() = 0;

  virtual const value_type& back() const = 0;
  virtual value_type& back() = 0;

  // public:
  //   virtual bool operator==(const generic_array_container& other) const = 0;
  //   virtual bool operator!=(const generic_array_container& other) const = 0;
};

//
//
//
//
//

template <typename InternalType, typename PublicType = InternalType>
class generic_array_container : public interface_generic_array_container<PublicType> {

public:
  using value_type = PublicType;
  using internal_type = InternalType;
  using base_iterator = generic_array_container_base_iterator<generic_array_container<value_type>>;
  using iterator = generic_array_container_iterator<generic_array_container<value_type>>;
  using const_iterator = generic_array_container_const_iterator<generic_array_container<value_type>>;

protected:
  friend base_iterator;
  friend iterator;
  friend const_iterator;

protected:
  std::size_t _size = 0;
  internal_type* _data = nullptr;

#ifdef D_REF_TRACKER_ITERATORS
  basic_double_linked_list _iterators_list;
  basic_double_linked_list _const_iterators_list;
#endif

public:
  generic_array_container() = default;

  // disable copy
  generic_array_container(const generic_array_container& other) = delete;
  generic_array_container& operator=(const generic_array_container& other) = delete;
  // disable copy

  generic_array_container(generic_array_container&& other) {
    std::swap(_size, other._size);
    std::swap(_data, other._data);
  }

  generic_array_container& operator=(generic_array_container&& other) {
    std::swap(_size, other._size);
    std::swap(_data, other._data);
    return *this;
  }

#ifdef D_REF_TRACKER_ITERATORS
  virtual ~generic_array_container() { invalidate_all_iterators(); }
#else
  virtual ~generic_array_container() = default;
#endif

protected:
  void _ensure_not_empty() const {
    if (_size == 0)
      D_THROW(std::runtime_error, "empty array");
  }

  std::size_t _get_index(int index) const {
    _ensure_not_empty();
    if (index < 0)
      index = int(_size) - (-index) % int(_size);
    if (index >= int(_size))
      index = index % int(_size);
    return std::size_t(index);
  }

  // std::size_t _get_index(std::size_t index) const
  // {
  //   return std::size_t(index) % _size;
  // }

public:
  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, int(_size)); }

  const_iterator begin() const { return const_iterator(*const_cast<generic_array_container*>(this), 0); }
  const_iterator end() const { return const_iterator(*const_cast<generic_array_container*>(this), int(_size)); }

#ifdef D_REF_TRACKER_ITERATORS
  void invalidate_all_iterators() {
    basic_double_linked_list::loop_list_links_and_reset<iterator>(
      _iterators_list, [](iterator* it) -> void { it->_container = nullptr; });
    basic_double_linked_list::loop_list_links_and_reset<const_iterator>(
      _const_iterators_list, [](const_iterator* it) -> void { it->_container = nullptr; });
  }
#endif

public:
  bool is_empty() const override { return _size == 0; }
  std::size_t size() const override { return _size; }
  bool is_out_of_range(std::size_t index) const override { return (index >= _size); }

#ifdef D_REF_TRACKER_ITERATORS
  std::size_t total_iterators() const { return _iterators_list.size + _const_iterators_list.size; }
#endif

public:
  // support out of range index (negative values included)
  const value_type& operator[](int index) const override { return _data[_get_index(index)]; }
  value_type& operator[](int index) override { return _data[_get_index(index)]; }

  const value_type& at(std::size_t index) const override {
    if (is_out_of_range(index))
      D_THROW(std::runtime_error, "out of range, index: " << index << ", size: " << _size);
    return _data[index];
  }
  value_type& at(std::size_t index) override {
    if (is_out_of_range(index))
      D_THROW(std::runtime_error, "out of range, index: " << index << ", size: " << _size);
    return _data[index];
  }

  const value_type& front() const override {
    _ensure_not_empty();
    return _data[0];
  }
  value_type& front() override {
    _ensure_not_empty();
    return _data[0];
  }

  const value_type& back() const override {
    _ensure_not_empty();
    return _data[_size - 1];
  }
  value_type& back() override {
    _ensure_not_empty();
    return _data[_size - 1];
  }

public:
  bool operator==(const generic_array_container& other) const { return this == &other; }
  bool operator!=(const generic_array_container& other) const { return !(*this == other); }
};

} // namespace gero
