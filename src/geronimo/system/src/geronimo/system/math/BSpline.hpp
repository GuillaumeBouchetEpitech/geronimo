
#pragma once

#include <cstdint>
#include <functional>
#include <vector>

namespace gero {
namespace math {

class BSpline {
public:
  using GetDataCallback = std::function<float(uint32_t)>;

  struct Definition {
    GetDataCallback getDataCallback;
    std::size_t knotsLength;
    uint32_t dimensions;
    uint32_t degree;
  };

public:
  void initialize(const Definition& inDef);
  float calcAt(float inCoef, uint32_t inDimension) const;

private:
  using BaseFunc = float (*)(float);

  Definition _def;
  BaseFunc _baseFunc = nullptr;
  int32_t _rangeInterval;
};

} // namespace math
} // namespace gero
