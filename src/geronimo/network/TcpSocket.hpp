
#pragma once

#include "AbstractSocket.hpp"
#include "IpAddress.hpp"

namespace gero {
namespace network {

template<typename SocketType>
class SocketSelector;

template<typename SocketType>
class SocketPoller;

class TcpSocket : public AbstractSocket {

  friend SocketSelector<TcpSocket>;
  friend SocketPoller<TcpSocket>;

public:
  TcpSocket() = default;
  TcpSocket(const TcpSocket& other) = delete; // TODO
  TcpSocket(TcpSocket&& other) = delete; // TODO
  TcpSocket& operator=(const TcpSocket& other) = delete; // TODO
  TcpSocket& operator=(TcpSocket&& other) = delete; // TODO
  virtual ~TcpSocket() = default;

public:
  bool operator==(const TcpSocket& other) const;
  bool operator==(int otherHandle) const;

public:
  bool create(bool inAsBlocking = true);

  bool disableNaggleAlgorithm();

  //

  bool listen(const IpAddress& inAddress, uint16_t inPort);
  socket::status accept(TcpSocket& inRemoteSocket, bool inAsBlocking = true);

  //

  bool connect(const IpAddress& remoteAddress, unsigned short remotePort,
               float inTimeout = 0.0f);
  void disconnect();

  socket::status send(const void* inData, std::size_t inSize, std::size_t& outSent);
  socket::status receive(void* outData, std::size_t inMaxSize, std::size_t& outReceived);

};

}
}
