
#include "headers.hpp"

TEST(system_weak_ref_data_pool,
     can_acquire_weak_ref_and_get_the_values_from_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->value, 555);
  ASSERT_EQ(ref->my_string, "test");
}

TEST(system_weak_ref_data_pool,
     can_acquire_weak_ref_and_copy_and_move_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto mainRef = myPool.acquire(555, "test");
  auto copiedRef1 = mainRef;    // copy
  auto copiedRef2 = copiedRef1; // copy

  auto movedRef = copiedRef2;            // copy
  auto copiedRef3 = std::move(movedRef); // move

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(mainRef), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 4);
  ASSERT_EQ(mainRef.is_active(), true);
  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(movedRef.is_active(), false);
  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(mainRef, true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(movedRef, false);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(mainRef.get(), myPool.get(0).get());
  ASSERT_EQ(movedRef.get(), nullptr);
  ASSERT_EQ(mainRef.get(), copiedRef1.get());
  ASSERT_EQ(mainRef.get(), copiedRef2.get());
  ASSERT_EQ(mainRef.get(), copiedRef3.get());

  ASSERT_EQ(mainRef->value, 555);
  ASSERT_EQ(mainRef->my_string, "test");
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");
  ASSERT_EQ(copiedRef2->value, 555);
  ASSERT_EQ(copiedRef2->my_string, "test");
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");
}

TEST(system_weak_ref_data_pool,
     can_acquire_weak_ref_and_get_and_set_the_values_of_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->value, 555);
  ASSERT_EQ(ref->my_string, "test");

  ref->value += 100;
  ref->my_string += " test";

  ASSERT_EQ(ref->value, 655);
  ASSERT_EQ(ref->my_string, "test test");
}

TEST(system_weak_ref_data_pool,
     can_acquire_weak_ref_and_get_total_ref_count_from_the_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(ref), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->value, 555);
  ASSERT_EQ(ref->my_string, "test");

  ref.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(ref.is_active(), false);
  ASSERT_EQ(ref, false);
}

TEST(system_weak_ref_data_pool,
     can_acquire_weak_ref_and_invalidate_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto mainRef = myPool.acquire(555, "test");
  auto copiedRef1 = mainRef;    // copy
  auto copiedRef2 = copiedRef1; // copy
  auto copiedRef3 = copiedRef2; // copy

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(mainRef), 0);
  ASSERT_EQ(myPool.get_index(copiedRef1), 0);
  ASSERT_EQ(myPool.get_index(copiedRef2), 0);
  ASSERT_EQ(myPool.get_index(copiedRef3), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 4);

  ASSERT_EQ(mainRef.is_active(), true);
  ASSERT_EQ(mainRef, true);
  ASSERT_EQ(mainRef.get(), myPool.get(0).get());
  ASSERT_EQ(mainRef->value, 555);
  ASSERT_EQ(mainRef->my_string, "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->value, 555);
  ASSERT_EQ(copiedRef2->my_string, "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");

  mainRef.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 3);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->value, 555);
  ASSERT_EQ(copiedRef2->my_string, "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");

  copiedRef2.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 2);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");

  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");

  copiedRef1.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 1);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), false);
  ASSERT_EQ(copiedRef1, false);

  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");

  copiedRef3.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 0);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), false);
  ASSERT_EQ(copiedRef1, false);

  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);

  ASSERT_EQ(copiedRef3.is_active(), false);
  ASSERT_EQ(copiedRef3, false);
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_release_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto mainRef = myPool.acquire(555, "test");
  auto copiedRef1 = mainRef;    // copy
  auto copiedRef2 = copiedRef1; // copy
  auto copiedRef3 = copiedRef2; // copy

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(mainRef), 0);
  ASSERT_EQ(myPool.get_index(copiedRef1), 0);
  ASSERT_EQ(myPool.get_index(copiedRef2), 0);
  ASSERT_EQ(myPool.get_index(copiedRef3), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 4);

  ASSERT_EQ(mainRef.is_active(), true);
  ASSERT_EQ(mainRef, true);
  ASSERT_EQ(mainRef.get(), myPool.get(0).get());
  ASSERT_EQ(mainRef->value, 555);
  ASSERT_EQ(mainRef->my_string, "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->value, 555);
  ASSERT_EQ(copiedRef2->my_string, "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->value, 555);
  ASSERT_EQ(copiedRef3->my_string, "test");

  myPool.release(mainRef);

  ASSERT_EQ(myPool.size(), 0);
  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);
  ASSERT_EQ(copiedRef1.is_active(), false);
  ASSERT_EQ(copiedRef1, false);
  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);
  ASSERT_EQ(copiedRef3.is_active(), false);
  ASSERT_EQ(copiedRef3, false);
}

TEST(system_weak_ref_data_pool,
     can_acquire_several_weak_ref_and_release_them_one_by_one) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto ref1 = myPool.acquire(555, "test");
  auto ref2 = myPool.acquire(666, "test");
  auto ref3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->value, 555);
  ASSERT_EQ(ref1->my_string, "test");

  ASSERT_EQ(myPool.get_index(ref2), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref2.is_active(), true);
  ASSERT_EQ(ref2, true);
  ASSERT_EQ(ref2.get(), myPool.get(1).get());
  ASSERT_EQ(ref2->value, 666);
  ASSERT_EQ(ref2->my_string, "test");

  ASSERT_EQ(myPool.get_index(ref3), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(2).get());
  ASSERT_EQ(ref3->value, 777);
  ASSERT_EQ(ref3->my_string, "test");

  myPool.release(ref2);

  ASSERT_EQ(myPool.size(), 2);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->value, 555);
  ASSERT_EQ(ref1->my_string, "test");

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(myPool.get_index(ref3), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(1).get());
  ASSERT_EQ(ref3->value, 777);
  ASSERT_EQ(ref3->my_string, "test");

  myPool.release(ref1);

  ASSERT_EQ(myPool.size(), 1);

  ASSERT_EQ(ref1.is_active(), false);
  ASSERT_EQ(ref1, false);

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(myPool.get_index(ref3), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(0).get());
  ASSERT_EQ(ref3->value, 777);
  ASSERT_EQ(ref3->my_string, "test");

  myPool.release(ref3);

  ASSERT_EQ(myPool.size(), 0);

  ASSERT_EQ(ref1.is_active(), false);
  ASSERT_EQ(ref1, false);

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(ref3.is_active(), false);
  ASSERT_EQ(ref3, false);
}

TEST(system_weak_ref_data_pool,
     can_acquire_several_weak_ref_and_clear_the_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  auto mainRef1 = myPool.acquire(555, "test");
  auto copiedRef1 = mainRef1; // copy

  auto mainRef2 = myPool.acquire(666, "test");
  auto copiedRef2 = mainRef2; // copy

  auto mainRef3 = myPool.acquire(777, "test");
  auto copiedRef3 = mainRef3; // copy

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_index(mainRef1), 0);
  ASSERT_EQ(myPool.get_index(copiedRef1), 0);
  ASSERT_EQ(myPool.get_index(mainRef2), 1);
  ASSERT_EQ(myPool.get_index(copiedRef2), 1);
  ASSERT_EQ(myPool.get_index(mainRef3), 2);
  ASSERT_EQ(myPool.get_index(copiedRef3), 2);
  ASSERT_EQ(myPool.get_ref_count(0), 2);
  ASSERT_EQ(myPool.get_ref_count(1), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 2);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef1, true);
  ASSERT_EQ(mainRef1.get(), myPool.get(0).get());
  ASSERT_EQ(mainRef1->value, 555);
  ASSERT_EQ(mainRef1->my_string, "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->value, 555);
  ASSERT_EQ(copiedRef1->my_string, "test");

  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef2, true);
  ASSERT_EQ(mainRef2.get(), myPool.get(1).get());
  ASSERT_EQ(mainRef2->value, 666);
  ASSERT_EQ(mainRef2->my_string, "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(1).get());
  ASSERT_EQ(copiedRef2->value, 666);
  ASSERT_EQ(copiedRef2->my_string, "test");

  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef3, true);
  ASSERT_EQ(mainRef3.get(), myPool.get(2).get());
  ASSERT_EQ(mainRef3->value, 777);
  ASSERT_EQ(mainRef3->my_string, "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(2).get());
  ASSERT_EQ(copiedRef3->value, 777);
  ASSERT_EQ(copiedRef3->my_string, "test");

  myPool.clear();

  ASSERT_EQ(myPool.size(), 0);
  ASSERT_EQ(mainRef1.is_active(), false);
  ASSERT_EQ(mainRef1, false);
  ASSERT_EQ(copiedRef1.is_active(), false);
  ASSERT_EQ(copiedRef1, false);
  ASSERT_EQ(mainRef2.is_active(), false);
  ASSERT_EQ(mainRef2, false);
  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);
  ASSERT_EQ(mainRef3.is_active(), false);
  ASSERT_EQ(mainRef3, false);
  ASSERT_EQ(copiedRef3.is_active(), false);
  ASSERT_EQ(copiedRef3, false);
}

TEST(system_weak_ref_data_pool, can_directly_access_pool_data) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  myPool.acquire(555, "test");
  myPool.acquire(666, "test");
  myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(myPool.get_ref_count(1), 0);
  ASSERT_EQ(myPool.get_ref_count(2), 0);

  ASSERT_EQ(myPool.get(0)->value, 555);
  ASSERT_EQ(myPool.get(0)->my_string, "test");
  ASSERT_EQ(myPool.get(1)->value, 666);
  ASSERT_EQ(myPool.get(1)->my_string, "test");
  ASSERT_EQ(myPool.get(2)->value, 777);
  ASSERT_EQ(myPool.get(2)->my_string, "test");
}

TEST(system_weak_ref_data_pool, can_prevent_pool_growth) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  for (int ii = 0; ii < 20; ++ii) {
    auto ref = myPool.acquire(ii);
    if (ii < 10) {
      ASSERT_EQ(ref.is_active(), true);
    } else {
      ASSERT_EQ(ref.is_active(), false);
    }
  }

  ASSERT_EQ(myPool.size(), 10);
  for (int ii = 0; ii < 10; ++ii)
    ASSERT_EQ(myPool.get(uint32_t(ii)).get()->value, ii);
}

TEST(system_weak_ref_data_pool, can_allow_pool_growth) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           false>
    myPool;

  for (int ii = 0; ii < 20; ++ii) {
    auto ref = myPool.acquire(ii);
    ASSERT_EQ(ref.is_active(), true);
  }

  ASSERT_EQ(myPool.size(), 20);
  for (int ii = 0; ii < 20; ++ii)
    ASSERT_EQ(myPool.get(uint32_t(ii)).get()->value, ii);
}

TEST(system_weak_ref_data_pool, can_move_data_into_a_weak_ref) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           false>
    myPool;

  auto mainRef = myPool.acquire(666, "test");

  ASSERT_EQ(mainRef->value, 666);
  ASSERT_EQ(mainRef->my_string, "test");

  common::TestStructure tmpData(777, "test test");
  *mainRef = std::move(tmpData);

  ASSERT_EQ(mainRef->value, 777);
  ASSERT_EQ(mainRef->my_string, "test test");
}

TEST(system_weak_ref_data_pool, can_move_entire_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           false>
    myPool1;
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           false>
    myPool2;

  for (int ii = 0; ii < 20; ++ii)
    myPool1.acquire(ii, "test");

  ASSERT_EQ(myPool1.size(), 20);
  ASSERT_EQ(myPool2.size(), 0);
  for (int ii = 0; ii < 20; ++ii) {
    ASSERT_EQ(myPool1.get(uint32_t(ii))->value, ii);
    ASSERT_EQ(myPool1.get(uint32_t(ii))->my_string, "test");
  }

  myPool2 = std::move(myPool1);

  ASSERT_EQ(myPool1.size(), 0);
  ASSERT_EQ(myPool2.size(), 20);
  for (int ii = 0; ii < 20; ++ii) {
    ASSERT_EQ(myPool2.get(uint32_t(ii))->value, ii);
    ASSERT_EQ(myPool2.get(uint32_t(ii))->my_string, "test");
  }
}

TEST(system_weak_ref_data_pool,
     can_make_weak_ref_that_invalidate_once_out_of_scope) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10,
                           true>
    myPool;

  myPool.acquire(555, "test");
  myPool.acquire(666, "test");
  myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(myPool.get_ref_count(1), 0);
  ASSERT_EQ(myPool.get_ref_count(2), 0);

  {
    auto ref1 = myPool.get(0);
    auto ref2 = myPool.get(1);
    auto ref3 = myPool.get(2);

    ASSERT_EQ(myPool.get_ref_count(0), 1);
    ASSERT_EQ(myPool.get_ref_count(1), 1);
    ASSERT_EQ(myPool.get_ref_count(2), 1);
  }

  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(myPool.get_ref_count(1), 0);
  ASSERT_EQ(myPool.get_ref_count(2), 0);
}

// TEST(system_weak_ref_data_pool,
// can_make_weak_ref_that_invalidate_once_out_of_scope_with_get_index_feature) {

//   using LocalPool = gero::weak_ref_data_pool<common::TestStructure,
//   common::TestStructure, 10, true>; using LocalRef = LocalPool::weak_ref;

//   LocalPool myPool;

//   LocalRef mainRef1 = myPool.acquire(555, "test");
//   LocalRef mainRef2 = myPool.acquire(666, "test");
//   LocalRef mainRef3 = myPool.acquire(777, "test");

//   ASSERT_EQ(myPool.size(), 3);
//   ASSERT_EQ(myPool.get_ref_count(0), 1);
//   ASSERT_EQ(myPool.get_ref_count(1), 1);
//   ASSERT_EQ(myPool.get_ref_count(2), 1);

//   {
//     LocalRef subRef1 = myPool.get(0);
//     LocalRef subRef2 = myPool.get(1);
//     LocalRef subRef3 = myPool.get(2);

//     ASSERT_EQ(myPool.get_ref_count(0), 2);
//     ASSERT_EQ(myPool.get_ref_count(1), 2);
//     ASSERT_EQ(myPool.get_ref_count(2), 2);
//   }

//   ASSERT_EQ(myPool.get_ref_count(0), 1);
//   ASSERT_EQ(myPool.get_ref_count(1), 1);
//   ASSERT_EQ(myPool.get_ref_count(2), 1);
// }

namespace {

using LocalPool = gero::weak_ref_data_pool<common::TestStructure,
                                           common::TestStructure, 10, true>;
using LocalRef = LocalPool::weak_ref;
using LocalValue = LocalPool::value_type;

struct TmpResult {
  int val1;
  int val2;
  LocalRef ref;
};

struct ResultRaw {
  bool hasHit = false;

  TmpResult* allRawImpactsData;
  std::size_t allImpactsMaxSize;
  std::size_t allImpactsTotal;
};

template <std::size_t N> struct ResultArray {
  bool hasHit = false;

  std::array<TmpResult, N> allImpactsData;
  std::size_t allImpactsTotal;
};
} // namespace

TEST(
  weak_ref_data_pool,
  can_make_weak_ref_that_invalidate_once_out_of_scope_with_get_index_feature_2) {

  LocalPool myPool;

  LocalRef mainRef1 = myPool.acquire(555, "test");
  LocalRef mainRef2 = myPool.acquire(666, "test");
  LocalRef mainRef3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);

  {

    constexpr std::size_t k_size = 10;

    ResultArray<k_size> tmpArray;
    ResultRaw tmpRaw;
    tmpRaw.allRawImpactsData = tmpArray.allImpactsData.data();
    tmpRaw.allImpactsMaxSize = k_size;
    tmpRaw.allImpactsTotal = 0;

    tmpRaw.allRawImpactsData[tmpRaw.allImpactsTotal++].ref = myPool.get(0);
    tmpRaw.allRawImpactsData[tmpRaw.allImpactsTotal++].ref = myPool.get(1);
    LocalRef subRef3 = myPool.get(2);

    ASSERT_EQ(tmpRaw.allImpactsTotal, 2);

    ASSERT_EQ(myPool.get_ref_count(0), 2);
    ASSERT_EQ(myPool.get_ref_count(1), 2);
    ASSERT_EQ(myPool.get_ref_count(2), 2);
  }

  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);
}

TEST(
  weak_ref_data_pool,
  can_make_weak_ref_that_invalidate_once_out_of_scope_with_get_index_feature_3) {

  LocalPool myPool;

  LocalRef mainRef1 = myPool.acquire(555, "test");

  // {
  //   const LocalValue& val = *mainRef1;
  //   D_MYERR("val addr  " << reinterpret_cast<int64_t>(&val));
  //   LocalRef tmpRef = myPool.get(val);
  //   D_MYERR("tmpRef._pool  " << tmpRef._pool);
  //   D_MYERR("tmpRef._index " << tmpRef._index);
  // }

  LocalRef mainRef2 = myPool.acquire(666, "test");
  LocalRef mainRef3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);

  {

    constexpr std::size_t k_size = 10;

    ResultArray<k_size> tmpArray;
    ResultRaw tmpRaw;
    tmpRaw.allRawImpactsData = tmpArray.allImpactsData.data();
    tmpRaw.allImpactsMaxSize = k_size;
    tmpRaw.allImpactsTotal = 0;

    // LocalRef testRef = LocalRef::make_invalid();
    LocalRef testRef;

    D_MYERR("testRef._pool  " << testRef._pool);
    D_MYERR("testRef._index " << testRef._index);

    ASSERT_EQ(testRef, LocalRef::make_invalid());

#if 1

    LocalRef found = myPool.find_if([&mainRef2](const LocalValue& target) {
      return &target == mainRef2.get();
    });

    D_MYERR("found._pool  " << found._pool);
    D_MYERR("found._index " << found._index);

    testRef = found;

#else

    // testRef = myPool.get(*mainRef2.get());
    testRef = myPool.get(*mainRef2);

#endif

    D_MYERR("testRef._pool  " << testRef._pool);
    D_MYERR("testRef._index " << testRef._index);

    ASSERT_EQ(testRef, mainRef2);
    // ASSERT_EQ(testRef, myPool.get(*mainRef1));
  }

  // {
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[0].ref, LocalRef::make_invalid());
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[1].ref, LocalRef::make_invalid());
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[2].ref, LocalRef::make_invalid());

  //   tmpRaw.allRawImpactsData[tmpRaw.allImpactsTotal++].ref =
  //   myPool.get(*mainRef1);
  //   tmpRaw.allRawImpactsData[tmpRaw.allImpactsTotal++].ref =
  //   myPool.get(*mainRef2);
  //   tmpRaw.allRawImpactsData[tmpRaw.allImpactsTotal++].ref =
  //   myPool.get(*mainRef3);
  //   // LocalRef subRef3 = myPool.get(2);

  //   ASSERT_EQ(tmpRaw.allImpactsTotal, 3);
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[0].ref, mainRef1);
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[1].ref, mainRef2);
  //   ASSERT_EQ(tmpRaw.allRawImpactsData[2].ref, mainRef3);

  //   ASSERT_EQ(myPool.get_ref_count(0), 2);
  //   ASSERT_EQ(myPool.get_ref_count(1), 2);
  //   ASSERT_EQ(myPool.get_ref_count(2), 2);
  // }

  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);
}
