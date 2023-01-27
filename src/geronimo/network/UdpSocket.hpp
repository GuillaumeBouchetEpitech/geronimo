
#pragma once

#include "AbstractSocket.hpp"
#include "IpAddress.hpp"

namespace gero {
namespace network {

template<typename SocketType>
class SocketSelector;

template<typename SocketType>
class SocketPoller;

class UdpSocket : public AbstractSocket {

  friend SocketSelector<UdpSocket>;
  friend SocketPoller<UdpSocket>;

public:
  UdpSocket() = default;
  UdpSocket(const UdpSocket& other) = delete; // TODO
  UdpSocket(UdpSocket&& other) = delete; // TODO
  UdpSocket& operator=(const UdpSocket& other) = delete; // TODO
  UdpSocket& operator=(UdpSocket&& other) = delete; // TODO
  virtual ~UdpSocket() = default;

public:
  bool create(bool inAsBlocking = true);

public:
  bool enableBroadcast();

public:
  bool bind(const IpAddress& inAddress, uint16_t inPort);
  bool unbind();

public:
  socket::status send(const void* inData, std::size_t inMaxSize, const IpAddress& inRemoteAddress, uint16_t inRemotePort, std::size_t& outSent, bool inAsBlocking = true);
  socket::status receive(void* outData, std::size_t inMaxSize, std::size_t& outReceived, IpAddress& outRemoteAddress, uint16_t& outRemotePort, bool inAsBlocking = true);

};

} // namespace network
} // namespace gero
