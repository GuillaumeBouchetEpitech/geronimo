
#pragma once

#include "basic_double_linked_list.hpp"
#include "dynamic_heap_array.hpp"

// #include <vector>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

namespace gero {

class weak_ref_data_pool_base_class {
public:
  weak_ref_data_pool_base_class() = default;
  weak_ref_data_pool_base_class(const weak_ref_data_pool_base_class& other) = delete;            // block copy
  weak_ref_data_pool_base_class& operator=(const weak_ref_data_pool_base_class& other) = delete; // block copy

  weak_ref_data_pool_base_class(weak_ref_data_pool_base_class&& other) = default;
  weak_ref_data_pool_base_class& operator=(weak_ref_data_pool_base_class&& other) = default;

  virtual ~weak_ref_data_pool_base_class() = default;
};

/**
 * weak_ref_data_pool
 *
 * fixed size data pool
 * - no reallocation at runtime
 * - weak pointer to active data
 * - loop over active data
 */
template <typename InternalBaseType,
          typename PublicBaseType = InternalBaseType,
          std::size_t initial_size = 256,
          bool no_realloc = true
          // , typename std::enable_if<std::is_base_of_v<weak_ref_data_pool_base_class, InternalBaseType>, bool>::type =
          // true
          >
class weak_ref_data_pool {
public:
  struct weak_ref;

  using value_type = PublicBaseType;
  using internal_base_type = InternalBaseType;

public:
  struct internal_data : public internal_base_type {
  public:
    int _index = -1;
    bool _is_active = false;
    basic_double_linked_list _weak_ref_list;

  public:
    using internal_base_type::internal_base_type; // reuse parent internal_base_type  class ctor(s)

  public:
    internal_data(const internal_data& other) = delete; // block copy
    internal_data(internal_data&& other) : internal_base_type(std::move(other)) {
      if (&other == this)
        return;
      std::swap(_index, other._index);
      std::swap(_is_active, other._is_active);
      std::swap(_weak_ref_list.head_link, other._weak_ref_list.head_link);
      std::swap(_weak_ref_list.size, other._weak_ref_list.size);
      sync_all_ref_index();
    }

    virtual ~internal_data() { invalidate_all_ref(); }

  public:
    internal_data& operator=(const internal_data& other) = delete; // block copy
    internal_data& operator=(internal_data&& other) {
      if (&other == this)
        return *this;

      internal_base_type::operator=(std::move(other));

      std::swap(_index, other._index);
      std::swap(_is_active, other._is_active);
      std::swap(_weak_ref_list.head_link, other._weak_ref_list.head_link);
      std::swap(_weak_ref_list.size, other._weak_ref_list.size);
      sync_all_ref_index();
      return *this;
    }

  public:
    void invalidate_all_ref() {

      // D_MYERR(" xxx> this " <<  this << ", list.size " << _weak_ref_list.size);

      basic_double_linked_list::loop_list_links_and_reset<weak_ref>(_weak_ref_list, [](weak_ref* currRef) {
        // D_MYERR("invalidate: " << currLink);

        // D_MYERR("   => currLink " << currLink);
        // D_MYERR("      currLink->_index " << currLink->_index);
        // D_MYERR("      currLink->_pool  " << currLink->_pool );

        if (currRef) {
          currRef->_pool = nullptr;
          currRef->_index = -1;
        }
      });
    }

    void sync_all_ref_index() {
      basic_double_linked_list::loop_list_links<weak_ref>(_weak_ref_list, [this](weak_ref* currRef) {
        if (currRef) {
          currRef->_index = _index;
        }
      });
    }

    void sync_all_ref_pool(weak_ref_data_pool* pool) {
      basic_double_linked_list::loop_list_links<weak_ref>(_weak_ref_list, [pool](weak_ref* currRef) {
        if (currRef) {
          currRef->_pool = pool;
        }
      });
    }
  };

public:
  struct weak_ref {
    friend weak_ref_data_pool;

    // private: // TODO
  public:
    weak_ref_data_pool* _pool = nullptr;
    int32_t _index = -1;

    basic_double_linked_list::link _link;

  public:
    weak_ref() = default;

  private:
    weak_ref(weak_ref_data_pool* inPool, int32_t inIndex) {
      // CTOR

      if (inPool == nullptr || inPool->_itemsPool.is_out_of_range(std::size_t(inIndex))) {
        return;
      }

      _pool = inPool;
      _index = inIndex;
      _link.user_data = static_cast<void*>(this);

      basic_double_linked_list& list = _pool->_itemsPool.at(std::size_t(_index))._weak_ref_list;
      basic_double_linked_list::add_link_to_list(list, _link);
    }

  public:
    // COPY
    weak_ref(const weak_ref& other) {
      if (&other == this)
        return;
      _doCopy(other);
    }

    // MOVE
    weak_ref(weak_ref&& other) {
      if (&other == this)
        return;
      _doMove(std::move(other));
    }

    virtual ~weak_ref() {

      // D_MYERR(" => this " <<  this << ",  _pool " << _pool << ", _index " << _index);
      // if (_pool) {
      //   D_MYERR("    this " <<  this << ", size " << _pool->size());

      //   if (!_pool->_itemsPool.is_out_of_range(std::size_t(_index))) {
      //     const auto& list = _pool->_itemsPool.at(std::size_t(_index))._weak_ref_list;
      //     D_MYERR("    this " <<  this << ", list.size " << list.size);
      //   }
      // }

      invalidate();
    }

  public:
    // COPY
    weak_ref& operator=(const weak_ref& other) {
      if (&other == this)
        return *this;
      _doCopy(other);
      return *this;
    }

    // MOVE
    weak_ref& operator=(weak_ref&& other) {
      if (&other == this)
        return *this;
      _doMove(std::move(other));
      return *this;
    }

  public:
    static weak_ref make_invalid() { return weak_ref(); }

    void invalidate() {

      // if (_pool)
      // {
      //   D_MYERR(" => this " <<  this << ",  _pool " << _pool << ", _index " << _index << ", size " << _pool->size());
      // }
      // else
      // {
      //   D_MYERR(" => this " <<  this << ",  _pool " << _pool << ", _index " << _index << ", size ???");
      // }

      // if (_pool != nullptr) {
      if (_pool != nullptr && _index >= 0 && !_pool->_itemsPool.is_out_of_range(std::size_t(_index))) {
        basic_double_linked_list& list = _pool->_itemsPool.at(std::size_t(_index))._weak_ref_list;
        basic_double_linked_list::remove_link_from_list(list, _link);
      }

      basic_double_linked_list::reset_link(_link);
      _pool = nullptr;
      _index = -1;
    }

  private:
    void _doCopy(const weak_ref& other) {

      if (&other == this)
        return;

      _pool = other._pool;
      _index = other._index;
      _link.user_data = static_cast<void*>(this);

      if (_index < 0 || _pool == nullptr)
        return;

      basic_double_linked_list& list = _pool->_itemsPool.at(std::size_t(_index))._weak_ref_list;
      basic_double_linked_list::add_link_to_list(list, _link);
    }

    void _doMove(weak_ref&& other) {
      if (&other == this)
        return;

      std::swap(_pool, other._pool);
      std::swap(_index, other._index);

      _link.user_data = static_cast<void*>(this);
      other._link.user_data = static_cast<void*>(&other);

      if (_index < 0 || _pool == nullptr)
        return;

      basic_double_linked_list& list = _pool->_itemsPool.at(std::size_t(_index))._weak_ref_list;
      basic_double_linked_list::swap_two_links_from_same_list(list, other._link, _link);
    }

  public:
    operator bool() const { return is_active(); }

    bool operator==(const weak_ref& other) const {
      return (is_active() == other.is_active() && _pool == other._pool && _index == other._index);
    }

    bool operator!=(const weak_ref& other) const { return !weak_ref::operator==(other); }

  public:
    bool is_active() const {
      return (_index >= 0 && _pool &&
              // !_pool->_itemsPool.is_out_of_range(std::size_t(_index)) &&
              _pool->_itemsPool.at(std::size_t(_index))._is_active);
    }

    int32_t index() { return _index; }
    int32_t index() const { return _index; }

    value_type* get() { return _index < 0 ? nullptr : &_pool->_itemsPool.at(std::size_t(_index)); }
    const value_type* get() const { return _index < 0 ? nullptr : &_pool->_itemsPool.at(std::size_t(_index)); }

    value_type* operator->() { return get(); }
    const value_type* operator->() const { return get(); }

    value_type& operator*() { return *get(); }
    const value_type& operator*() const { return *get(); }
  };

private:
  dynamic_heap_array<internal_data, internal_data, initial_size> _itemsPool;

public:
  weak_ref_data_pool() = default;
  ~weak_ref_data_pool() { clear(); }

  // disable copy
  weak_ref_data_pool(const weak_ref_data_pool& other) = delete;
  weak_ref_data_pool& operator=(const weak_ref_data_pool& other) = delete;
  // disable copy

  weak_ref_data_pool(weak_ref_data_pool&& other) {
    _itemsPool = std::move(other._itemsPool);
    for (auto& item : _itemsPool)
      item.sync_all_ref_pool(this);
  }

  weak_ref_data_pool& operator=(weak_ref_data_pool&& other) {
    _itemsPool = std::move(other._itemsPool);
    for (auto& item : _itemsPool)
      item.sync_all_ref_pool(this);

    return *this;
  }

  void pre_allocate(std::size_t newCapacity) { _itemsPool.pre_allocate(newCapacity); }

  void clear() {
    for (internal_data& item : _itemsPool)
      item.invalidate_all_ref();

    _itemsPool.clear();
  }

  // weak_ref acquire(value_type&& toMove)
  // {
  //   if (_itemsPool.size() == _itemsPool.capacity())
  //     return weak_ref::make_invalid();

  //   const std::size_t index = _itemsPool.size();

  //   // internal_data& currData = _itemsPool.push_back(std::move(toMove));
  //   internal_data testData = internal_data(std::move(toMove));
  //   internal_data& currData = _itemsPool.push_back(std::move(testData));
  //   // currData = std::move(toMove);
  //   // internal_data movedData = std::move(toMove);
  //   // internal_data& currData = _itemsPool.push_back(std::move(movedData));

  //   currData._index = int32_t(index);
  //   currData._is_active = true;

  //   return weak_ref(this, std::size_t(index));
  // }

  // weak_ref acquire()
  // {
  //   if (no_realloc == true && _itemsPool.size() == _itemsPool.capacity())
  //     return weak_ref::make_invalid();

  //   const std::size_t index = _itemsPool.size();

  //   internal_data& currData = _itemsPool.emplace_back();

  //   currData._index = int32_t(index);
  //   currData._is_active = true;

  //   return weak_ref(this, std::size_t(index));
  // }

  // weak_ref acquire(value_type&& toMove)
  // {
  //   if (no_realloc == true && _itemsPool.size() == _itemsPool.capacity())
  //     return weak_ref::make_invalid();

  //   const std::size_t index = _itemsPool.size();

  //   internal_data tmpData = std::move()

  //   internal_data& currData = _itemsPool.push_back(std::move(toMove));

  //   currData._index = int32_t(index);
  //   currData._is_active = true;

  //   // std::cout << "acquire index=" << index << std::endl;
  //   // std::cout << "acquire _index=" << _itemsPool.at(index)._index <<
  //   std::endl;
  //   // std::cout << "acquire _is_active=" << _itemsPool.at(index)._is_active
  //   << std::endl;

  //   return weak_ref(this, std::size_t(index));
  // }

  template <typename... Args> weak_ref acquire(Args&&... args) {
    if (no_realloc == true && _itemsPool.size() == _itemsPool.capacity())
      return weak_ref::make_invalid();

    const int32_t index = int32_t(_itemsPool.size());

    internal_data& currData = _itemsPool.emplace_back(std::forward<Args>(args)...);

    currData._index = index;
    currData._is_active = true;

    // D_MYERR(" lol ====> " << index);
    // D_MYERR("           " << _itemsPool.get_index(currData));
    // D_MYERR("      addr " << reinterpret_cast<int64_t>(&currData));

    return weak_ref(this, index);
  }

  weak_ref get(uint32_t index) { return weak_ref(this, int32_t(index)); }
  weak_ref get(uint32_t index) const { return weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index)); }

  weak_ref get(const weak_ref& ref) { return weak_ref(this, get_index(ref)); }
  weak_ref get(const weak_ref& ref) const { return weak_ref(const_cast<weak_ref_data_pool*>(this), get_index(ref)); }

  // weak_ref get(const value_type& target) { return weak_ref(this,
  // get_index(target)); } weak_ref get(const value_type& target) const {
  // return weak_ref(const_cast<weak_ref_data_pool*>(this), get_index(target));
  // }

  // weak_ref get(value_type& public_type) {

  //   return weak_ref(this, int32_t(index));
  // }
  // const weak_ref get(value_type& public_type) const {
  //   return weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index));
  // }

  std::size_t size() const { return _itemsPool.size(); }
  bool is_empty() const { return _itemsPool.is_empty(); }

public:
  uint32_t get_ref_count(uint32_t index) {
    if (_itemsPool.is_out_of_range(index))
      return 0;
    return _itemsPool.at(index)._weak_ref_list.size;
  }

  uint32_t get_ref_count(const weak_ref& ref) { return get_ref_count(uint32_t(get_index(ref))); }

  // uint32_t get_ref_count(const value_type& target) {
  //   return get_ref_count(uint32_t(get_index(&target)));
  // }

public:
  int32_t get_index(const weak_ref& ref) const { return ref._index; }
  // int32_t get_index(const value_type& target) const { return
  // _itemsPool.get_index(target); }

public:
  bool is_active(const weak_ref& ref) { return ref.is_active(); }
  bool is_active(std::size_t index) const { return is_active(&_itemsPool.at(index)); }
  bool is_active(const value_type& target) const { return reinterpret_cast<internal_data*>(&target)->_is_active; }

public:
  void release(const weak_ref& ref) {
    if (!ref.is_active())
      return;
    release(ref._index);
  }
  // void release(std::size_t index) { release(&_itemsPool.at(index)); }

  // void release(const value_type& externalData) {
  // release(get_index(externalData)); }

  // void release(const value_type& externalData) {

  //   const int32_t index = get_index(externalData);

  void release(int32_t index) {

    // internal_data* pInternal_data =
    // static_cast<internal_data*>(externalData);

    // if (pInternal_data->_is_active == false)
    if (index == -1)
      return;

    auto& currData = _itemsPool.at(std::size_t(index));

    // currData.invalidate_all_ref();
    // currData._is_active = false;

    // // std::size_t index = std::size_t(currData._index);

    // const uint32_t totalSwapped = _itemsPool.unsorted_erase(std::size_t(index));

    // // std::cout << "post" << std::endl;

    // if (totalSwapped > 0) {
    //   auto& item = _itemsPool.at(std::size_t(index));
    //   item._index = index;
    //   item.sync_all_ref_index();
    // }

    _release(currData);
  }

private:
  void _release(internal_data& curr_item) {

    int32_t index = curr_item._index;

    curr_item.invalidate_all_ref();
    curr_item._is_active = false;

    // std::size_t index = std::size_t(index);

    const uint32_t totalSwapped = _itemsPool.unsorted_erase(std::size_t(index));

    // std::cout << "post" << std::endl;

    if (totalSwapped > 0) {
      auto& item = _itemsPool.at(std::size_t(index));
      item._index = index;
      item.sync_all_ref_index();
    }
  }

public:
  void remove_unreferenced_items() {
    for (std::size_t index = 0; index < _itemsPool.size();) {
      if (_itemsPool.at(index)._weak_ref_list.size == 0) {
        release(int32_t(index));
      } else {
        ++index;
      }
    }
  }

  void filter(std::function<bool(value_type&)> callback) {
    for (std::size_t index = 0; index < _itemsPool.size();) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false) {
        ++index;
        continue;
      }

      if (callback(item) == false) {
        release(int32_t(index));
      } else {
        ++index;
      }
    }
  }

  void filter(std::function<bool(value_type&, int)> callback) {
    for (std::size_t index = 0; index < _itemsPool.size();) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false) {
        ++index;
        continue;
      }

      if (callback(item, index) == false) {
        release(int32_t(index));
      } else {
        ++index;
      }
    }
  }

  void for_each(std::function<void(value_type&)> callback) {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item);
  }

  void for_each(std::function<void(value_type&, int)> callback) {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item, item._index);
  }

  void for_each(std::function<void(const value_type&)> callback) const {
    for (const auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item);
  }

  void for_each(std::function<void(const value_type&, int)> callback) const {
    for (const auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item, item._index);
  }

  weak_ref find_if(std::function<bool(const value_type&)> callback) const {
    for (std::size_t index = 0; index < _itemsPool.size(); ++index) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false)
        continue;

      if (callback(item) == true)
        return weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index));
    }
    return weak_ref::make_invalid();
  }
};

template <typename InternalBaseType,
          typename PublicBaseType = InternalBaseType,
          std::size_t initial_size = 256,
          bool no_realloc = true,
          typename std::enable_if<std::is_base_of_v<weak_ref_data_pool_base_class, InternalBaseType>, bool>::type =
            true>
using safe_weak_ref_data_pool = weak_ref_data_pool<InternalBaseType, PublicBaseType, initial_size, no_realloc>;

} // namespace gero
