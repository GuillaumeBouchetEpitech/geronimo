
#include "headers.hpp"

// #include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_fixed_size_array, pop_back) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  ASSERT_EQ(myDefaultFixedArray.size(), 4);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[3].value, 4);
  ASSERT_EQ(myDefaultFixedArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  ASSERT_EQ(myDefaultFixedArray.size(), 3);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  ASSERT_EQ(myDefaultFixedArray.size(), 2);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  ASSERT_EQ(myDefaultFixedArray.size(), 1);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  ASSERT_EQ(myDefaultFixedArray.size(), 0);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, unsorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  ASSERT_EQ(myDefaultFixedArray.size(), 4);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[3].value, 4);
  ASSERT_EQ(myDefaultFixedArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.unsorted_erase(1), 1); // remove middle
  ASSERT_EQ(myDefaultFixedArray.size(), 3);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 4);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "4");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.unsorted_erase(0), 1); // remove first
  ASSERT_EQ(myDefaultFixedArray.size(), 2);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 3);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[1].value, 4);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 3);
  ASSERT_EQ(common::getTotalDtor(), 2);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.unsorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultFixedArray.size(), 1);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 3);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.unsorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultFixedArray.size(), 0);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, sorted_erase) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  ASSERT_EQ(myDefaultFixedArray.size(), 4);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[3].value, 4);
  ASSERT_EQ(myDefaultFixedArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.sorted_erase(1), 2); // remove middle
  ASSERT_EQ(myDefaultFixedArray.size(), 3);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 3);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[2].value, 4);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.sorted_erase(0), 2); // remove first
  ASSERT_EQ(myDefaultFixedArray.size(), 2);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 3);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[1].value, 4);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 6);
  ASSERT_EQ(common::getTotalDtor(), 3);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.sorted_erase(1), 0); // remove last
  ASSERT_EQ(myDefaultFixedArray.size(), 1);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 3);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "3");

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  ASSERT_EQ(myDefaultFixedArray.sorted_erase(0), 0); // remove only left
  ASSERT_EQ(myDefaultFixedArray.size(), 0);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 1);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, clear) {
  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 0);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>>
    myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  ASSERT_EQ(myDefaultFixedArray.size(), 4);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), false);
  ASSERT_EQ(myDefaultFixedArray[0].value, 1);
  ASSERT_EQ(myDefaultFixedArray[0].my_string, "1");
  ASSERT_EQ(myDefaultFixedArray[1].value, 2);
  ASSERT_EQ(myDefaultFixedArray[1].my_string, "2");
  ASSERT_EQ(myDefaultFixedArray[2].value, 3);
  ASSERT_EQ(myDefaultFixedArray[2].my_string, "3");
  ASSERT_EQ(myDefaultFixedArray[3].value, 4);
  ASSERT_EQ(myDefaultFixedArray[3].my_string, "4");

  ASSERT_EQ(common::getTotalCtor(), 4);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 4);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.clear();
  ASSERT_EQ(myDefaultFixedArray.size(), 0);
  ASSERT_EQ(myDefaultFixedArray.capacity(), 5);
  ASSERT_EQ(myDefaultFixedArray.is_empty(), true);

  ASSERT_EQ(common::getTotalCtor(), 0);
  ASSERT_EQ(common::getTotalCopyCtor(), 0);
  ASSERT_EQ(common::getTotalMoveCtor(), 0);
  ASSERT_EQ(common::getTotalDtor(), 4);
  ASSERT_EQ(common::getTotalAlloc(), 0);
  ASSERT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}
