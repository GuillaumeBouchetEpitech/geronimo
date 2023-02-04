
#include "headers.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_dynamic_heap_array,
       not_pre_allocated__pre_allocate) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.pre_allocate(5);

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
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__pre_allocate_same) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.pre_allocate(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__pre_allocate_less) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.pre_allocate(3);

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__pre_allocate_more) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.pre_allocate(10);

  ASSERT_EQ(myDefaultDynamicArray.size(), 0);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       not_pre_allocated__ensure_size) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_same) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_less) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.ensure_size(3);

  ASSERT_EQ(myDefaultDynamicArray.size(), 3);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 3);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_more) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;

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

  myDefaultDynamicArray.ensure_size(10);

  ASSERT_EQ(myDefaultDynamicArray.size(), 10);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[5].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[5].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[6].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[6].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[7].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[7].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[8].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[8].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[9].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[9].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 10);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       not_pre_allocated__ensure_size_grow_existing) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 0, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;
  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 5);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.ensure_size(10);

  ASSERT_EQ(myDefaultDynamicArray.size(), 10);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[5].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[5].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[6].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[6].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[7].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[7].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[8].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[8].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[9].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[9].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 5);
  ASSERT_EQ(common::getTotalDtor(), 5);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_grow_existing_same) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 10, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;
  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.ensure_size(10);

  ASSERT_EQ(myDefaultDynamicArray.size(), 10);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[5].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[5].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[6].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[6].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[7].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[7].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[8].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[8].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[9].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[9].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_grow_existing_less) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 10, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;
  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.ensure_size(8);

  ASSERT_EQ(myDefaultDynamicArray.size(), 8);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[5].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[5].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[6].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[6].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[7].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[7].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 3);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_dynamic_heap_array,
       is_pre_allocated__ensure_size_grow_existing_more) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::dynamic_heap_array<common::TestStructure, common::TestStructure, 10, common::MyAllocator<common::TestStructure>> myDefaultDynamicArray;
  myDefaultDynamicArray.ensure_size(5);

  ASSERT_EQ(myDefaultDynamicArray.size(), 5);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 10);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);

  ASSERT_EQ(common::getTotalCtor(), 5);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultDynamicArray.ensure_size(15);

  ASSERT_EQ(myDefaultDynamicArray.size(), 15);
  ASSERT_EQ(myDefaultDynamicArray.capacity(), 15);
  ASSERT_EQ(myDefaultDynamicArray.is_empty(), false);
  ASSERT_EQ(myDefaultDynamicArray[0].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[0].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[1].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[1].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[2].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[2].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[3].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[3].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[4].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[4].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[5].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[5].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[6].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[6].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[7].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[7].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[8].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[8].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[9].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[9].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[10].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[10].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[11].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[11].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[12].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[12].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[13].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[13].my_string, "");
  ASSERT_EQ(myDefaultDynamicArray[14].value, 0);
  ASSERT_EQ(myDefaultDynamicArray[14].my_string, "");

  ASSERT_EQ(common::getTotalCtor(), 10);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 5);
  ASSERT_EQ(common::getTotalDtor(), 5);
  ASSERT_EQ(common::getTotalAlloc(), 1);
  ASSERT_EQ(common::getTotalDealloc(), 1);
  common::reset();
}
