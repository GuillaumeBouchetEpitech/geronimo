
#pragma once

#include "IpAddress.hpp"

namespace gero {
namespace network {

namespace socket {
  enum class status {
    done = 0,
    notReady = 1,
    error = 2,
  };
}

class AbstractSocket {

protected:
  int _handle = -1;
  bool _isValid = false;
  bool _isBlocking = true;

public:
  void* userData = nullptr;
  int32_t uservalue = 0;

public:
  AbstractSocket() = default;
  AbstractSocket(const AbstractSocket& other) = delete; // TODO
  AbstractSocket(AbstractSocket&& other) = delete; // TODO
  AbstractSocket& operator=(const AbstractSocket& other) = delete; // TODO
  AbstractSocket& operator=(AbstractSocket&& other) = delete; // TODO
  virtual ~AbstractSocket() = default;

public:
  bool close();

public:
  bool setAsBlocking(bool inBlocking);
  bool isBlocking() const;

public:
  bool setAsReusingAddress() const;

public:
  bool validate() const;
  bool isValid() const;
  bool isInitialised() const;
};

} // namespace network
} // namespace gero
