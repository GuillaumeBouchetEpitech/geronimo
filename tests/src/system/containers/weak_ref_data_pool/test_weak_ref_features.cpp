
#include "headers.hpp"

#include <list>

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_get_the_values_from_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  // auto ref = myPool.acquire(555, "test");
  gero::data_pool_weak_ref<common::ITestStructure> ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->get_value(), 555);
  ASSERT_EQ(ref->get_my_string(), "test");
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_copy_and_move_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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

  ASSERT_EQ(mainRef->get_value(), 555);
  ASSERT_EQ(mainRef->get_my_string(), "test");
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");
  ASSERT_EQ(copiedRef2->get_value(), 555);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_get_and_set_the_values_of_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  auto ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->get_value(), 555);
  ASSERT_EQ(ref->get_my_string(), "test");

  ref->set_value(ref->get_value() + 100);
  ref->set_my_string(ref->get_my_string() + " test");

  ASSERT_EQ(ref->get_value(), 655);
  ASSERT_EQ(ref->get_my_string(), "test test");
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_get_total_ref_count_from_the_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  auto ref = myPool.acquire(555, "test");

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_index(ref), 0);
  ASSERT_EQ(myPool.get_ref_count(ref), 1);
  ASSERT_EQ(ref.is_active(), true);
  ASSERT_EQ(ref, true);
  ASSERT_EQ(ref.get(), myPool.get(0).get());
  ASSERT_EQ(ref->get_value(), 555);
  ASSERT_EQ(ref->get_my_string(), "test");

  ref.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(ref.is_active(), false);
  ASSERT_EQ(ref, false);
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_invalidate_it) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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
  ASSERT_EQ(mainRef->get_value(), 555);
  ASSERT_EQ(mainRef->get_my_string(), "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->get_value(), 555);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

  mainRef.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 3);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->get_value(), 555);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

  copiedRef2.invalidate();

  ASSERT_EQ(myPool.size(), 1);
  ASSERT_EQ(myPool.get_ref_count(0), 2);

  ASSERT_EQ(mainRef.is_active(), false);
  ASSERT_EQ(mainRef, false);

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), false);
  ASSERT_EQ(copiedRef2, false);

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

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
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

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
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  D_MYLOG("step");

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

  D_MYLOG("step");

  ASSERT_EQ(mainRef.is_active(), true);
  ASSERT_EQ(mainRef, true);
  ASSERT_EQ(mainRef.get(), myPool.get(0).get());
  ASSERT_EQ(mainRef->get_value(), 555);
  ASSERT_EQ(mainRef->get_my_string(), "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->get_value(), 555);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

  D_MYLOG("step");

  myPool.release(mainRef);

  D_MYLOG("step");

  ASSERT_EQ(myPool.size(), 0);

  D_MYLOG("step");

  ASSERT_EQ(mainRef.is_active(), false);

  D_MYLOG("step");

  ASSERT_EQ(mainRef, false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef1.is_active(), false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef1, false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef2.is_active(), false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef2, false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef3.is_active(), false);

  D_MYLOG("step");

  ASSERT_EQ(copiedRef3, false);

  D_MYLOG("step");
}

TEST(system_weak_ref_data_pool, can_acquire_weak_ref_and_release_by_index) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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
  ASSERT_EQ(mainRef->get_value(), 555);
  ASSERT_EQ(mainRef->get_my_string(), "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef2->get_value(), 555);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef3->get_value(), 555);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

  myPool.release(0);

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

TEST(system_weak_ref_data_pool, can_acquire_several_weak_ref_and_release_them_one_by_one) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  auto ref1 = myPool.acquire(555, "test");
  auto ref2 = myPool.acquire(666, "test");
  auto ref3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->get_value(), 555);
  ASSERT_EQ(ref1->get_my_string(), "test");

  ASSERT_EQ(myPool.get_index(ref2), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref2.is_active(), true);
  ASSERT_EQ(ref2, true);
  ASSERT_EQ(ref2.get(), myPool.get(1).get());
  ASSERT_EQ(ref2->get_value(), 666);
  ASSERT_EQ(ref2->get_my_string(), "test");

  ASSERT_EQ(myPool.get_index(ref3), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(2).get());
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

  myPool.release(ref2);

  ASSERT_EQ(myPool.size(), 2);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->get_value(), 555);
  ASSERT_EQ(ref1->get_my_string(), "test");

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(myPool.get_index(ref3), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(1).get());
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

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
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

  myPool.release(ref3);

  ASSERT_EQ(myPool.size(), 0);

  ASSERT_EQ(ref1.is_active(), false);
  ASSERT_EQ(ref1, false);

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(ref3.is_active(), false);
  ASSERT_EQ(ref3, false);
}

TEST(system_weak_ref_data_pool, can_acquire_several_weak_ref_and_release_them_one_by_one_by_index) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  auto ref1 = myPool.acquire(555, "test");
  auto ref2 = myPool.acquire(666, "test");
  auto ref3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->get_value(), 555);
  ASSERT_EQ(ref1->get_my_string(), "test");

  ASSERT_EQ(myPool.get_index(ref2), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref2.is_active(), true);
  ASSERT_EQ(ref2, true);
  ASSERT_EQ(ref2.get(), myPool.get(1).get());
  ASSERT_EQ(ref2->get_value(), 666);
  ASSERT_EQ(ref2->get_my_string(), "test");

  ASSERT_EQ(myPool.get_index(ref3), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(2).get());
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

  myPool.release(1);

  ASSERT_EQ(myPool.size(), 2);

  ASSERT_EQ(myPool.get_index(ref1), 0);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(ref1.is_active(), true);
  ASSERT_EQ(ref1, true);
  ASSERT_EQ(ref1.get(), myPool.get(0).get());
  ASSERT_EQ(ref1->get_value(), 555);
  ASSERT_EQ(ref1->get_my_string(), "test");

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(myPool.get_index(ref3), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(ref3.is_active(), true);
  ASSERT_EQ(ref3, true);
  ASSERT_EQ(ref3.get(), myPool.get(1).get());
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

  myPool.release(0);

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
  ASSERT_EQ(ref3->get_value(), 777);
  ASSERT_EQ(ref3->get_my_string(), "test");

  myPool.release(0);

  ASSERT_EQ(myPool.size(), 0);

  ASSERT_EQ(ref1.is_active(), false);
  ASSERT_EQ(ref1, false);

  ASSERT_EQ(ref2.is_active(), false);
  ASSERT_EQ(ref2, false);

  ASSERT_EQ(ref3.is_active(), false);
  ASSERT_EQ(ref3, false);
}

namespace {

void rec_expand(const std::vector<int>& inData, std::vector<std::vector<int>>& inoutResults) {
  if (inData.empty())
    return;

  if (inData.size() == 1) {
    std::vector<int> tmpRes;
    tmpRes.push_back(inData[0]);
    inoutResults.push_back(tmpRes);
    return;
  }

  for (std::size_t ii = 0; ii < inData.size(); ++ii) {
    std::vector<int> copiedData = inData;

    copiedData.erase(copiedData.begin() + int(ii));

    std::vector<std::vector<int>> subRes;
    rec_expand(copiedData, subRes);

    for (const auto& currRes : subRes) {
      std::vector<int> tmpRes;

      tmpRes.push_back(inData[ii]);

      for (int currVal : currRes)
        tmpRes.push_back(currVal);

      inoutResults.push_back(tmpRes);
    }
  }
}

} // namespace

TEST(system_weak_ref_data_pool, can_acquire_several_weak_ref_and_release_them_one_by_one_by_index_LOL) {

  constexpr int k_totalIndices = 6;

  std::vector<int> indices;
  indices.reserve(k_totalIndices);
  for (int ii = 0; ii < k_totalIndices; ++ii)
    indices.push_back(ii);

  std::vector<std::vector<int>> allResults;
  rec_expand(indices, allResults);

  for (std::vector<int>& currResult : allResults) {
    using test = gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true>;
    test myPool;

    std::list<test::weak_ref> allRefsList;
    for (std::size_t ii = 0; ii < indices.size(); ++ii)
      allRefsList.push_back(myPool.acquire((ii + 1) * 100, "test"));

    std::vector<test::weak_ref> allRefsArray;
    std::vector<std::size_t> allIndicesArray;
    allRefsArray.reserve(allRefsList.size());
    allIndicesArray.reserve(allRefsList.size());
    for (test::weak_ref tmpRef : allRefsList)
      allRefsArray.push_back(tmpRef);
    for (std::size_t ii = 0; ii < allRefsList.size(); ++ii)
      allIndicesArray.push_back(ii);

    ASSERT_EQ(myPool.size(), indices.size());

    // D_MYLOG(" === ");
    // for (int currIndex : currResult)
    // {
    //   D_MYLOG(" ->  " << currIndex);
    // }
    // D_MYLOG(" === ");

    for (int currIndex : currResult) {

      // D_MYLOG("currIndex " << currIndex);

      for (std::size_t ii = 0; ii < allRefsArray.size(); ++ii) {
        auto& tmpRef = allRefsArray[ii];
        const std::size_t tmpIndex = allIndicesArray[ii];

        ASSERT_EQ(myPool.get_index(tmpRef), ii);
        ASSERT_EQ(myPool.get_ref_count(uint32_t(ii)), 2);
        ASSERT_EQ(tmpRef.is_active(), true);
        ASSERT_EQ(tmpRef, true);
        ASSERT_EQ(tmpRef.get(), myPool.get(uint32_t(ii)).get());
        ASSERT_EQ(tmpRef->get_value(), (tmpIndex + 1) * 100);
        ASSERT_EQ(tmpRef->get_my_string(), "test");
      }

      ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2);

      test::weak_ref ref1 = myPool.get(uint32_t(currIndex));
      test::weak_ref ref2 = myPool.get(uint32_t(currIndex));
      test::weak_ref ref3 = myPool.get(uint32_t(currIndex));
      test::weak_ref ref4 = myPool.get(uint32_t(currIndex));
      test::weak_ref ref5 = myPool.get(uint32_t(currIndex));

      ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2 + 5);

      ASSERT_EQ(ref1.is_active(), true);
      ASSERT_EQ(ref2.is_active(), true);
      ASSERT_EQ(ref3.is_active(), true);
      ASSERT_EQ(ref4.is_active(), true);
      ASSERT_EQ(ref5.is_active(), true);

      ref3.invalidate();
      ref4.invalidate();

      ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2 + 3);

      ASSERT_EQ(ref1.is_active(), true);
      ASSERT_EQ(ref2.is_active(), true);
      ASSERT_EQ(ref3.is_active(), false);
      ASSERT_EQ(ref4.is_active(), false);
      ASSERT_EQ(ref5.is_active(), true);

      {

        test::weak_ref ref6 = myPool.get(uint32_t(currIndex));
        test::weak_ref ref7 = myPool.get(uint32_t(currIndex));
        test::weak_ref ref8 = myPool.get(uint32_t(currIndex));

        ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2 + 3 + 3);

        ASSERT_EQ(ref6.is_active(), true);
        ASSERT_EQ(ref7.is_active(), true);
        ASSERT_EQ(ref8.is_active(), true);
      }

      ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2 + 3);

      myPool.release(currIndex);

      ASSERT_EQ(ref1.is_active(), false);
      ASSERT_EQ(ref2.is_active(), false);
      ASSERT_EQ(ref3.is_active(), false);
      ASSERT_EQ(ref4.is_active(), false);
      ASSERT_EQ(ref5.is_active(), false);

      {
        auto& tmpRef = allRefsArray[std::size_t(currIndex)];

        // ASSERT_EQ(myPool.get_index(tmpRef), currIndex);
        // ASSERT_EQ(myPool.get_ref_count(uint32_t(currIndex)), 2);
        ASSERT_EQ(tmpRef.is_active(), false);
        ASSERT_EQ(tmpRef, false);
        // ASSERT_EQ(tmpRef.get(), myPool.get(uint32_t(currIndex)).get());
        // ASSERT_EQ(tmpRef->get_value(), (currIndex + 1) * 100);
        // ASSERT_EQ(tmpRef->get_my_string(), "test");
      }

      // allRefsArray.erase(allRefsArray.begin() + currIndex);
      std::swap(allRefsArray.at(std::size_t(currIndex)), allRefsArray.back());
      allRefsArray.pop_back();
      std::swap(allIndicesArray.at(std::size_t(currIndex)), allIndicesArray.back());
      allIndicesArray.pop_back();

      for (int& currVal : currResult)
        if (currVal > currIndex)
          currVal -= 1;

      ASSERT_EQ(myPool.size(), allRefsArray.size());

      for (std::size_t ii = 0; ii < allRefsArray.size(); ++ii) {
        auto& tmpRef = allRefsArray[ii];
        const std::size_t tmpIndex = allIndicesArray[ii];

        ASSERT_EQ(myPool.get_index(tmpRef), ii);
        ASSERT_EQ(myPool.get_ref_count(uint32_t(ii)), 2);
        ASSERT_EQ(tmpRef.is_active(), true);
        ASSERT_EQ(tmpRef, true);
        ASSERT_EQ(tmpRef.get(), myPool.get(uint32_t(ii)).get());
        ASSERT_EQ(tmpRef->get_value(), (tmpIndex + 1) * 100);
        ASSERT_EQ(tmpRef->get_my_string(), "test");
      }
    }

    ASSERT_EQ(myPool.size(), 0);
  }
}

TEST(system_weak_ref_data_pool, can_acquire_several_weak_ref_and_clear_the_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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
  ASSERT_EQ(mainRef1->get_value(), 555);
  ASSERT_EQ(mainRef1->get_my_string(), "test");

  ASSERT_EQ(copiedRef1.is_active(), true);
  ASSERT_EQ(copiedRef1, true);
  ASSERT_EQ(copiedRef1.get(), myPool.get(0).get());
  ASSERT_EQ(copiedRef1->get_value(), 555);
  ASSERT_EQ(copiedRef1->get_my_string(), "test");

  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef2, true);
  ASSERT_EQ(mainRef2.get(), myPool.get(1).get());
  ASSERT_EQ(mainRef2->get_value(), 666);
  ASSERT_EQ(mainRef2->get_my_string(), "test");

  ASSERT_EQ(copiedRef2.is_active(), true);
  ASSERT_EQ(copiedRef2, true);
  ASSERT_EQ(copiedRef2.get(), myPool.get(1).get());
  ASSERT_EQ(copiedRef2->get_value(), 666);
  ASSERT_EQ(copiedRef2->get_my_string(), "test");

  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef3, true);
  ASSERT_EQ(mainRef3.get(), myPool.get(2).get());
  ASSERT_EQ(mainRef3->get_value(), 777);
  ASSERT_EQ(mainRef3->get_my_string(), "test");

  ASSERT_EQ(copiedRef3.is_active(), true);
  ASSERT_EQ(copiedRef3, true);
  ASSERT_EQ(copiedRef3.get(), myPool.get(2).get());
  ASSERT_EQ(copiedRef3->get_value(), 777);
  ASSERT_EQ(copiedRef3->get_my_string(), "test");

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
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

  myPool.acquire(555, "test");
  myPool.acquire(666, "test");
  myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 0);
  ASSERT_EQ(myPool.get_ref_count(1), 0);
  ASSERT_EQ(myPool.get_ref_count(2), 0);

  ASSERT_EQ(myPool.get(0)->get_value(), 555);
  ASSERT_EQ(myPool.get(0)->get_my_string(), "test");
  ASSERT_EQ(myPool.get(1)->get_value(), 666);
  ASSERT_EQ(myPool.get(1)->get_my_string(), "test");
  ASSERT_EQ(myPool.get(2)->get_value(), 777);
  ASSERT_EQ(myPool.get(2)->get_my_string(), "test");
}

TEST(system_weak_ref_data_pool, can_prevent_pool_growth) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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
    ASSERT_EQ(myPool.get(uint32_t(ii)).get()->get_value(), ii);
}

TEST(system_weak_ref_data_pool, can_allow_pool_growth) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, false> myPool;

  for (int ii = 0; ii < 20; ++ii) {
    auto ref = myPool.acquire(ii);
    ASSERT_EQ(ref.is_active(), true);
  }

  ASSERT_EQ(myPool.size(), 20);
  for (int ii = 0; ii < 20; ++ii)
    ASSERT_EQ(myPool.get(uint32_t(ii)).get()->get_value(), ii);
}

TEST(system_weak_ref_data_pool, can_move_data_into_a_weak_ref) {
  gero::weak_ref_data_pool<common::TestStructure, common::TestStructure, 10, false> myPool;

  auto mainRef = myPool.acquire(666, "test");

  ASSERT_EQ(mainRef->get_value(), 666);
  ASSERT_EQ(mainRef->get_my_string(), "test");

  common::TestStructure tmpData(777, "test test");
  *mainRef = std::move(tmpData);

  ASSERT_EQ(mainRef->get_value(), 777);
  ASSERT_EQ(mainRef->get_my_string(), "test test");
}

TEST(system_weak_ref_data_pool, can_move_entire_pool) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, false> myPool1;
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, false> myPool2;

  for (int ii = 0; ii < 20; ++ii)
    myPool1.acquire(ii, "test");

  ASSERT_EQ(myPool1.size(), 20);
  ASSERT_EQ(myPool2.size(), 0);
  for (int ii = 0; ii < 20; ++ii) {
    ASSERT_EQ(myPool1.get(uint32_t(ii))->get_value(), ii);
    ASSERT_EQ(myPool1.get(uint32_t(ii))->get_my_string(), "test");
  }

  myPool2 = std::move(myPool1);

  ASSERT_EQ(myPool1.size(), 0);
  ASSERT_EQ(myPool2.size(), 20);
  for (int ii = 0; ii < 20; ++ii) {
    ASSERT_EQ(myPool2.get(uint32_t(ii))->get_value(), ii);
    ASSERT_EQ(myPool2.get(uint32_t(ii))->get_my_string(), "test");
  }
}

TEST(system_weak_ref_data_pool, can_make_weak_ref_that_invalidate_once_out_of_scope) {
  gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true> myPool;

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

using LocalPool = gero::weak_ref_data_pool<common::TestStructure, common::ITestStructure, 10, true>;
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

TEST(system_weak_ref_data_pool, can_make_weak_ref_that_invalidate_once_out_of_scope_with_get_index_feature_2) {

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

TEST(system_weak_ref_data_pool, can_make_weak_ref_that_invalidate_once_out_of_scope_with_get_index_feature_3) {

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

    LocalRef found = myPool.find_if([&mainRef2](const LocalValue& target) { return &target == mainRef2.get(); });

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




template<typename T>
struct LOL {
  int value;
  T data;
};



TEST(system_weak_ref_data_pool, can_have_weak_ref_in_templated_std_vector_copied_and_still_up_to_date) {

  LocalPool myPool;

  LocalRef mainRef1 = myPool.acquire(555, "test");
  LocalRef mainRef2 = myPool.acquire(666, "test");
  LocalRef mainRef3 = myPool.acquire(777, "test");

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 1);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef1.get()->get_value(), 555);
  ASSERT_EQ(mainRef2.get()->get_value(), 666);
  ASSERT_EQ(mainRef3.get()->get_value(), 777);

  //
  //

  std::vector<LOL<LocalRef>> myVector;

  myVector.push_back({ 1, mainRef1 });
  myVector.push_back({ 2, mainRef2 });
  myVector.push_back({ 3, mainRef3 });

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 2);
  ASSERT_EQ(myPool.get_ref_count(1), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 2);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef1.get()->get_value(), 555);
  ASSERT_EQ(mainRef2.get()->get_value(), 666);
  ASSERT_EQ(mainRef3.get()->get_value(), 777);
  ASSERT_EQ(myVector.at(0).data.is_active(), true);
  ASSERT_EQ(myVector.at(1).data.is_active(), true);
  ASSERT_EQ(myVector.at(2).data.is_active(), true);
  ASSERT_EQ(myVector.at(0).data.get()->get_value(), 555);
  ASSERT_EQ(myVector.at(1).data.get()->get_value(), 666);
  ASSERT_EQ(myVector.at(2).data.get()->get_value(), 777);

  //
  //

  auto lol1 = myVector.at(0);

  // lol1.data.invalidate();

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 3);
  ASSERT_EQ(myPool.get_ref_count(1), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 2);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef1.get()->get_value(), 555);
  ASSERT_EQ(mainRef2.get()->get_value(), 666);
  ASSERT_EQ(mainRef3.get()->get_value(), 777);
  ASSERT_EQ(myVector.at(0).data.is_active(), true);
  ASSERT_EQ(myVector.at(1).data.is_active(), true);
  ASSERT_EQ(myVector.at(2).data.is_active(), true);
  ASSERT_EQ(myVector.at(0).data.get()->get_value(), 555);
  ASSERT_EQ(myVector.at(1).data.get()->get_value(), 666);
  ASSERT_EQ(myVector.at(2).data.get()->get_value(), 777);
  ASSERT_EQ(lol1.data.is_active(), true);
  ASSERT_EQ(lol1.data.get()->get_value(), 555);

  //
  //

  myVector.push_back({ 4, lol1.data });

  ASSERT_EQ(myPool.size(), 3);
  ASSERT_EQ(myPool.get_ref_count(0), 4);
  ASSERT_EQ(myPool.get_ref_count(1), 2);
  ASSERT_EQ(myPool.get_ref_count(2), 2);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef1.get()->get_value(), 555);
  ASSERT_EQ(mainRef2.get()->get_value(), 666);
  ASSERT_EQ(mainRef3.get()->get_value(), 777);
  ASSERT_EQ(myVector.at(0).data.is_active(), true);
  ASSERT_EQ(myVector.at(1).data.is_active(), true);
  ASSERT_EQ(myVector.at(2).data.is_active(), true);
  ASSERT_EQ(myVector.at(3).data.is_active(), true);
  ASSERT_EQ(myVector.at(0).data.get()->get_value(), 555);
  ASSERT_EQ(myVector.at(1).data.get()->get_value(), 666);
  ASSERT_EQ(myVector.at(2).data.get()->get_value(), 777);
  ASSERT_EQ(myVector.at(3).data.get()->get_value(), 555);
  ASSERT_EQ(lol1.data.is_active(), true);
  ASSERT_EQ(lol1.data.get()->get_value(), 555);

  //
  //

  myVector.clear();

  ASSERT_EQ(myPool.get_ref_count(0), 2);
  ASSERT_EQ(myPool.get_ref_count(1), 1);
  ASSERT_EQ(myPool.get_ref_count(2), 1);

  ASSERT_EQ(mainRef1.is_active(), true);
  ASSERT_EQ(mainRef2.is_active(), true);
  ASSERT_EQ(mainRef3.is_active(), true);
  ASSERT_EQ(mainRef1.get()->get_value(), 555);
  ASSERT_EQ(mainRef2.get()->get_value(), 666);
  ASSERT_EQ(mainRef3.get()->get_value(), 777);
}
