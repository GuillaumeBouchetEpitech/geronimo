
#include "./DeterministicRng.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <unordered_set>

#include "gtest/gtest.h"

TEST(system_deterministic_rng, can_generate_10000_unique_random_values) {
  gero::rng::DeterministicRng rng;

  constexpr int testSize = 10000;

  std::unordered_set<int64_t> allValues;
  for (int index = 0; index < testSize; ++index) {
    const int64_t value = rng.getValue();

    ASSERT_EQ(allValues.count(value), 0);
    ASSERT_LT(value, rng.getMaxValue());

    allValues.insert(value);
  }

  ASSERT_EQ(allValues.size(), testSize);
}

TEST(system_deterministic_rng, can_generate_10000_uniformed_random_values) {
  gero::rng::DeterministicRng rng;

  constexpr int testSize = 10000;

  for (int index = 0; index < testSize; ++index) {
    const float value = rng.getNormalisedValue();

    ASSERT_GE(value, 0.0f);
    ASSERT_LT(value, 1.0f);
  }
}

TEST(system_deterministic_rng, can_generate_10000_random_but_deterministic_values) {
  constexpr int32_t k_seed = 666;

  gero::rng::DeterministicRng rng;
  rng.setSeed(k_seed);

  constexpr int testSize = 10000;

  std::vector<int64_t> allValues;
  allValues.reserve(testSize);
  for (int index = 0; index < testSize; ++index)
    allValues.push_back(rng.getValue());

  ASSERT_EQ(allValues.size(), testSize);

  rng.setSeed(k_seed);
  for (std::size_t index = 0; index < testSize; ++index) {
    ASSERT_EQ(allValues[index], rng.getValue());
  }
}
