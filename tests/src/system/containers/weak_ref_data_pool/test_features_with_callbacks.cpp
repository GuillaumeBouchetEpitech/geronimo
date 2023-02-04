
#include "headers.hpp"

TEST(system_weak_ref_data_pool, can_filter_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref0 = myPool.acquire(0, "test");
  auto ref1 = myPool.acquire(1, "test");
  auto ref2 = myPool.acquire(2, "test");
  auto ref3 = myPool.acquire(3, "test");
  auto ref4 = myPool.acquire(4, "test");
  auto ref5 = myPool.acquire(5, "test");
  auto ref6 = myPool.acquire(6, "test");
  auto ref7 = myPool.acquire(7, "test");
  auto ref8 = myPool.acquire(8, "test");
  auto ref9 = myPool.acquire(9, "test");

  ASSERT_EQ(myPool.size(), 10);
  for (uint32_t ii = 0; ii < 10U; ++ii) {
    ASSERT_EQ(myPool.get(ii).get()->value, int32_t(ii));
    ASSERT_EQ(myPool.get(ii).get()->my_string, "test");
  }

  myPool.filter([](common::TestStructure& currValue) {
    return (currValue.value % 2 == 0);
  });

  ASSERT_EQ(myPool.size(), 5);

  ASSERT_EQ(ref0, true);
  ASSERT_EQ(ref0.is_active(), true);
  ASSERT_EQ(ref2, true);
  ASSERT_EQ(ref2.is_active(), true);
  ASSERT_EQ(ref4, true);
  ASSERT_EQ(ref4.is_active(), true);
  ASSERT_EQ(ref6, true);
  ASSERT_EQ(ref6.is_active(), true);
  ASSERT_EQ(ref8, true);
  ASSERT_EQ(ref8.is_active(), true);

  ASSERT_EQ(ref1, false);
  ASSERT_EQ(ref1.is_active(), false);
  ASSERT_EQ(ref3, false);
  ASSERT_EQ(ref3.is_active(), false);
  ASSERT_EQ(ref5, false);
  ASSERT_EQ(ref5.is_active(), false);
  ASSERT_EQ(ref7, false);
  ASSERT_EQ(ref7.is_active(), false);
  ASSERT_EQ(ref9, false);
  ASSERT_EQ(ref9.is_active(), false);

  ASSERT_EQ(myPool.get(0).get()->value, 0);
  ASSERT_EQ(myPool.get(0).get()->my_string, "test");
  ASSERT_EQ(myPool.get(1).get()->value, 8);
  ASSERT_EQ(myPool.get(1).get()->my_string, "test");
  ASSERT_EQ(myPool.get(2).get()->value, 2);
  ASSERT_EQ(myPool.get(2).get()->my_string, "test");
  ASSERT_EQ(myPool.get(3).get()->value, 6);
  ASSERT_EQ(myPool.get(3).get()->my_string, "test");
  ASSERT_EQ(myPool.get(4).get()->value, 4);
  ASSERT_EQ(myPool.get(4).get()->my_string, "test");
}

TEST(system_weak_ref_data_pool, can_loop_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref0 = myPool.acquire(0, "test");
  auto ref1 = myPool.acquire(1, "test");
  auto ref2 = myPool.acquire(2, "test");
  auto ref3 = myPool.acquire(3, "test");
  auto ref4 = myPool.acquire(4, "test");
  auto ref5 = myPool.acquire(5, "test");
  auto ref6 = myPool.acquire(6, "test");
  auto ref7 = myPool.acquire(7, "test");
  auto ref8 = myPool.acquire(8, "test");
  auto ref9 = myPool.acquire(9, "test");

  ASSERT_EQ(myPool.size(), 10);

  {
    int index = 0;
    myPool.for_each([&index](common::TestStructure& currValue) {
      ASSERT_EQ(currValue.value, index);
      ASSERT_EQ(currValue.my_string, "test");
      ++index;
    });
    ASSERT_EQ(index, 10);
  }

  {
    const auto& cmyPool = myPool;
    int index = 0;
    cmyPool.for_each([&index](const common::TestStructure& currValue) {
      ASSERT_EQ(currValue.value, index);
      ASSERT_EQ(currValue.my_string, "test");
      ++index;
    });
    ASSERT_EQ(index, 10);
  }
}

TEST(system_weak_ref_data_pool, can_find_if) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref0 = myPool.acquire(0, "test");
  auto ref1 = myPool.acquire(1, "test");
  auto ref2 = myPool.acquire(2, "test");
  auto ref3 = myPool.acquire(3, "test");
  auto ref4 = myPool.acquire(4, "test");
  auto ref5 = myPool.acquire(5, "test");
  auto ref6 = myPool.acquire(6, "test");
  auto ref7 = myPool.acquire(7, "test");
  auto ref8 = myPool.acquire(8, "test");
  auto ref9 = myPool.acquire(9, "test");

  ASSERT_EQ(myPool.size(), 10);

  {
    auto foundRef =
      myPool.find_if([](const common::TestStructure& currValue) -> bool {
        return (currValue.value == 6);
      });
    ASSERT_EQ(foundRef.is_active(), true);
    ASSERT_EQ(foundRef, true);
    ASSERT_EQ(foundRef->value, 6);
    ASSERT_EQ(foundRef->my_string, "test");
  }

  {
    const auto& cmyPool = myPool;
    auto foundRef =
      cmyPool.find_if([](const common::TestStructure& currValue) -> bool {
        return (currValue.value == 6);
      });
    ASSERT_EQ(foundRef.is_active(), true);
    ASSERT_EQ(foundRef, true);
    ASSERT_EQ(foundRef->value, 6);
    ASSERT_EQ(foundRef->my_string, "test");
  }
}
