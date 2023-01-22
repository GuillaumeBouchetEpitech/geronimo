
#pragma once

#include "geronimo/system/NonCopyable.hpp"

#include <functional>
#include <memory>

namespace gero {
namespace threading {

using WorkCallback = std::function<void()>;

class Consumer;

class IProducer : public NonCopyable {
  // friendship so the consumers can call _notifyWorkDone()
  friend Consumer;

public:
  virtual ~IProducer() = default;

protected:
  virtual void _notifyWorkDone(Consumer* consumer) = 0;
};

} // namespace threading
} // namespace gero
