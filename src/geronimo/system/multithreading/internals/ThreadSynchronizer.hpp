
#pragma once

#include "geronimo/system/NonCopyable.hpp"

#include <condition_variable>
#include <mutex>

namespace gero {
namespace threading {

// this class handle all locking and conditional variable interactions
class ThreadSynchronizer : public NonCopyable {
private:
  std::mutex _mutex;
  std::condition_variable _condVar;
  bool _isNotified = false;

public:
  // this class act like a scoped lock but notify before unlocking
  class ScopedLockedNotifier {
    // friendship since we need access to the _mutex
    friend ThreadSynchronizer;

  private:
    ThreadSynchronizer& _synchronizer;

  private:
    ScopedLockedNotifier(ThreadSynchronizer& synchronizer);

  public:
    ~ScopedLockedNotifier();
  };

public:
  ThreadSynchronizer() = default;

public:
  bool waitUntilNotified(std::unique_lock<std::mutex>& lock, float seconds = 0.0f);
  void notify();

public:
  std::unique_lock<std::mutex> makeScopedLock();
  ScopedLockedNotifier makeScopedLockNotifier();
  bool isNotified() const;
};

} // namespace threading
} // namespace gero
