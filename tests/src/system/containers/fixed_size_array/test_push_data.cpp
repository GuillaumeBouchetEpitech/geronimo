
#include "headers.hpp"

#include "system/containers/generic_array_container_commons/common.hpp"

TEST_F(system_fixed_size_array, push_back_copy) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;

  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.push_back(common::TestStructure(1, "1"));
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.push_back(common::TestStructure(2, "2"));
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.push_back(common::TestStructure(3, "3"));
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

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

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 1);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, push_back_move) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;

  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data1(1, "1");
  myDefaultFixedArray.push_back(std::move(data1));
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data2(2, "2");
  myDefaultFixedArray.push_back(std::move(data2));
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data3(3, "3");
  myDefaultFixedArray.push_back(std::move(data3));
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  common::TestStructure data4(4, "4");
  myDefaultFixedArray.push_back(std::move(data4));
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

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 1);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}

TEST_F(system_fixed_size_array, emplace_back) {
  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  gero::fixed_size_array<common::TestStructure, common::TestStructure, 5, common::MyAllocator<common::TestStructure>> myDefaultFixedArray;

  EXPECT_EQ(myDefaultFixedArray.size(), 0);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), true);

  EXPECT_EQ(common::getTotalCtor(), 0);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.emplace_back(1, "1");
  EXPECT_EQ(myDefaultFixedArray.size(), 1);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.emplace_back(2, "2");
  EXPECT_EQ(myDefaultFixedArray.size(), 2);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.emplace_back(3, "3");
  EXPECT_EQ(myDefaultFixedArray.size(), 3);
  EXPECT_EQ(myDefaultFixedArray.capacity(), 5);
  EXPECT_EQ(myDefaultFixedArray.is_empty(), false);
  EXPECT_EQ(myDefaultFixedArray[0].value, 1);
  EXPECT_EQ(myDefaultFixedArray[0].my_string, "1");
  EXPECT_EQ(myDefaultFixedArray[1].value, 2);
  EXPECT_EQ(myDefaultFixedArray[1].my_string, "2");
  EXPECT_EQ(myDefaultFixedArray[2].value, 3);
  EXPECT_EQ(myDefaultFixedArray[2].my_string, "3");

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();

  myDefaultFixedArray.emplace_back(4, "4");
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

  EXPECT_EQ(common::getTotalCtor(), 1);
  EXPECT_EQ(common::getTotalCopyCtor(), 0);
  EXPECT_EQ(common::getTotalMoveCtor(), 0);
  EXPECT_EQ(common::getTotalDtor(), 0);
  EXPECT_EQ(common::getTotalAlloc(), 0);
  EXPECT_EQ(common::getTotalDealloc(), 0);
  common::reset();
}
