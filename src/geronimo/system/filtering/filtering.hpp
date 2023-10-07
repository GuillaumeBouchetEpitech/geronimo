
#pragma once

#include <cstdint>
#include <array>

namespace gero {
namespace filtering {

  template<typename T>
  T setBit(T src, int32_t index)
  {
    return src | T(1 << index);
  }
  template<typename T, typename ... Args>
  T setBits(T src, Args ... args)
  {
    std::array<int32_t, sizeof...(args)> allIndices{{ args... }};

    for (int32_t index : allIndices)
      src = setBit(src, index);
    return src;
  }

  template<typename T>
  T unsetBit(T src, int32_t index)
  {
    return src & ~T(1 << index);
  }
  template<typename T, typename ... Args>
  T unsetBits(T src, Args ... args)
  {
    std::array<int32_t, sizeof...(args)> allIndices{{ args... }};

    for (int32_t index : allIndices)
      src = unsetBit(src, index);
    return src;
  }

  template<typename T>
  bool containsBit(T src, int32_t index)
  {
    return (src & T(1 << index)) != 0;
  }

  template<typename T, typename ... Args>
  bool containsBits(T src, Args ... args)
  {
    std::array<int32_t, sizeof...(args)> allIndices{{ args... }};

    for (int32_t index : allIndices)
      if (containsBit(src, index) == false)
        return false;
    return true;
  }

  template<typename T>
  bool anyBitsMatches(T valA, T valB)
  {
    return (valA & valB) != 0;
  }


} // namespace fileUtils
} // namespace gero


