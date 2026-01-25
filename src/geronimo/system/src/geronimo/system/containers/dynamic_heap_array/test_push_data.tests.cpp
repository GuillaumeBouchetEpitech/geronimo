
#include "headers.hpp"

#include "../generic_array_container_commons_tests/common.tests.hpp"

TEST_F(system_dynamic_heap_array, not_pre_allocated__push_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 0);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 1);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 2);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 2);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(4, "4"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, not_pre_allocated__push_back_move) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 0);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data1(1, "1");
  myDefaultDynamicArray.push_back(std::move(data1));
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 1);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data2(2, "2");
  myDefaultDynamicArray.push_back(std::move(data2));
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 2);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 2);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  common::TestStructure data3(3, "3");
  myDefaultDynamicArray.push_back(std::move(data3));
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  common::TestStructure data4(4, "4");
  myDefaultDynamicArray.push_back(std::move(data4));
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__push_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.push_back(common::TestStructure(4, "4"));
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__push_back_move) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data1(1, "1");
  myDefaultDynamicArray.push_back(std::move(data1));
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data2(2, "2");
  myDefaultDynamicArray.push_back(std::move(data2));
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data3(3, "3");
  myDefaultDynamicArray.push_back(std::move(data3));
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data4(4, "4");
  myDefaultDynamicArray.push_back(std::move(data4));
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, not_pre_allocated__emplace_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 0);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(1, "1");
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 1);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(2, "2");
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 2);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 1);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  myDefaultDynamicArray.emplace_back(3, "3");
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 2);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();

  myDefaultDynamicArray.emplace_back(4, "4");
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__emplace_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(1, "1");
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(2, "2");
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(3, "3");
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.emplace_back(4, "4");
  ASSERT_EQ(myDefaultDynamicArray.size(), 4);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 1);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}
