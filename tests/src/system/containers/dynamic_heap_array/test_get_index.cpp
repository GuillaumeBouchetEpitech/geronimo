
// #include "headers.hpp"

// #include "system/containers/generic_array_container_commons/common.hpp"

// TEST(dynamic_heap_array, get_index) {

//   dynamic_heap_array<common::TestStructure> myDefaultDynamicArray;
//   ASSERT_EQ(myDefaultDynamicArray.size(), 0);
//   ASSERT_EQ(myDefaultDynamicArray.capacity(), 0);
//   ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

//   myDefaultDynamicArray.pre_allocate(10);

//   ASSERT_EQ(myDefaultDynamicArray.size(), 0);
//   ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
//   ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

//   for (int ii = 0; ii < 10; ++ii)
//   {
//     myDefaultDynamicArray.emplace_back(ii);
//   }

//   ASSERT_EQ(myDefaultDynamicArray.size(), 10);
//   ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
//   ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);

//   for (int ii = 0; ii < 10; ++ii)
//   {
//     ASSERT_EQ(myDefaultDynamicArray.at(ii).value, ii);
//     ASSERT_EQ(myDefaultDynamicArray.get_index(myDefaultDynamicArray.at(ii)),
//     ii);
//   }

//   const common::TestStructure* fakedData = (common::TestStructure*)(666);
//   ASSERT_EQ(myDefaultDynamicArray.get_index(*fakedData), -1);
// }
