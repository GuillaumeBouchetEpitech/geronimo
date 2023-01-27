
#pragma once

#include <netdb.h> // fd_set

namespace gero {
namespace network {

template<typename SocketType>
class SocketSelector {
private:
  fd_set _allSockets;
  fd_set _socketsReady;
  int32_t _maxSocket = 0;

public:
  SocketSelector() = default;
  SocketSelector(const SocketSelector& other) = delete; // TODO
  SocketSelector(SocketSelector&& other) = delete; // TODO
  SocketSelector& operator=(const SocketSelector& other) = delete; // TODO
  SocketSelector& operator=(SocketSelector&& other) = delete; // TODO
  ~SocketSelector() = default;

public:
  bool add(const SocketType& socket);
  bool remove(const SocketType& inSocket);
  void clear();

public:
  bool waitRead(float inTimeout /*= 0.0f*/);
  bool waitWrite(float inTimeout /*= 0.0f*/);
  bool isReady(const SocketType& inSocket);

};

} // namespace network
} // namespace gero

#include "SocketSelector.inl"
