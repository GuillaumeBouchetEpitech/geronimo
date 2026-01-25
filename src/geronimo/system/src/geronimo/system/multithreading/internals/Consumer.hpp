
#pragma once

#include "IProducer.hpp"
#include "ThreadSynchronizer.hpp"

#include "geronimo/system/NonCopyable.hpp"

#include <functional>

#include <thread>

namespace gero {
namespace threading {

class Consumer : public NonCopyable {
private:
  IProducer& _producer;

  std::thread _thread;
  ThreadSynchronizer _setupSynchronizer;
  ThreadSynchronizer _taskSynchronizer;

  bool _isRunning = false;
  bool _avoidBlocking = false;

  WorkCallback _workCallback;

public:
  explicit Consumer(IProducer& inProducer, bool inAvoidBlocking);
  ~Consumer();

public:
  void execute(const WorkCallback& inWorkCallback);
  void quit();

public:
  bool isRunning() const;
  bool isAvailable() const;

private:
  void _threadedMethod();
};

} // namespace threading
} // namespace gero
