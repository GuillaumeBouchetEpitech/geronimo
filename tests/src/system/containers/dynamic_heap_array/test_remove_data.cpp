
#include "headers.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_dynamic_heap_array, not_pre_allocated__pop_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 7);
  ASSERT_EQ(common::getTotalDtor(), 7);
  ASSERT_EQ(common::getTotalAlloc(), 3);
  ASSERT_EQ(common::getTotalDealloc(), 2);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__pop_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 2);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.pop_back();
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, not_pre_allocated__unsorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 7);
  ASSERT_EQ(common::getTotalDtor(), 7);
  ASSERT_EQ(common::getTotalAlloc(), 3);
  ASSERT_EQ(common::getTotalDealloc(), 2);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(1), 1); // remove middle

  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(0), 1); // remove first
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__unsorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(1), 1); // remove middle
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(0), 1); // remove first
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.unsorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, not_pre_allocated__sorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 7);
  ASSERT_EQ(common::getTotalDtor(), 7);
  ASSERT_EQ(common::getTotalAlloc(), 3);
  ASSERT_EQ(common::getTotalDealloc(), 2);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(1), 2); // remove middle
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(0), 2); // remove first
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__sorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(1), 2); // remove middle
  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 1);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "1");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(0), 2); // remove first
  ASSERT_EQ(myDefaultDynamicArray.size(), 2);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 4);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultDynamicArray.size(), 1);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 3);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultDynamicArray.sorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, not_pre_allocated__clear) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 7);
  ASSERT_EQ(common::getTotalDtor(), 7);
  ASSERT_EQ(common::getTotalAlloc(), 3);
  ASSERT_EQ(common::getTotalDealloc(), 2);
  common::reset();

  myDefaultDynamicArray.clear();
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 4);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array, is_pre_allocated__clear) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultDynamicArray;
  myDefaultDynamicArray.push_back(common::TestStructure(1, "1"));
  myDefaultDynamicArray.push_back(common::TestStructure(2, "2"));
  myDefaultDynamicArray.push_back(common::TestStructure(3, "3"));
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

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.clear();
  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}
