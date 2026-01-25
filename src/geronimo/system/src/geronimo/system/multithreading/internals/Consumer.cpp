
#include "Consumer.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <chrono>

namespace gero {
namespace threading {

Consumer::Consumer(IProducer& inProducer, bool inAvoidBlocking)
  : _producer(inProducer), _avoidBlocking(inAvoidBlocking) {

  if (_avoidBlocking == false) {
    auto setupLock = _setupSynchronizer.makeScopedLock();

    _isRunning = false; // the consumer's thread will set it to true

    // launch consumer thread

    _thread = std::thread(&Consumer::_threadedMethod, this);

    // here we wait for the thread to be running

    // wait -> release the lock for other thread(s)
    _setupSynchronizer.waitUntilNotified(setupLock);
  } else {
    _isRunning = false; // the consumer's thread will set it to true

    // launch consumer thread

    _thread = std::thread(&Consumer::_threadedMethod, this);

    // here we wait for the thread to be running

    // avoid blocking on the main thread
    while (_isRunning == false)
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

Consumer::~Consumer() { quit(); }

//
//

void Consumer::execute(const WorkCallback& inWorkCallback) {
  auto lockNotifier = _taskSynchronizer.makeScopedLockNotifier();

  // this part is locked and will notify at the end of the scope

  _workCallback = inWorkCallback;
}

void Consumer::quit() {
  if (!_isRunning)
    return;

  {
    auto lockNotifier = _taskSynchronizer.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _isRunning = false;
  }

  if (_thread.joinable())
    _thread.join();
}

//
//

bool Consumer::isRunning() const { return _isRunning; }

bool Consumer::isAvailable() const { return !_taskSynchronizer.isNotified(); }

//
//

void Consumer::_threadedMethod() {

  auto taskLock = _taskSynchronizer.makeScopedLock();

  if (_avoidBlocking == false) {
    auto setupLockNotifier = _setupSynchronizer.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _isRunning = true;
  } else {
    _isRunning = true;
  }

  // this part is locked

  while (_isRunning) {
    // wait -> release the lock for other thread(s)
    _taskSynchronizer.waitUntilNotified(taskLock);

    // this part is locked

    if (!_isRunning)
      break; // quit scenario

    _workCallback();

    _producer._notifyWorkDone(this);
  }
}

} // namespace threading
} // namespace gero
