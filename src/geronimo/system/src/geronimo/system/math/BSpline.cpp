
#include "BSpline.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cmath>
#include <cstdint>

namespace gero {
namespace math {

namespace BaseFuncs {

float basisDeg2(float x) {
  if (-0.5f <= x && x < 0.5f)
    return 0.75f - x * x;
  if (0.5f <= x && x <= 1.5f)
    return 1.125f + (-1.5f + x / 2.0f) * x;
  if (-1.5f <= x && x < -0.5f)
    return 1.125f + (1.5f + x / 2.0f) * x;

  return 0;
}

float basisDeg3(float x) {
  if (-1 <= x && x < 0)
    return 2.0f / 3.0f + (-1.0f - x / 2.0f) * x * x;
  if (1 <= x && x <= 2)
    return 4.0f / 3.0f + x * (-2.0f + (1.0f - x / 6.0f) * x);
  if (-2 <= x && x < -1)
    return 4.0f / 3.0f + x * (2.0f + (1.0f + x / 6.0f) * x);
  if (0 <= x && x < 1)
    return 2.0f / 3.0f + (-1.0f + x / 2.0f) * x * x;

  return 0;
}

float basisDeg4(float x) {
  if (-1.5f <= x && x < -0.5f)
    return 55.0f / 96.0f + x * (-(5.0f / 24.0f) + x * (-(5.0f / 4.0f) + (-(5.0f / 6.0f) - x / 6.0f) * x));
  if (0.5f <= x && x < 1.5f)
    return 55.0f / 96.0f + x * (5.0f / 24.0f + x * (-(5.0f / 4.0f) + (5.0f / 6.0f - x / 6.0f) * x));
  if (1.5f <= x && x <= 2.5f)
    return 625.0f / 384.0f + x * (-(125.0f / 48.0f) + x * (25.0f / 16.0f + (-(5.0f / 12.0f) + x / 24.0f) * x));
  if (-2.5f <= x && x <= -1.5f)
    return 625.0f / 384.0f + x * (125.0f / 48.0f + x * (25.0f / 16.0f + (5.0f / 12.0f + x / 24.0f) * x));
  if (-1.5f <= x && x < 1.5f)
    return 115.0f / 192.0f + x * x * (-(5.0f / 8.0f) + x * x / 4.0f);

  return 0;
}

float basisDeg5(float x) {
  if (-2 <= x && x < -1)
    return 17.0f / 40.0f +
           x * (-(5.0f / 8.0f) + x * (-(7.0f / 4.0f) + x * (-(5.0f / 4.0f) + (-(3.0f / 8.0f) - x / 24.0f) * x)));
  if (0 <= x && x < 1)
    return 11.0f / 20.0f + x * x * (-(1.0f / 2.0f) + (1.0f / 4.0f - x / 12.0f) * x * x);
  if (2 <= x && x <= 3)
    return 81.0f / 40.0f +
           x * (-(27.0f / 8.0f) + x * (9.0f / 4.0f + x * (-(3.0f / 4.0f) + (1.0f / 8.0f - x / 120.0f) * x)));
  if (-3 <= x && x < -2)
    return 81.0f / 40.0f + x * (27.0f / 8.0f + x * (9.0f / 4.0f + x * (3.0f / 4.0f + (1.0f / 8.0f + x / 120.0f) * x)));
  if (1 <= x && x < 2)
    return 17.0f / 40.0f +
           x * (5.0f / 8.0f + x * (-(7.0f / 4.0f) + x * (5.0f / 4.0f + (-(3.0f / 8.0f) + x / 24.0f) * x)));
  if (-1 <= x && x < 0)
    return 11.0f / 20.0f + x * x * (-(1.0f / 2.0f) + (1.0f / 4.0f + x / 12.0f) * x * x);

  return 0;
}

} // namespace BaseFuncs

void BSpline::initialize(const Definition& inDef) {
  _def = inDef;

  if (!_def.getDataCallback)
    D_THROW(std::invalid_argument, "BSpline need getDataCallback, got nullptr");
  if (_def.knotsLength == 0)
    D_THROW(std::invalid_argument, "BSpline need knots, got emtpy length");
  if (_def.dimensions == 0)
    D_THROW(std::invalid_argument, "BSpline need positive dimensions, got 0");

  if (_def.degree <= 2) {
    _baseFunc = &BaseFuncs::basisDeg2;
    _rangeInterval = 2;
  } else if (_def.degree == 3) {
    _baseFunc = &BaseFuncs::basisDeg3;
    _rangeInterval = 2;
  } else if (_def.degree == 4) {
    _baseFunc = &BaseFuncs::basisDeg4;
    _rangeInterval = 3;
  } else if (_def.degree > 4) {
    _baseFunc = &BaseFuncs::basisDeg5;
    _rangeInterval = 3;
  }
}

float BSpline::calcAt(float inCoef, uint32_t inCurrDimension) const {
  if (_baseFunc == nullptr)
    D_THROW(std::invalid_argument, "not initialized");

  if (inCurrDimension >= _def.dimensions)
    D_THROW(std::invalid_argument, "dimension out of bound");

  if (inCoef < 0.0f || inCoef > 1.0f)
    D_THROW(std::invalid_argument, "coef must be in [0..1], input=" << inCoef);

  const float k_margin = float(_def.degree) + 1.0f;
  const float k_rawLength = float(_def.knotsLength) / float(_def.dimensions);

  inCoef = inCoef * (k_margin * 2.0f + k_rawLength);
  const int32_t coefInterval = int32_t(std::floor(inCoef));

  // const uint32_t i32KnotsLength = uint32_t(_def.knotsLength);
  const uint32_t k_dimensions = uint32_t(_def.dimensions);
  const uint32_t k_currDimension = uint32_t(inCurrDimension);

  float result = 0.0f;
  for (int32_t index = coefInterval - _rangeInterval; index <= coefInterval + _rangeInterval; ++index) {
    uint32_t valueIndex = 0;

    if (float(index) < k_margin) {
      valueIndex = k_currDimension;
    } else if (float(index) >= k_rawLength + k_margin) {
      valueIndex = uint32_t(k_rawLength - 1.0f) * k_dimensions + k_currDimension;
    } else {
      valueIndex = uint32_t(float(index) - k_margin) * k_dimensions + k_currDimension;
    }

    const float value = _def.getDataCallback(valueIndex);
    result += value * _baseFunc(inCoef - float(index));
  }

  return result;
}

} // namespace math
} // namespace gero
