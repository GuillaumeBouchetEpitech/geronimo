
#include "Producer.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <algorithm>
#include <chrono>

namespace gero {
namespace threading {

Producer::Task::Task(const WorkCallback& inWork) : work(inWork) {}

Producer::~Producer() { quit(); }

//
//

void Producer::initialise(uint32_t inTotalCores, bool inAvoidBlocking /*= false*/) {

  _avoidBlocking = inAvoidBlocking;

  //
  // launch consumers

  for (uint32_t ii = 0; ii < inTotalCores; ++ii) {
    auto newConsumer = std::make_shared<Consumer>(*this, _avoidBlocking);

    _allConsumers.push_back(newConsumer);
    _freeConsumers.push_back(newConsumer);
  }

  //
  // launch producer thread

  if (_avoidBlocking == false)
  {
    auto setupLock = _setupSynchroniser.makeScopedLock();

    _isRunning = false; // the producer's thread will set it to true

    _thread = std::thread(&Producer::_threadedMethod, this);

    // here we wait for the thread to be running

    // wait -> release the lock for other thread(s)
    _setupSynchroniser.waitUntilNotified(setupLock);
  }
  else
  {
    _isRunning = false; // the producer's thread will set it to true

    _thread = std::thread(&Producer::_threadedMethod, this);

    // here we wait for the thread to be running

    // avoid blocking on the main thread
    while (_isRunning == false)
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Producer::push(const WorkCallback& inWorkCallback) {
  if (!_isRunning)
    D_THROW(std::runtime_error, "producer not running");

  auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

  // this part is locked and will notify at the end of the scope

  _allPlannedTasks.push_back(Task(inWorkCallback));
}

void Producer::quit() {
  if (!_isRunning)
    return;

  // clear the planned task(s) and wake up the running thread
  {
    auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _allPlannedTasks.clear();
  }

  waitUntilAllCompleted();

  // clear the planned task(s) and wake up the running thread
  {
    auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    // _allPlannedTasks.clear();
    _isRunning = false;
  }

  if (_thread.joinable())
    _thread.join();

  _freeConsumers.clear();
  _busyConsumers.clear();
  _allConsumers.clear();

  // no more shared_ptr references are held
  //
  // the consumers destructor:
  // => should have been called
  // => will terminate their thread
}

void Producer::waitUntilAllCompleted() {
  if (!_isRunning)
    D_THROW(std::runtime_error, "producer not running");

  auto lock = _allTaskSynchroniser.makeScopedLock();

  // this part is locked

  // make the (main) thread wait for all tasks to be completed
  while (!allCompleted()) {
    // wait -> release the lock for other thread(s)
    _allTaskSynchroniser.waitUntilNotified(lock);
  }
}

bool Producer::allCompleted() const {
  return (_allPlannedTasks.empty() && _allRunningTasks.empty());
}

//
//

void Producer::_notifyWorkDone(Consumer* inConsumer) {
  auto lockNotifier = _taskSynchroniser.makeScopedLockNotifier();

  // this part is locked and will notify at the end of the scope

  // find the task per consumer in the "running" list
  auto comparison = [inConsumer](const Task& currTask) {
    return currTask.consumer.get() == inConsumer;
  };
  auto itTask = std::find_if(_allRunningTasks.begin(), _allRunningTasks.end(), comparison);

  if (itTask == _allRunningTasks.end())
    return; // <= this should never fail

  // move consumer from free to busy
  auto currConsumer = itTask->consumer;
  _freeConsumers.push_back(currConsumer);
  auto itConsumer =
    std::find(_busyConsumers.begin(), _busyConsumers.end(), currConsumer);
  if (itConsumer != _busyConsumers.end()) // <= this should never fail
    _busyConsumers.erase(itConsumer);

  _allRunningTasks.erase(itTask);

  // wake up potentially waiting (main) thread
  if (_allPlannedTasks.empty() && _allRunningTasks.empty())
    _allTaskSynchroniser.notify();
}

void Producer::_threadedMethod() {

  auto taskLock = _taskSynchroniser.makeScopedLock();

  // this part is locked


  if (_avoidBlocking == false)
  {
    auto setupLockNotifier = _setupSynchroniser.makeScopedLockNotifier();

    // this part is locked and will notify at the end of the scope

    _isRunning = true;
  }
  else
  {
    _isRunning = true;
  }

  while (_isRunning) {
    // wait -> release the lock for other thread(s)
    _taskSynchroniser.waitUntilNotified(taskLock);

    // this part is locked

    if (!_isRunning)
      break; // quit scenario

    while (!_allPlannedTasks.empty()) {
      if (_freeConsumers.empty())
        break; // no consumer available, wait again

      // move consumer from free to busy
      auto currConsumer = _freeConsumers.front();
      _busyConsumers.push_back(currConsumer);
      _freeConsumers.pop_front();

      Task& currentTask = _allPlannedTasks.front();

      // run task
      currentTask.consumer = currConsumer;
      currConsumer->execute(currentTask.work);

      // move task from planned to running
      _allRunningTasks.push_back(currentTask);
      _allPlannedTasks.pop_front();
    }
  }
}

} // namespace threading
} // namespace gero
