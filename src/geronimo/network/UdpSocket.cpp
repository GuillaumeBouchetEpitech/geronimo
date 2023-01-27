
#include "UdpSocket.hpp"
#include "helpers.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"

#include <arpa/inet.h> // sockaddr, sockaddr_in, socklen_t
#include <cstring>  // std::memset

namespace gero {
namespace network {

bool UdpSocket::create(bool inAsBlocking /*= true*/) {
  if (AbstractSocket::isValid())
    AbstractSocket::close();

  this->_isBlocking = inAsBlocking;
  int32_t socketType = SOCK_DGRAM;

  if (!inAsBlocking) {
    socketType |= SOCK_NONBLOCK;
    socketType |= SOCK_CLOEXEC;
  }

  this->_handle = ::socket(PF_INET, socketType, IPPROTO_UDP);

  if (this->_handle == -1) {
    D_MYERR("failed to create socket, message: " << std::strerror(errno));
    return false;
  }

  this->_isValid = true;
  return true;
}

bool UdpSocket::enableBroadcast() {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return false;
  }
  const int32_t yes = 1;
  if (::setsockopt(this->_handle, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) == -1) {
    D_MYERR("failed to enable broadcast, message: " << std::strerror(errno));
    return false;
  }
  return true;
}

//

bool UdpSocket::bind(const IpAddress& inAddress, uint16_t inPort) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return false;
  }

  sockaddr_in addrData = helpers::getIpAddr(inAddress, inPort);
  const socklen_t addrSize = sizeof(addrData);
  sockaddr* pAddrData = reinterpret_cast<sockaddr*>(&addrData);

  if (::bind(this->_handle, pAddrData, addrSize) == -1) {
    D_MYERR("failed to bind the socket, message: " << std::strerror(errno));
    return false;
  }
  return true;
}

bool UdpSocket::unbind() { return AbstractSocket::close(); }

//

socket::status UdpSocket::send(const void* inData, std::size_t inMaxSize, const IpAddress& inRemoteAddress, uint16_t inRemotePort, std::size_t& outSent, bool inAsBlocking /*= true*/) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return socket::status::error;
  }

  if (!inData || inMaxSize == 0) {
    D_MYERR("send(), cannot send empty data");
    return socket::status::error;
  }

  outSent = 0;

  sockaddr_in addrData = helpers::getIpAddr(inRemoteAddress, inRemotePort);
  const socklen_t addrSize = sizeof(addrData);
  sockaddr* pAddrData = reinterpret_cast<sockaddr*>(&addrData);

  //

  int flags = 0;
  if (!inAsBlocking)
    flags |= MSG_DONTWAIT;

  //

  const int64_t sizeSent = ::sendto(this->_handle, static_cast<const char*>(inData), inMaxSize, flags, pAddrData, addrSize);

  if (sizeSent == 0) {
    // D_MYDEBUG("[udp socket] DEAD");
    AbstractSocket::close();
    return socket::status::error;
  }

  if (sizeSent < 0) {

    socket::status newStatus = helpers::getStatus();
    if (newStatus != socket::status::done)
    {
      if (newStatus == socket::status::error)
      {
        // D_MYERR("failed to send data");
        AbstractSocket::close();
      }
      return newStatus;
    }
  }

  outSent = std::size_t(sizeSent);

  return socket::status::done;
}

socket::status UdpSocket::receive(void* outData, std::size_t inMaxSize, std::size_t& outReceived, IpAddress& outRemoteAddress, uint16_t& outRemotePort, bool inAsBlocking /*= true*/) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return socket::status::error;
  }

  if (!outData || inMaxSize == 0) {
    D_MYERR("cannot receive empty data");
    return socket::status::error;
  }

  outReceived = 0;
  outRemoteAddress.reset();
  outRemotePort = 0;

  sockaddr_in addrData;
  socklen_t addrSize = sizeof(addrData);
  std::memset(&addrData, 0, addrSize);
  addrData.sin_family = AF_INET;
  addrData.sin_addr.s_addr = htonl(INADDR_ANY); // TODO: every time?
  addrData.sin_port = htons(0); // TODO: every time?

  sockaddr* pAddrData = reinterpret_cast<sockaddr*>(&addrData);

  //

  int flags = 0;
  if (!inAsBlocking)
    flags |= MSG_DONTWAIT;

  //

  const int64_t sizeReceived = ::recvfrom(this->_handle, static_cast<char*>(outData), inMaxSize, flags, pAddrData, &addrSize);

  if (sizeReceived == 0) {
    // D_MYERR("failed to receive data");
    AbstractSocket::close();
    return socket::status::error;
  }

  if (sizeReceived < 0) {
    socket::status newStatus = helpers::getStatus();
    if (newStatus != socket::status::done)
    {
      if (newStatus == socket::status::error)
      {
        D_MYERR("failed to receive data");
        AbstractSocket::close();
      }
      return newStatus;
    }
  }

  //

  outReceived = std::size_t(sizeReceived);
  outRemoteAddress = addrData.sin_addr.s_addr;
  outRemotePort = ntohs(addrData.sin_port);

  return socket::status::done;
}

} // namespace network
} // namespace gero
