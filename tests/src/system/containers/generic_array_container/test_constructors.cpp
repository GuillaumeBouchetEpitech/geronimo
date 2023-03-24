
#include "headers.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_generic_array_container, construct_with_empty_size) {
  shorthand_dynamic_heap_array<0> myDefaultDynamicArray;
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 0);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  gero::dynamic_heap_array<common::TestStructure> mySizedDynamicArray;
  ASSERT_EQ(mySizedDynamicArray.size(), 0);
  ASSERT_EQ(mySizedDynamicArray.capacity(), 0);
  ASSERT_EQ(mySizedDynamicArray.is_empty(), true);
}

TEST_F(system_generic_array_container, construct_with_size_of_5) {
  gero::static_array<common::TestStructure, 5> mySaticArray;
  ASSERT_EQ(mySaticArray.size(), 5);
  ASSERT_EQ(mySaticArray.is_empty(), false);

  shorthand_dynamic_heap_array<5> mySizedDynamicArray;
  ASSERT_EQ(mySizedDynamicArray.size(), 0);
  ASSERT_EQ(mySizedDynamicArray.capacity(), 5);
  ASSERT_EQ(mySizedDynamicArray.is_empty(), true);

  mySizedDynamicArray.ensure_size(5);
  ASSERT_EQ(mySizedDynamicArray.size(), 5);
  ASSERT_EQ(mySizedDynamicArray.capacity(), 5);
  ASSERT_EQ(mySizedDynamicArray.is_empty(), false);
}

TEST_F(system_generic_array_container, comparison_operators) {
  {
    gero::static_array<common::TestStructure, 5> mySaticArrayA;
    gero::static_array<common::TestStructure, 5> mySaticArrayB;
    ASSERT_EQ(mySaticArrayA == mySaticArrayB, false);
    ASSERT_EQ(mySaticArrayA != mySaticArrayB, true);
  }

  {
    gero::dynamic_heap_array<common::TestStructure> myDynamicArrayA;
    gero::dynamic_heap_array<common::TestStructure> myDynamicArrayB;
    ASSERT_EQ(myDynamicArrayA == myDynamicArrayB, false);
    ASSERT_EQ(myDynamicArrayA != myDynamicArrayB, true);
  }
}

TEST_F(system_generic_array_container, do_not_call_any_ctor_when_moving_containers) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);

  auto mySizedDynamicArray1 = std::make_unique<gero::dynamic_heap_array<common::TestStructure,
                                                                        common::TestStructure,
                                                                        5,
                                                                        common::MyAllocator<common::TestStructure>>>();

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  mySizedDynamicArray1->ensure_size(5);

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  for (std::size_t ii = 0; ii < mySizedDynamicArray1->size(); ++ii)
    (*mySizedDynamicArray1)[int(ii)].value = int(ii);

  auto mySizedDynamicArray2 = std::move(mySizedDynamicArray1);
  ASSERT_EQ(mySizedDynamicArray1, nullptr);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  mySizedDynamicArray2.reset(nullptr);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 5);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();
}
