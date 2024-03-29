
#pragma once

#include <random>

namespace gero {
namespace rng {

class RandomNumberGenerator {
private:
  static std::mt19937 _engine;

public:
  static void setSeed(uint32_t seed);
  static void ensureRandomSeed();

public:
  static float getNormalisedValue();

  template <typename T> static T getRangedValue(T min, T max) {
    return T(getRangedValue<double>(double(min), double(max)));
  }
};

template <> double RandomNumberGenerator::getRangedValue<double>(double min, double max);

using RNG = RandomNumberGenerator;

} // namespace rng
} // namespace gero
