
#include "headers.hpp"

#include <mutex>
#include <thread>

TEST(system_multithreading,
     avoid_blocking___can_run_task_in_parallel_and_wait_for_them_to_finish) {

  constexpr int k_totalThreads = 3;

  gero::threading::Producer threadedProducer;
  threadedProducer.initialise(k_totalThreads, true);

  const auto startTime = std::chrono::high_resolution_clock::now();

  for (int ii = 0; ii < k_totalThreads; ++ii) {
    threadedProducer.push(
      []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
  }

  threadedProducer.waitUntilAllCompleted();

  const auto endTime = std::chrono::high_resolution_clock::now();
  const auto microseconds =
    std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  const int64_t elapsedTime = microseconds.count();

  ASSERT_GT(elapsedTime, 90);
  // not using ASSERT_LT -> valgrind slow down the test and make it fail
  EXPECT_LT(elapsedTime, 110);
}

TEST(
  system_multithreading,
  avoid_blocking___can_run_more_task_in_parallel_than_the_total_thread_number_and_wait_for_them_to_finish) {

  constexpr int k_totalThreads = 3;

  gero::threading::Producer threadedProducer;
  threadedProducer.initialise(k_totalThreads, true);

  const auto startTime = std::chrono::high_resolution_clock::now();

  for (int ii = 0; ii < k_totalThreads * 2; ++ii) {
    threadedProducer.push(
      []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
  }

  threadedProducer.waitUntilAllCompleted();

  const auto endTime = std::chrono::high_resolution_clock::now();
  const auto microseconds =
    std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  const int64_t elapsedTime = microseconds.count();

  ASSERT_GT(elapsedTime, 190);
  ASSERT_LT(elapsedTime, 210);
}

TEST(system_multithreading, avoid_blocking___can_run_task_in_parallel_that_get_and_set_values) {

  constexpr int k_totalThreads = 3;
  std::mutex tmpMutex;

  gero::threading::Producer threadedProducer;
  threadedProducer.initialise(k_totalThreads, true);

  int value = 0;

  for (int ii = 0; ii < 100; ++ii) {
    threadedProducer.push([&tmpMutex, &value]() {
      std::lock_guard<std::mutex> scopedLoc(tmpMutex);

      value += 1;
    });
  }

  threadedProducer.waitUntilAllCompleted();

  ASSERT_EQ(value, 100);
}

TEST(system_multithreading,
     avoid_blocking___can_run_task_in_parallel_that_get_and_set_values_in_a_thead_safe_way) {

  constexpr int k_totalThreads = 3;
  std::mutex tmpMutex;

  gero::threading::Producer threadedProducer;
  threadedProducer.initialise(k_totalThreads, true);

  int totalChanges = 0;
  bool value = true;

  for (int ii = 0; ii < 10; ++ii) {
    threadedProducer.push([&tmpMutex, &totalChanges, &value]() {
      std::lock_guard<std::mutex> scopedLoc(tmpMutex);

      if (value == false)
        value = true;
      else
        value = false;

      totalChanges += 1;
    });
  }

  threadedProducer.waitUntilAllCompleted();

  ASSERT_EQ(value, true);
  ASSERT_EQ(totalChanges, 10);
}
