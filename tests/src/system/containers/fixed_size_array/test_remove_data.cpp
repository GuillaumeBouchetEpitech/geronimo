
#include "headers.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_fixed_size_array, pop_back) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  EXPECT_EQ(myDefaultFixedArray.size(), 4);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[3].value, 4);
  EXPECT_EQ(myDefaultFixedArray[3].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 4);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 4);
  EXPECT_EQ(common::getTotalDtor(), 4);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.pop_back();
  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, unsorted_erase) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  EXPECT_EQ(myDefaultFixedArray.size(), 4);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[3].value, 4);
  EXPECT_EQ(myDefaultFixedArray[3].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 4);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 4);
  EXPECT_EQ(common::getTotalDtor(), 4);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.unsorted_erase(1), 1); // remove middle
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 4);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "4");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 3);
  EXPECT_EQ(common::getTotalDtor(), 2);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.unsorted_erase(0), 1); // remove first
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 3);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[1].value, 4);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 3);
  EXPECT_EQ(common::getTotalDtor(), 2);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.unsorted_erase(1), 0); // remove last
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 3);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.unsorted_erase(0), 0); // remove only left
  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, sorted_erase) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  EXPECT_EQ(myDefaultFixedArray.size(), 4);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[3].value, 4);
  EXPECT_EQ(myDefaultFixedArray[3].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 4);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 4);
  EXPECT_EQ(common::getTotalDtor(), 4);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.sorted_erase(1), 2); // remove middle
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 3);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[2].value, 4);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 6);
  EXPECT_EQ(common::getTotalDtor(), 3);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.sorted_erase(0), 2); // remove first
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 3);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[1].value, 4);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 6);
  EXPECT_EQ(common::getTotalDtor(), 3);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.sorted_erase(1), 0); // remove last
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 3);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  EXPECT_EQ(myDefaultFixedArray.sorted_erase(0), 0); // remove only left
  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, clear) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;
  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  myDefaultFixedArray.push_back(common::TestStructure(4, "4"));

  EXPECT_EQ(myDefaultFixedArray.size(), 4);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");
  EXPECT_EQ(myDefaultFixedArray[3].value, 4);
  EXPECT_EQ(myDefaultFixedArray[3].my_string, "4");

  EXPECT_EQ(common::getTotalCtor(), 4);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 4);
  EXPECT_EQ(common::getTotalDtor(), 4);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.clear();
  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 4);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}
