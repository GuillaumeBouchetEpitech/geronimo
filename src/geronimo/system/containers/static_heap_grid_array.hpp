
#pragma once

#include "basic_double_linked_list.hpp"
#include "dynamic_heap_array.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <vector>
// #include <functional>
#include <iostream>
#include <utility>

namespace gero {

template <typename static_heap_grid_array>
class static_heap_grid_array_base_column_iterator : public basic_double_linked_list::link {

  friend static_heap_grid_array;

public:
  using value_type = typename static_heap_grid_array::value_type;

protected:
  static_heap_grid_array* _container;
  int _row;
  int _column;

public:
  static_heap_grid_array_base_column_iterator(static_heap_grid_array& container, int row, int column)
    : _container(&container), _row(row), _column(column) {
    basic_double_linked_list::add_link_to_list(_container->_column_iterators_list, *this);
  }

  ~static_heap_grid_array_base_column_iterator() {
    if (_container)
      basic_double_linked_list::remove_link_from_list(_container->_column_iterators_list, *this);
  }

public:
  bool is_valid() const { return _container != nullptr; }

protected:
  void _ensure_is_valid() const {
    if (!is_valid())
      D_THROW(std::runtime_error, "invalid iterator");
  }

public:
  static_heap_grid_array_base_column_iterator& operator++() // ++pre
  {
    _ensure_is_valid();
    // if (_column < int(_container->size()))
    ++_column;
    return *this;
  }
  static_heap_grid_array_base_column_iterator& operator++(int) // post++
  {
    static_heap_grid_array_base_column_iterator copy = *this;
    ++(*this);
    return copy;
  }

  static_heap_grid_array_base_column_iterator& operator--() // --pre
  {
    _ensure_is_valid();
    // if (_column > 0)
    --_column;
    return *this;
  }
  static_heap_grid_array_base_column_iterator& operator--(int) // post--
  {
    static_heap_grid_array_base_column_iterator copy = *this;
    --(*this);
    return copy;
  }

public:
  bool operator==(const static_heap_grid_array_base_column_iterator& other) const {
    return (_container == other._container && _row == other._row && _column == other._column);
  }
  bool operator!=(const static_heap_grid_array_base_column_iterator& other) const { return !(*this == other); }
};

template <typename static_heap_grid_array>
class static_heap_grid_array_column_iterator
  : public static_heap_grid_array_base_column_iterator<static_heap_grid_array> {

  friend static_heap_grid_array;

public:
  using base_type = static_heap_grid_array_base_column_iterator<static_heap_grid_array>;
  using value_type = typename base_type::value_type;

public:
  static_heap_grid_array_column_iterator(static_heap_grid_array& container, int row, int column)
    : base_type(container, row, column) {}

public:
  value_type& operator[](int index) {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column + index);
  }
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column + index);
  }
  value_type* operator->() {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  value_type& operator*() {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }

public:
  static_heap_grid_array_column_iterator operator+(int value) const {
    static_heap_grid_array_column_iterator copy = *this;
    copy._column += value;
    return copy;
  }
  static_heap_grid_array_column_iterator operator-(int value) const {
    static_heap_grid_array_column_iterator copy = *this;
    copy._column -= value;
    return copy;
  }
};

template <typename static_heap_grid_array>
class static_heap_grid_array_const_column_iterator
  : public static_heap_grid_array_base_column_iterator<static_heap_grid_array> {

  friend static_heap_grid_array;

public:
  using base_type = static_heap_grid_array_base_column_iterator<static_heap_grid_array>;
  using value_type = typename base_type::value_type;

public:
  static_heap_grid_array_const_column_iterator(static_heap_grid_array& container, int row, int column)
    : base_type(container, row, column) {}

public:
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column + index);
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }

public:
  static_heap_grid_array_const_column_iterator operator+(int value) const {
    static_heap_grid_array_const_column_iterator copy = *this;
    copy._column += value;
    return copy;
  }
  static_heap_grid_array_const_column_iterator operator-(int value) const {
    static_heap_grid_array_const_column_iterator copy = *this;
    copy._column -= value;
    return copy;
  }
};

//
//
//
//
//

template <typename static_heap_grid_array>
class static_heap_grid_array_base_row_iterator : public basic_double_linked_list::link {

  friend static_heap_grid_array;

public:
  using value_type = typename static_heap_grid_array::value_type;

protected:
  static_heap_grid_array* _container;
  int _row;
  int _column;

public:
  static_heap_grid_array_base_row_iterator(static_heap_grid_array& container, int row, int column)
    : _container(&container), _row(row), _column(column) {
    basic_double_linked_list::add_link_to_list(_container->_row_iterators_list, *this);
  }

  ~static_heap_grid_array_base_row_iterator() {
    if (_container)
      basic_double_linked_list::remove_link_from_list(_container->_row_iterators_list, *this);
  }

public:
  bool is_valid() const { return _container != nullptr; }

protected:
  void _ensure_is_valid() const {
    if (!is_valid())
      D_THROW(std::runtime_error, "invalid iterator");
  }

public:
  static_heap_grid_array_base_row_iterator& operator++() // ++pre
  {
    _ensure_is_valid();
    // if (_column < int(_container->size()))
    ++_row;
    return *this;
  }
  static_heap_grid_array_base_row_iterator& operator++(int) // post++
  {
    static_heap_grid_array_base_row_iterator copy = *this;
    ++(*this);
    return copy;
  }

  static_heap_grid_array_base_row_iterator& operator--() // --pre
  {
    _ensure_is_valid();
    // if (_column > 0)
    --_row;
    return *this;
  }
  static_heap_grid_array_base_row_iterator& operator--(int) // post--
  {
    static_heap_grid_array_base_row_iterator copy = *this;
    --(*this);
    return copy;
  }

public:
  bool operator==(const static_heap_grid_array_base_row_iterator& other) const {
    return (_container == other._container && _row == other._row && _column == other._column);
  }
  bool operator!=(const static_heap_grid_array_base_row_iterator& other) const { return !(*this == other); }
};

template <typename static_heap_grid_array>
class static_heap_grid_array_row_iterator : public static_heap_grid_array_base_row_iterator<static_heap_grid_array> {

  friend static_heap_grid_array;

public:
  using base_type = static_heap_grid_array_base_row_iterator<static_heap_grid_array>;
  using column_iterator = typename static_heap_grid_array::column_iterator;
  using const_column_iterator = typename static_heap_grid_array::const_column_iterator;
  using value_type = typename base_type::value_type;

public:
  static_heap_grid_array_row_iterator(static_heap_grid_array& container, int row, int column)
    : base_type(container, row, column) {}

public:
  value_type& operator[](int index) {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row + index, this->_column);
  }
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row + index, this->_column);
  }
  value_type* operator->() {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  value_type& operator*() {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }

public:
  column_iterator beginColumns() { return column_iterator(*this->_container, this->_row, this->_column); }
  column_iterator endColumns() {
    return column_iterator(*this->_container, this->_row, this->_column + int(this->_container->_width));
  }
  const_column_iterator beginColumns() const {
    return const_column_iterator(*this->_container, this->_row, this->_column);
  }
  const_column_iterator endColumns() const {
    return const_column_iterator(*this->_container, this->_row, this->_column + int(this->_container->_width));
  }

public:
  static_heap_grid_array_row_iterator operator+(int value) const {
    static_heap_grid_array_row_iterator copy = *this;
    copy._column += value;
    return copy;
  }
  static_heap_grid_array_row_iterator operator-(int value) const {
    static_heap_grid_array_row_iterator copy = *this;
    copy._column -= value;
    return copy;
  }
};

template <typename static_heap_grid_array>
class static_heap_grid_array_const_row_iterator
  : public static_heap_grid_array_base_row_iterator<static_heap_grid_array> {

  friend static_heap_grid_array;

public:
  using base_type = static_heap_grid_array_base_row_iterator<static_heap_grid_array>;
  using const_column_iterator = typename static_heap_grid_array::const_column_iterator;
  using value_type = typename base_type::value_type;

public:
  static_heap_grid_array_const_row_iterator(static_heap_grid_array& container, int row, int column)
    : base_type(container, row, column) {}

public:
  const value_type& operator[](int index) const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row + index, this->_column);
  }
  const value_type* operator->() const {
    base_type::_ensure_is_valid();
    return &((*this->_container)(this->_row, this->_column));
  }
  const value_type& operator*() const {
    base_type::_ensure_is_valid();
    return (*this->_container)(this->_row, this->_column);
  }

public:
  const_column_iterator beginColumns() const {
    return const_column_iterator(*this->_container, this->_row, this->_column);
  }
  const_column_iterator endColumns() const {
    return const_column_iterator(*this->_container, this->_row, this->_column + int(this->_container->_width));
  }

public:
  static_heap_grid_array_const_row_iterator operator+(int value) const {
    static_heap_grid_array_const_row_iterator copy = *this;
    copy._column += value;
    return copy;
  }
  static_heap_grid_array_const_row_iterator operator-(int value) const {
    static_heap_grid_array_const_row_iterator copy = *this;
    copy._column -= value;
    return copy;
  }
};

//
//
//
//
//

/**
 * static_heap_grid_array
 *
 * ...TODO
 */
template <typename InternalBaseType, typename PublicBaseType = InternalBaseType> class static_heap_grid_array {
public:
  using value_type = PublicBaseType;
  using internal_type = InternalBaseType;
  using base_column_iterator = static_heap_grid_array_base_column_iterator<static_heap_grid_array<value_type>>;
  using column_iterator = static_heap_grid_array_column_iterator<static_heap_grid_array<value_type>>;
  using const_column_iterator = static_heap_grid_array_const_column_iterator<static_heap_grid_array<value_type>>;
  using base_row_iterator = static_heap_grid_array_base_row_iterator<static_heap_grid_array<value_type>>;
  using row_iterator = static_heap_grid_array_row_iterator<static_heap_grid_array<value_type>>;
  using const_row_iterator = static_heap_grid_array_const_row_iterator<static_heap_grid_array<value_type>>;

protected:
  friend base_column_iterator;
  friend column_iterator;
  friend const_column_iterator;
  friend base_row_iterator;
  friend row_iterator;
  friend const_row_iterator;

private:
  dynamic_heap_array<internal_type> _data;
  std::size_t _height = 0;
  std::size_t _width = 0;

  basic_double_linked_list _row_iterators_list;
  basic_double_linked_list _column_iterators_list;

public:
  static_heap_grid_array() = default;
  virtual ~static_heap_grid_array() { clear(); }

  // disable move/copy
  static_heap_grid_array(const static_heap_grid_array& other) = delete;
  static_heap_grid_array(static_heap_grid_array&& other) = delete;
  static_heap_grid_array& operator=(const static_heap_grid_array& other) = delete;
  static_heap_grid_array& operator=(static_heap_grid_array&& other) = delete;
  // disable move/copy

public:
  void allocate(std::size_t height, std::size_t width) {
    _height = height;
    _width = width;
    _data.ensure_size(_height * _width);
  }

  void clear() {
    invalidate_all_iterators();
    _data.clear();
  }

private:
  int _get_index(int index, std::size_t size) const {
    if (index < 0)
      index = int(size) - (-index) % int(size);
    if (index >= int(size))
      index = index % int(size);
    return index;
  }

  int _get_data_index(int row, int column) const {
    return _get_index(row, _height) * int(_width) + _get_index(column, _width);
  }

public:
  value_type& get(int row, int column) { return _data[_get_data_index(row, column)]; }
  value_type& operator()(int row, int column) { return get(row, column); }

  const value_type& get(int row, int column) const { return _data[_get_data_index(row, column)]; }
  const value_type& operator()(int row, int column) const { return get(row, column); }

  const value_type& operator[](int index) const { return _data[index]; }
  value_type& operator[](int index) { return _data[index]; }

  std::size_t width() const { return _width; }
  std::size_t height() const { return _height; }
  std::size_t size() const { return _data.size(); }
  bool is_empty() const { return _data.is_empty(); }

public:
  column_iterator beginColumns() { return column_iterator(*this, 0, 0); }
  column_iterator endColumns() { return column_iterator(*this, int(_height), int(_width)); }
  const_column_iterator beginColumns() const {
    return const_column_iterator(*const_cast<static_heap_grid_array*>(this), 0, 0);
  }
  const_column_iterator endColumns() const {
    return const_column_iterator(*const_cast<static_heap_grid_array*>(this), int(_height), int(_width));
  }

public:
  column_iterator beginColumns(uint32_t row) { return column_iterator(*this, int(row), 0); }
  column_iterator endColumns(uint32_t row) { return column_iterator(*this, int(row), int(_width)); }
  const_column_iterator beginColumns(uint32_t row) const {
    return const_column_iterator(*const_cast<static_heap_grid_array*>(this), int(row), 0);
  }
  const_column_iterator endColumns(uint32_t row) const {
    return const_column_iterator(*const_cast<static_heap_grid_array*>(this), int(row), int(_width));
  }

public:
  row_iterator beginRows(uint32_t row) { return row_iterator(*this, int(row), 0); }
  row_iterator endRows(uint32_t row) { return row_iterator(*this, int(row) + 1, 0); }
  const_row_iterator beginRows(uint32_t row) const {
    return const_row_iterator(*const_cast<static_heap_grid_array*>(this), int(row), 0);
  }
  const_row_iterator endRows(uint32_t row) const {
    return const_row_iterator(*const_cast<static_heap_grid_array*>(this), int(row) + 1, 0);
  }

public:
  row_iterator beginRows() { return row_iterator(*this, 0, 0); }
  row_iterator endRows() { return row_iterator(*this, int(_height), 0); }
  const_row_iterator beginRows() const { return const_row_iterator(*const_cast<static_heap_grid_array*>(this), 0, 0); }
  const_row_iterator endRows() const {
    return const_row_iterator(*const_cast<static_heap_grid_array*>(this), int(_height), 0);
  }

public:
  void invalidate_all_iterators() {
    basic_double_linked_list::loop_list_links_and_reset<column_iterator>(
      _column_iterators_list, [](column_iterator* it) -> void { it->_container = nullptr; });
    basic_double_linked_list::loop_list_links_and_reset<row_iterator>(
      _row_iterators_list, [](row_iterator* it) -> void { it->_container = nullptr; });
  }
};

} // namespace gero
