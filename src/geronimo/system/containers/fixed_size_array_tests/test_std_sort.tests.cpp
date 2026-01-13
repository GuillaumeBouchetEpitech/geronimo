
#include "headers.hpp"

// #include "system/containers/generic_array_container_commons/common.hpp"

#include <algorithm>

TEST_F(system_fixed_size_array, std_sort) {

  gero::fixed_size_array<int, int, 10> myDefaultFixedArray;

  myDefaultFixedArray.push_back(666);
  myDefaultFixedArray.push_back(555);
  myDefaultFixedArray.push_back(444);
  myDefaultFixedArray.push_back(333);
  myDefaultFixedArray.push_back(222);
  myDefaultFixedArray.push_back(111);

  std::sort(myDefaultFixedArray.begin(), myDefaultFixedArray.end());

  ASSERT_EQ(myDefaultFixedArray.at(0), 111);
  ASSERT_EQ(myDefaultFixedArray.at(1), 222);
  ASSERT_EQ(myDefaultFixedArray.at(2), 333);
  ASSERT_EQ(myDefaultFixedArray.at(3), 444);
  ASSERT_EQ(myDefaultFixedArray.at(4), 555);
  ASSERT_EQ(myDefaultFixedArray.at(5), 666);
}
