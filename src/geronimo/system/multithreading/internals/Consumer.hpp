
#pragma once

#include "IProducer.hpp"
#include "ThreadSynchroniser.hpp"

#include "geronimo/system/NonCopyable.hpp"

#include <functional>

#include <thread>

namespace gero {
namespace threading {

class Consumer : public NonCopyable {
private:
  std::thread _thread;
  ThreadSynchroniser _setupSynchroniser;
  ThreadSynchroniser _taskSynchroniser;

  bool _isRunning = false;

  WorkCallback _workCallback;

  IProducer& _producer;

public:
  explicit Consumer(IProducer& inProducer);
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
