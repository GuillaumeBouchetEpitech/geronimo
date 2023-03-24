
#pragma once

#include <vector>

/**
 * DataPool
 *
 * simple and fast contiguous/aligned memory pool of objects
 */
template <typename DataType> class DataPool {
private:
  struct InternalData : public DataType {
  public:
    uint32_t _index;
  };

private:
  std::vector<InternalData> _allItems;
  std::vector<int32_t> _unusedIndices;

public:
  DataPool(std::size_t poolSize = 128) {
    _allItems.resize(poolSize);
    _unusedIndices.reserve(poolSize);
    for (std::size_t ii = 0; ii < poolSize; ++ii)
      _unusedIndices.push_back(ii);
  }

  void grow(std::size_t extraSize) {
    // make the pool grow here
    const std::size_t currSize = _allItems.size();
    const std::size_t nextSize = currSize + extraSize;

    // here a simple resize is enough
    _allItems.resize(nextSize);

    // here we need to fill it with the newly created indexes
    _unusedIndices.reserve(nextSize);
    for (std::size_t ii = currSize; ii < nextSize; ++ii)
      _unusedIndices.push_back(ii);
  }

  DataType* pop() {
    if (_unusedIndices.empty())
      grow(_allItems.size() * 2);

    const int32_t index = _unusedIndices.back();
    _unusedIndices.pop_back();
    InternalData* pData = &_allItems[index];
    pData->_index = index;
    return pData;
  }

  inline DataType* get(uint32_t index) { return reinterpret_cast<DataType*>(&_allItems[index]); }

  inline uint32_t getIndex(DataType* pTarget) const { return reinterpret_cast<InternalData*>(pTarget)->_index; }

  inline void release(uint32_t index) { _unusedIndices.push_back(index); }
};
