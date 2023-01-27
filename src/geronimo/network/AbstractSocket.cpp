
#include "AbstractSocket.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"

#include <arpa/inet.h> // ::shutdown()
#include <fcntl.h> // fcntl, F_GETFL, F_SETFL, ~O_NONBLOCK
#include <unistd.h> // ::close()

// #define D_DEBUG_ABSTRACT_SOCKET

#if defined D_DEBUG_ABSTRACT_SOCKET
  #define D_MYDEBUG_ABSTRACT_SOCKET D_MYERR
#else
  #define D_MYDEBUG_ABSTRACT_SOCKET(stream)
#endif

namespace gero {
namespace network {

bool AbstractSocket::close() {
  if (!this->validate()) {
    D_MYERR("failed validation");
    return false;
  }

  D_MYDEBUG_ABSTRACT_SOCKET("socket close, _handle=" << _handle);

  {
#if defined D_DEBUG_ABSTRACT_SOCKET

    const int32_t result1 = ::shutdown(_handle, SHUT_RDWR);

    if (result1 == SHUT_RD)
    {
      D_MYDEBUG_ABSTRACT_SOCKET("socket close, shutdown(): No more receptions");
    }
    if (result1 == SHUT_WR)
    {
      D_MYDEBUG_ABSTRACT_SOCKET("socket close, shutdown(): No more transmissions");
    }
    if (result1 == SHUT_RDWR)
    {
      D_MYDEBUG_ABSTRACT_SOCKET("socket close, shutdown(): No more receptions or transmissions");
    }

#else

    ::shutdown(_handle, SHUT_RDWR);

#endif

  }

  {


    const int32_t result = ::close(_handle);
    D_MYDEBUG_ABSTRACT_SOCKET("socket close, close(), result = " << result);
    if (result == -1) {
      D_MYERR("could not close the socket, message: " << std::strerror(errno));
      return false;
    }

  }

  _isValid = false;
  return true;
}

bool AbstractSocket::setAsBlocking(bool inBlocking) {
  if (!this->validate()) {
    D_MYERR("failed validation");
    return false;
  }

  if (_isBlocking == inBlocking) {
    D_MYERR("call will do nothing");
    return false;
  }

  const int currStatus = ::fcntl(this->_handle, F_GETFL);
  const int nextStatus = inBlocking
    ? currStatus & ~O_NONBLOCK
    : currStatus | O_NONBLOCK;

  if (::fcntl(this->_handle, F_SETFL, nextStatus) == -1)
  {
    D_MYERR("failed to set socket as blocking, message: " << std::strerror(errno));
    return false;
  }

  _isBlocking = inBlocking;
  return true;
}

bool AbstractSocket::isBlocking() const { return _isBlocking; }

bool AbstractSocket::setAsReusingAddress() const {
  if (!this->validate()) {
    D_MYERR("failed validation");
    return false;
  }

  const int value = 1;
  if (::setsockopt(this->_handle, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1) {
    D_MYERR("failed to set the reuse addr value, message: " << std::strerror(errno));
    return false;
  }
  return true;
}

bool AbstractSocket::validate() const {
  if (!this->isValid()) {
    D_MYERR("socket is invalid");
    return false;
  }
  if (!this->isInitialised()) {
    D_MYERR("socket not initialised");
    return false;
  }
  return true;
}

bool AbstractSocket::isValid() const { return _isValid; }
bool AbstractSocket::isInitialised() const { return _handle != -1; }

} // namespace network
} // namespace gero
