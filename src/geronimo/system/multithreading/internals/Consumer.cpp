
#include "Consumer.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <chrono>

namespace gero {
namespace threading {

Consumer::Consumer(IProducer& inProducer) : _producer(inProducer) {

  auto setupLock = _setupSynchroniser.makeScopedLock();

  _isRunning = false; // the consumer's thread will set it to true

  // launch consumer thread

  _thread = std::thread(&Consumer::_threadedMethod, this);

  // here we wait for the thread to be running

  // wait -> release the lock for other thread(s)
  _setupSynchroniser.waitUntilNotified(setupLock);
}

Consumer::~Consumer() { quit(); }

//
//

void Consumer::execute(const WorkCallback& inWorkCallback) {
  auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

  // this part is locked and will notify at the end of the scope

  _workCallback = inWorkCallback;
}

void Consumer::quit() {
  if (!_isRunning)
    return;

  {
    auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _isRunning = false;
  }

  if (_thread.joinable())
    _thread.join();
}

//
//

bool Consumer::isRunning() const { return _isRunning; }

bool Consumer::isAvailable() const { return !_taskSynchroniser.isNotified(); }

//
//

void Consumer::_threadedMethod() {

  auto taskLock = _taskSynchroniser.makeScopedLock();

  {
    auto setupLockNotifier = _setupSynchroniser.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _isRunning = true;
  }

  // this part is locked

  while (_isRunning) {
    // wait -> release the lock for other thread(s)
    _taskSynchroniser.waitUntilNotified(taskLock);

    // this part is locked

    if (!_isRunning)
      break; // quit scenario

    _workCallback();

    _producer._notifyWorkDone(this);
  }
}

} // namespace threading
} // namespace gero
