
#include "headers.hpp"

#include <algorithm>

TEST_F(system_dynamic_heap_array, std_sort) {

  gero::dynamic_heap_array<int, int, 0> myDynamicArray;

  myDynamicArray.push_back(666);
  myDynamicArray.push_back(555);
  myDynamicArray.push_back(444);
  myDynamicArray.push_back(333);
  myDynamicArray.push_back(222);
  myDynamicArray.push_back(111);

  std::sort(myDynamicArray.begin(), myDynamicArray.end());

  ASSERT_EQ(myDynamicArray.at(0), 111);
  ASSERT_EQ(myDynamicArray.at(1), 222);
  ASSERT_EQ(myDynamicArray.at(2), 333);
  ASSERT_EQ(myDynamicArray.at(3), 444);
  ASSERT_EQ(myDynamicArray.at(4), 555);
  ASSERT_EQ(myDynamicArray.at(5), 666);

}
