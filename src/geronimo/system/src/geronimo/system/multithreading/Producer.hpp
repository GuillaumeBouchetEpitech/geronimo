
#pragma once

#include "internals/Consumer.hpp"
#include "internals/IProducer.hpp"
#include "internals/ThreadSynchronizer.hpp"

#include <functional>
#include <list>
#include <memory>
#include <thread>

namespace gero {
namespace threading {

class Producer : public IProducer {
private:
  struct Task {
  public:
    WorkCallback work = nullptr;
    std::shared_ptr<Consumer> consumer = nullptr;

  public:
    Task(const WorkCallback& inWork);
  };

private:
  std::thread _thread;
  ThreadSynchronizer _setupSynchronizer;
  ThreadSynchronizer _taskSynchronizer;
  ThreadSynchronizer _allTaskSynchronizer;

  bool _isRunning = false;
  bool _avoidBlocking = false;

  std::list<std::shared_ptr<Consumer>> _allConsumers;
  std::list<std::shared_ptr<Consumer>> _freeConsumers;
  std::list<std::shared_ptr<Consumer>> _busyConsumers;

  std::list<Task> _allPlannedTasks;
  std::list<Task> _allRunningTasks;

public:
  Producer() = default;
  virtual ~Producer();

public:
  void initialize(uint32_t inTotalCores, bool inAvoidBlocking = false);
  void push(const WorkCallback& inWorkCallback);
  void quit();
  void waitUntilAllCompleted();
  bool allCompleted() const;

private:
  virtual void _notifyWorkDone(Consumer* inConsumer) override;
  void _threadedMethod();
};

} // namespace threading

} // namespace gero