
#pragma once

#include "basic_double_linked_list.hpp"
#include "dynamic_heap_array.hpp"

// #include <vector>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

namespace gero {

template <typename PublicBaseType> struct data_pool_weak_ref;

struct interface_internal_data {
  virtual ~interface_internal_data() = default;
  virtual basic_double_linked_list& get_weak_ref_list() = 0;
  virtual bool is_active() = 0;
};

template <typename PublicBaseType> struct data_pool_weak_ref_interface_weak_ref_data_pool {

  friend data_pool_weak_ref<PublicBaseType>;

public:
  virtual ~data_pool_weak_ref_interface_weak_ref_data_pool() = default;

  using value_type = PublicBaseType;

protected:
  virtual interface_internal_data& _get_itemsPool_data_by_index(std::size_t inIndex) = 0;
  virtual value_type& _get_itemsPool_public_data_by_index(std::size_t inIndex) = 0;
  virtual const value_type& _get_itemsPool_public_data_by_index(std::size_t inIndex) const = 0;
  virtual bool _is_out_of_range(std::size_t inIndex) = 0;
};

// _itemsPool.is_out_of_range(index)
// _itemsPool.at(std::size_t(index))._weak_ref_list
// _itemsPool.at(std::size_t(index))._is_active

// class interface_weak_ref_data_pool {
// public:
//   virtual ~interface_weak_ref_data_pool() = default;

// public:
// };

template <typename PublicBaseType> struct data_pool_weak_ref {

  using pool_type = data_pool_weak_ref_interface_weak_ref_data_pool<PublicBaseType>;

  // friend weak_ref_data_pool<InternalBaseType, PublicBaseType, initial_size, no_realloc>;

  using value_type = PublicBaseType;

  // private: // TODO
public:
  pool_type* _pool = nullptr;
  int32_t _index = -1;

  basic_double_linked_list::link _link;

public:
  explicit data_pool_weak_ref() = default;

  // private: // TODO
  explicit data_pool_weak_ref(pool_type* inPool, int32_t inIndex) {
    // CTOR

    if (inPool == nullptr || inPool->_is_out_of_range(std::size_t(inIndex))) {
      return;
    }

    _pool = inPool;
    _index = inIndex;
    _link.user_data = static_cast<void*>(this);

    basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
    basic_double_linked_list::add_link_to_list(list, _link);
  }

public:
  // COPY
  data_pool_weak_ref(const data_pool_weak_ref& other) {
    if (&other == this)
      return;
    _doCopy(other);
  }

  // MOVE
  data_pool_weak_ref(data_pool_weak_ref&& other) {
    if (&other == this)
      return;
    _doMove(std::move(other));
  }

  virtual ~data_pool_weak_ref() {

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
  data_pool_weak_ref& operator=(const data_pool_weak_ref& other) {
    if (&other == this)
      return *this;
    _doCopy(other);
    return *this;
  }

  // MOVE
  data_pool_weak_ref& operator=(data_pool_weak_ref&& other) {
    if (&other == this)
      return *this;
    _doMove(std::move(other));
    return *this;
  }

public:
  static data_pool_weak_ref make_invalid() { return data_pool_weak_ref(); }

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
    if (_pool != nullptr && _index >= 0 && !_pool->_is_out_of_range(std::size_t(_index))) {
      basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
      basic_double_linked_list::remove_link_from_list(list, _link);
    }

    basic_double_linked_list::reset_link(_link);
    _pool = nullptr;
    _index = -1;
  }

private:
  void _doCopy(const data_pool_weak_ref& other) {

    if (&other == this)
      return;

    if (is_active()) {
      basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
      basic_double_linked_list::remove_link_from_list(list, _link);
    }

    _pool = other._pool;
    _index = other._index;
    basic_double_linked_list::reset_link(_link);
    _link.user_data = static_cast<void*>(this);

    if (is_active()) {
      basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
      basic_double_linked_list::add_link_to_list(list, _link);
    }
  }

  void _doMove(data_pool_weak_ref&& other) {
    if (&other == this)
      return;

    const bool currWasActive = is_active();
    const bool otherWasActive = other.is_active();

    // if (!currWasActive && !otherWasActive) {
    //   // do nothing
    // }
    // else
    if (currWasActive && otherWasActive) {

      std::swap(_index, other._index);

      {
        basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
        basic_double_linked_list::swap_two_links_from_same_list(list, other._link, _link);
      }

    } else if (currWasActive && !otherWasActive) {
      // remove current
      // add other

      {
        basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
        basic_double_linked_list::remove_link_from_list(list, _link);
      }

      std::swap(_pool, other._pool);
      std::swap(_index, other._index);

      {
        other._link.user_data = static_cast<void*>(&other);
        basic_double_linked_list& list =
          other._pool->_get_itemsPool_data_by_index(std::size_t(other._index)).get_weak_ref_list();
        basic_double_linked_list::add_link_to_list(list, other._link);
      }
    } else if (!currWasActive && otherWasActive) {
      // remove other
      // add current

      {
        basic_double_linked_list& list =
          other._pool->_get_itemsPool_data_by_index(std::size_t(other._index)).get_weak_ref_list();
        basic_double_linked_list::remove_link_from_list(list, other._link);
      }

      std::swap(_pool, other._pool);
      std::swap(_index, other._index);

      {
        _link.user_data = static_cast<void*>(this);
        basic_double_linked_list& list = _pool->_get_itemsPool_data_by_index(std::size_t(_index)).get_weak_ref_list();
        basic_double_linked_list::add_link_to_list(list, _link);
      }
    }
  }

public:
  operator bool() const { return is_active(); }

  bool operator==(const data_pool_weak_ref& other) const {
    return (is_active() == other.is_active() && _pool == other._pool && _index == other._index);
  }

  bool operator!=(const data_pool_weak_ref& other) const { return !data_pool_weak_ref::operator==(other); }

public:
  bool is_active() {
    return (_index >= 0 && _pool && _pool->_get_itemsPool_data_by_index(std::size_t(_index)).is_active());
  }
  bool is_active() const {
    return (_index >= 0 && _pool && _pool->_get_itemsPool_data_by_index(std::size_t(_index)).is_active());
  }

  int32_t index() { return _index; }
  int32_t index() const { return _index; }

  value_type* get() { return _index < 0 ? nullptr : &_pool->_get_itemsPool_public_data_by_index(std::size_t(_index)); }
  const value_type* get() const {
    return _index < 0 ? nullptr : &_pool->_get_itemsPool_public_data_by_index(std::size_t(_index));
  }

  value_type* operator->() { return get(); }
  const value_type* operator->() const { return get(); }

  value_type& operator*() { return *get(); }
  const value_type& operator*() const { return *get(); }
};

/**
 * interface_weak_ref_data_pool
 *
 * fixed size data pool
 * - no reallocation at runtime
 * - weak pointer to active data
 * - loop over active data
 */
template <typename PublicBaseType> class interface_weak_ref_data_pool {
public:
  virtual ~interface_weak_ref_data_pool() = default;

public:
  using value_type = PublicBaseType;
  using weak_ref = data_pool_weak_ref<PublicBaseType>;

public:
  virtual void pre_allocate(std::size_t newCapacity) = 0;

  virtual void clear() = 0;

  // template <typename... Args>
  // virtual weak_ref acquire(Args&&... args) = 0;

  virtual weak_ref get(uint32_t index) = 0;
  virtual weak_ref get(uint32_t index) const = 0;

  virtual weak_ref get(const weak_ref& ref) = 0;
  virtual weak_ref get(const weak_ref& ref) const = 0;

  virtual std::size_t size() const = 0;
  virtual bool is_empty() const = 0;

public:
  virtual uint32_t get_ref_count(uint32_t index) const = 0;
  virtual uint32_t get_ref_count(const weak_ref& ref) const = 0;

public:
  virtual int32_t get_index(const weak_ref& ref) const = 0;

public:
  virtual bool is_active(const weak_ref& ref) = 0;
  virtual bool is_active(std::size_t index) const = 0;
  // virtual bool is_active(const value_type& target) const = 0;

public:
  virtual void release(const weak_ref& ref) = 0;
  virtual void release(int32_t index) = 0;

public:
  virtual void remove_unreferenced_items() = 0;

  virtual void filter(std::function<bool(value_type&)> callback) = 0;
  virtual void filter(std::function<bool(value_type&, int)> callback) = 0;

  virtual void for_each(std::function<void(value_type&)> callback) = 0;
  virtual void for_each(std::function<void(value_type&, int)> callback) = 0;
  virtual void for_each(std::function<void(weak_ref)> callback) = 0;

  virtual void for_each(std::function<void(const value_type&)> callback) const = 0;
  virtual void for_each(std::function<void(const value_type&, int)> callback) const = 0;
  virtual void for_each(std::function<void(weak_ref)> callback) const = 0;

  virtual weak_ref find_if(std::function<bool(const value_type&)> callback) const = 0;
  virtual weak_ref find_if(std::function<bool(const weak_ref)> callback) const = 0;
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
          bool no_realloc = true>
class weak_ref_data_pool : public interface_weak_ref_data_pool<PublicBaseType>,
                           public data_pool_weak_ref_interface_weak_ref_data_pool<PublicBaseType> {
public:
  // struct weak_ref;

  using value_type = PublicBaseType;
  using internal_base_type = InternalBaseType;

  using weak_ref = data_pool_weak_ref<PublicBaseType>;

  friend data_pool_weak_ref<PublicBaseType>;

public:
  struct internal_data : public internal_base_type, public interface_internal_data {
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
          // basic_double_linked_list::reset_link(currRef->_link);
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

  public:
    basic_double_linked_list& get_weak_ref_list() override { return _weak_ref_list; }
    bool is_active() override { return _is_active; }
  };

private:
  dynamic_heap_array<internal_data, internal_data, initial_size> _itemsPool;

private:
  // interface_generic_array_container<interface_internal_data>& _get_itemsPool()
  // {
  //   return _itemsPool;
  // }
  virtual interface_internal_data& _get_itemsPool_data_by_index(std::size_t inIndex) override {
    return _itemsPool.at(inIndex);
  }
  virtual value_type& _get_itemsPool_public_data_by_index(std::size_t inIndex) override {
    return _itemsPool.at(inIndex);
  }
  virtual const value_type& _get_itemsPool_public_data_by_index(std::size_t inIndex) const override {
    return _itemsPool.at(inIndex);
  }
  virtual bool _is_out_of_range(std::size_t inIndex) override { return _itemsPool.is_out_of_range(inIndex); }

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

  void pre_allocate(std::size_t newCapacity) override { _itemsPool.pre_allocate(newCapacity); }

  void clear() override {
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

  weak_ref get(uint32_t index) override { return weak_ref(this, int32_t(index)); }
  weak_ref get(uint32_t index) const override {
    return weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index));
  }

  weak_ref get(const weak_ref& ref) override { return weak_ref(this, get_index(ref)); }
  weak_ref get(const weak_ref& ref) const override {
    return weak_ref(const_cast<weak_ref_data_pool*>(this), get_index(ref));
  }

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

  std::size_t size() const override { return _itemsPool.size(); }
  bool is_empty() const override { return _itemsPool.is_empty(); }

public:
  uint32_t get_ref_count(uint32_t index) const override {
    if (_itemsPool.is_out_of_range(index))
      return 0;
    return _itemsPool.at(index)._weak_ref_list.size;
  }

  uint32_t get_ref_count(const weak_ref& ref) const override { return get_ref_count(uint32_t(get_index(ref))); }

  // uint32_t get_ref_count(const value_type& target) {
  //   return get_ref_count(uint32_t(get_index(&target)));
  // }

public:
  int32_t get_index(const weak_ref& ref) const override { return ref._index; }
  // int32_t get_index(const value_type& target) const { return
  // _itemsPool.get_index(target); }

public:
  bool is_active(const weak_ref& ref) override { return ref.is_active(); }
  // bool is_active(std::size_t index) const override { return is_active(&_itemsPool.at(index)); }
  bool is_active(std::size_t index) const override {
    return reinterpret_cast<const internal_data*>(&_itemsPool.at(index))->_is_active;
  }
  // bool is_active(const value_type& target) const override { return reinterpret_cast<const
  // internal_data*>(&target)->_is_active; }

public:
  void release(const weak_ref& ref) override {
    if (!ref.is_active())
      return;
    release(ref._index);
  }
  // void release(std::size_t index) { release(&_itemsPool.at(index)); }

  // void release(const value_type& externalData) {
  // release(get_index(externalData)); }

  // void release(const value_type& externalData) {

  //   const int32_t index = get_index(externalData);

  void release(int32_t index) override {

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
  void remove_unreferenced_items() override {
    for (std::size_t index = 0; index < _itemsPool.size();) {
      if (_itemsPool.at(index)._weak_ref_list.size == 0) {
        release(int32_t(index));
      } else {
        ++index;
      }
    }
  }

  void filter(std::function<bool(value_type&)> callback) override {
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

  void filter(std::function<bool(value_type&, int)> callback) override {
    for (std::size_t index = 0; index < _itemsPool.size();) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false) {
        ++index;
        continue;
      }

      if (callback(item, int(index)) == false) {
        release(int32_t(index));
      } else {
        ++index;
      }
    }
  }

  void for_each(std::function<void(value_type&)> callback) override {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item);
  }

  void for_each(std::function<void(value_type&, int)> callback) override {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item, item._index);
  }

  void for_each(std::function<void(weak_ref)> callback) override {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(weak_ref(this, int32_t(item._index)));
  }

  void for_each(std::function<void(const value_type&)> callback) const override {
    for (const auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item);
  }

  void for_each(std::function<void(const value_type&, int)> callback) const override {
    for (const auto& item : _itemsPool)
      if (item._is_active == true)
        callback(item, item._index);
  }

  void for_each(std::function<void(weak_ref)> callback) const override {
    for (auto& item : _itemsPool)
      if (item._is_active == true)
        callback(weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(item._index)));
  }

  weak_ref find_if(std::function<bool(const value_type&)> callback) const override {
    for (std::size_t index = 0; index < _itemsPool.size(); ++index) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false)
        continue;

      if (callback(item) == true)
        return weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index));
    }
    return weak_ref::make_invalid();
  }

  weak_ref find_if(std::function<bool(const weak_ref)> callback) const override {
    for (std::size_t index = 0; index < _itemsPool.size(); ++index) {
      auto& item = _itemsPool.at(index);

      if (item._is_active == false)
        continue;

      auto currRef = weak_ref(const_cast<weak_ref_data_pool*>(this), int32_t(index));

      if (callback(currRef) == true)
        return currRef;
    }
    return weak_ref::make_invalid();
  }
};

class weak_ref_data_pool_base_class {
public:
  weak_ref_data_pool_base_class() = default;
  weak_ref_data_pool_base_class(const weak_ref_data_pool_base_class& other) = delete;            // block copy
  weak_ref_data_pool_base_class& operator=(const weak_ref_data_pool_base_class& other) = delete; // block copy

  weak_ref_data_pool_base_class(weak_ref_data_pool_base_class&& other) = default;
  weak_ref_data_pool_base_class& operator=(weak_ref_data_pool_base_class&& other) = default;

  virtual ~weak_ref_data_pool_base_class() = default;
};

template <typename InternalBaseType,
          typename PublicBaseType = InternalBaseType,
          std::size_t initial_size = 256,
          bool no_realloc = true,
          typename std::enable_if<std::is_base_of_v<weak_ref_data_pool_base_class, InternalBaseType>, bool>::type =
            true>
using safe_weak_ref_data_pool = weak_ref_data_pool<InternalBaseType, PublicBaseType, initial_size, no_realloc>;

} // namespace gero
