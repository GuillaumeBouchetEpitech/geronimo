
// #include "SocketSelector.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"

namespace gero {
namespace network {

template<typename SocketType>
bool SocketSelector<SocketType>::add(const SocketType& inSocket) {
  if (!inSocket.validate()) {
    D_MYERR("trying to add an invalid socket");
    return false;
  }

  if (inSocket._handle >= FD_SETSIZE) {
    D_MYERR("trying to add too many sockets");
    return false;
  }

  _maxSocket = std::max(_maxSocket, inSocket._handle);

  FD_SET(inSocket._handle, &_allSockets);
  return true;
}

template<typename SocketType>
bool SocketSelector<SocketType>::remove(const SocketType& inSocket) {
  if (!inSocket.isInitialised()) {
    D_MYERR("trying to remove an invalid socket");
    return false;
  }

  FD_CLR(inSocket._handle, &_allSockets);
  FD_CLR(inSocket._handle, &_socketsReady);
  return true;
}

template<typename SocketType>
void SocketSelector<SocketType>::clear() {
  FD_ZERO(&_allSockets);
  FD_ZERO(&_socketsReady);

  _maxSocket = 0;
}

template<typename SocketType>
bool SocketSelector<SocketType>::waitRead(float inTimeout /*= 0.0f*/) {
  timeval timeData;
  timeval* pTimeData = nullptr;

  if (inTimeout > 0.0f) {
    const int64_t microSec = int64_t(inTimeout * 1000000.0f);
    timeData.tv_sec = static_cast<int64_t>(microSec / 1000000);
    timeData.tv_usec = static_cast<int64_t>(microSec % 1000000);
    pTimeData = &timeData;
  }

  _socketsReady = _allSockets;
  fd_set* readHandles = &_socketsReady;
  fd_set* writeHandles = nullptr;

  const int32_t count = ::select(_maxSocket + 1, readHandles, writeHandles, nullptr, pTimeData);

  if (count < 0) {
    D_MYERR("select returned an error, message: " << std::strerror(errno));
    return false;
  }

  return count > 0;
}

template<typename SocketType>
bool SocketSelector<SocketType>::waitWrite(float inTimeout /*= 0.0f*/) {
  timeval timeData;
  timeval* pTimeData = nullptr;

  if (inTimeout > 0.0f) {
    const int64_t microSec = int64_t(inTimeout * 1000000.0f);
    timeData.tv_sec = static_cast<int64_t>(microSec / 1000000);
    timeData.tv_usec = static_cast<int64_t>(microSec % 1000000);
    pTimeData = &timeData;
  }

  _socketsReady = _allSockets;
  fd_set* readHandles = nullptr;
  fd_set* writeHandles = &_socketsReady;

  const int32_t count = ::select(_maxSocket + 1, readHandles, writeHandles, nullptr, pTimeData);

  if (count < 0) {
    D_MYERR("select returned an error, message: " << std::strerror(errno));
  }

  return count > 0;
}

template<typename SocketType>
bool SocketSelector<SocketType>::isReady(const SocketType& inSocket) {
  if (inSocket._handle == -1) {
    D_MYERR("trying to check an invalid socket");
    return false;
  }

  if (inSocket._handle >= FD_SETSIZE)
    return false;

  return (FD_ISSET(inSocket._handle, &_socketsReady) != 0);
}

} // namespace network
} // namespace gero
