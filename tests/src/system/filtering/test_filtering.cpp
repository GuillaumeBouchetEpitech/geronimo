
#include "geronimo/system/filtering/filtering.hpp"

#include "gtest/gtest.h"

//
//
//

TEST(filtering, can_check_if_contain_bit) {
  const int32_t input = (1 << 1) | (1 << 6) | (1 << 11);

  ASSERT_EQ(gero::filtering::containsBit(input, 0), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 1), true);
  ASSERT_EQ(gero::filtering::containsBit(input, 2), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 3), false);

  ASSERT_EQ(gero::filtering::containsBit(input, 4), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 5), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 6), true);
  ASSERT_EQ(gero::filtering::containsBit(input, 7), false);

  ASSERT_EQ(gero::filtering::containsBit(input, 8), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 9), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 10), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 11), true);

  ASSERT_EQ(gero::filtering::containsBit(input, 12), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 13), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 14), false);
  ASSERT_EQ(gero::filtering::containsBit(input, 15), false);

  ASSERT_EQ(gero::filtering::containsBits(input, 1, 6, 11), true);
  ASSERT_EQ(gero::filtering::containsBits(input, 0, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 14, 15), false);
}

TEST(filtering, can_check_if_any_bits_matches) {

  const int32_t input = (1 << 1) | (1 << 6) | (1 << 11);

  ASSERT_EQ(gero::filtering::anyBitsMatches(input, 0), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, input), true);

  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 0)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 1)), true);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 2)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 3)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 4)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 5)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 6)), true);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 7)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 8)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 9)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 10)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 11)), true);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 12)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 13)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 14)), false);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 15)), false);


  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 1) | (1 << 2)), true);
  ASSERT_EQ(gero::filtering::anyBitsMatches(input, (1 << 2) | (1 << 3)), false);

}

TEST(filtering, can_set_bit) {

  const int32_t origin = 0;
  const int32_t expected1 = (1 << 1);
  const int32_t expected2 = expected1 | (1 << 2);

  const int32_t result1 = gero::filtering::setBit(origin, 1);

  ASSERT_EQ(result1, expected1);
  ASSERT_EQ(gero::filtering::containsBits(result1, 1), true);
  ASSERT_EQ(gero::filtering::containsBits(result1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), false);

  const int32_t result2 = gero::filtering::setBit(result1, 2);

  ASSERT_EQ(result2, expected2);
  ASSERT_EQ(gero::filtering::containsBits(result2, 1, 2), true);
  ASSERT_EQ(gero::filtering::containsBits(result2, 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), false);
}

TEST(filtering, can_set_bits) {

  const int32_t origin = 0;
  const int32_t expected = (1 << 1) | (1 << 2) | (1 << 6) | (1 << 13);

  const int32_t result = gero::filtering::setBits(origin, 1, 2, 6, 13);

  ASSERT_EQ(result, expected);
  ASSERT_EQ(gero::filtering::containsBits(result, 1, 2, 6, 13), true);
  ASSERT_EQ(gero::filtering::containsBits(result, 0, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 15), false);
}

TEST(filtering, can_unset_bit) {

  const int32_t origin = (1 << 1) | (1 << 6);
  const int32_t expected1 = (1 << 6);
  const int32_t expected2 = 0;

  const int32_t result1 = gero::filtering::unsetBit(origin, 1);

  ASSERT_EQ(result1, expected1);
  ASSERT_EQ(gero::filtering::containsBits(result1, 6), true);
  ASSERT_EQ(gero::filtering::containsBits(result1, 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15), false);

  const int32_t result2 = gero::filtering::unsetBit(result1, 6);

  ASSERT_EQ(result2, expected2);
  // ASSERT_EQ(gero::filtering::containsBits(result2, 6), true);
  ASSERT_EQ(gero::filtering::containsBits(result2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), false);
}

TEST(filtering, can_unset_bits) {

  const int32_t origin = (1 << 1) | (1 << 2) | (1 << 6) | (1 << 13);
  const int32_t expected = 0;

  const int32_t result = gero::filtering::unsetBits(origin, 1, 2, 6, 13);

  ASSERT_EQ(result, expected);
  // ASSERT_EQ(gero::filtering::containsBits(result, 1, 2, 6, 13), true);
  ASSERT_EQ(gero::filtering::containsBits(result, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), false);
}
