
#include "TcpSocket.hpp"

#include "SocketPoller.hpp"
#include "SocketSelector.hpp"
#include "helpers.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <cstring> // std::memset
#include <netinet/tcp.h> // TCP_NODELAY
#include <cerrno>

// #define D_DEBUG_TCP_SOCKET

#if defined D_DEBUG_TCP_SOCKET
  #define D_MYDEBUG_TCP_SOCKET D_MYERR
#else
  #define D_MYDEBUG_TCP_SOCKET(stream)
#endif

namespace gero {
namespace network {

bool TcpSocket::operator==(const TcpSocket& other) const {
  return this->_handle == other._handle;
}
bool TcpSocket::operator==(int otherHandle) const {
  return this->_handle == otherHandle;
}

bool TcpSocket::create(bool inAsBlocking /*= true*/) {
  if (AbstractSocket::isValid())
    AbstractSocket::close();

  this->_isBlocking = inAsBlocking;
  int32_t socketType = SOCK_STREAM;

  if (!inAsBlocking) {
    socketType |= SOCK_NONBLOCK;
    socketType |= SOCK_CLOEXEC;
  }

  this->_handle = ::socket(PF_INET, socketType, IPPROTO_TCP);

  if (this->_handle == -1) {
    D_MYERR("failed to create socket, message: " << std::strerror(errno));
    return false;
  }

  this->_isValid = true;
  return true;
}

bool TcpSocket::disableNaggleAlgorithm() {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return false;
  }
  // disable the Nagle algorithm
  const int32_t yes = 1;
  if (::setsockopt(this->_handle, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
    D_MYERR("failed to diable the naggle algorithm, message: " << std::strerror(errno));
    return false;
  }
  return true;
}

//

bool TcpSocket::listen(const IpAddress& inAddress, uint16_t inPort) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return false;
  }

  // bind
  const sockaddr_in addr = helpers::getIpAddr(inAddress, inPort);
  if (::bind(this->_handle, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == -1) {
    D_MYERR("failed to bind the socket, message : " << std::strerror(errno));
    return false;
  }

  // listen
  if (::listen(this->_handle, SOMAXCONN) == -1) {
    D_MYERR("failed to listen the socket, message: " << std::strerror(errno));
    return false;
  }
  return true;
}

socket::status TcpSocket::accept(TcpSocket& inRemoteSocket, bool inAsBlocking /*= true*/) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return socket::status::error;
  }

  sockaddr_in addrData;
  socklen_t addrSize = sizeof(addrData);
  sockaddr* pAddrData = reinterpret_cast<sockaddr*>(&addrData);
  std::memset(pAddrData, 0, addrSize);

  int flags = 0;
  if (inAsBlocking) {
    flags |= SOCK_CLOEXEC;
    flags |= SOCK_NONBLOCK;
  }

  const int32_t remoteHandle = ::accept4(this->_handle, pAddrData, &addrSize, flags);

  if (remoteHandle <= 0) {
    socket::status newStatus = helpers::getStatus();
    if (newStatus != socket::status::done)
    {
      if (newStatus == socket::status::error)
      {
        D_MYERR("failed to accept the socket");
        AbstractSocket::close();
      }
      return newStatus;
    }
  }

  if (inRemoteSocket.isValid())
    inRemoteSocket.disconnect(); // reset

  inRemoteSocket._handle = remoteHandle;
  inRemoteSocket._isValid = true;
  inRemoteSocket._isBlocking = inAsBlocking;
  return socket::status::done;
}

//

bool TcpSocket::connect(const IpAddress& remoteAddress, uint16_t remotePort, float inTimeout /*= 0.0f*/) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return false;
  }

  sockaddr_in addrData = helpers::getIpAddr(remoteAddress, remotePort);
  const socklen_t addrSize = sizeof(addrData);
  sockaddr* pAddrData = reinterpret_cast<sockaddr*>(&addrData);

  if (inTimeout == 0.0f) {
    if (::connect(this->_handle, pAddrData, addrSize) == -1) {
      D_MYERR("failed to connect, message: " << std::strerror(errno));
      return false;
    }
    return true;
  }

  const bool wasBlocking = this->_isBlocking;

  // Switch to non-blocking to enable our connection timeout
  if (wasBlocking)
    this->setAsBlocking(false);

  if (::connect(this->_handle, pAddrData, addrSize) >= 0) {
    D_MYDEBUG_TCP_SOCKET("instantly connected");

    if (wasBlocking) {
      this->setAsBlocking(true);
    }

    return true;
  }

  if (!wasBlocking)
    return true;

#if 1

  SocketPoller<TcpSocket> connecPoller;
  connecPoller.create(1);
  connecPoller.add(*this, false, true);
  connecPoller.wait([this](TcpSocket& currSocket, bool isRead, bool isWrite) {

    static_cast<void>(isRead); // unused

    if (!isWrite)
      return true;

    sockaddr_in tmpAddrData;
    socklen_t tmpAddrSize = sizeof(tmpAddrData);
    sockaddr* pTmpAddrData = reinterpret_cast<sockaddr*>(&tmpAddrData);
    const int32_t result = ::getpeername(currSocket._handle, pTmpAddrData, &tmpAddrSize);

    if (result != -1) {
      const uint32_t remoteAddr = ::ntohl(tmpAddrData.sin_addr.s_addr);
      if (remoteAddr != 0) {
        // set back as a blocking socket
        this->setAsBlocking(true);
        return true;
      }
    }
    AbstractSocket::close();
    return true;
  },
  [](AbstractSocket& currSocket) {
    static_cast<void>(currSocket); // unused
  },
  inTimeout);

  return AbstractSocket::validate();

#else

  SocketSelector<TcpSocket> connectSelector;
  connectSelector.add(*this);

  if (
    !connectSelector.waitWrite(inTimeout) ||
    !connectSelector.isReady(*this)
  ) {
    return false;
  }

  sockaddr_in tmpAddrData;
  socklen_t tmpAddrSize = sizeof(tmpAddrData);
  sockaddr* pTmpAddrData = reinterpret_cast<sockaddr*>(&tmpAddrData);
  const int32_t result = ::getpeername(this->_handle, pTmpAddrData, &tmpAddrSize);

  if (result != -1)
  {
    const uint32_t remoteAddr = ::ntohl(tmpAddrData.sin_addr.s_addr);
    if (remoteAddr != 0) {
      // set back as a blocking socket
      this->setAsBlocking(true);
      return true;
    }
  }

  return false;

#endif

}

void TcpSocket::disconnect() { AbstractSocket::close(); }

socket::status TcpSocket::send(const void* inData, std::size_t inSize, std::size_t& outSent) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return socket::status::error;
  }

  if (!inData || inSize == 0) {
    D_MYERR("send(), cannot send empty data");
    return socket::status::error;
  }

  int64_t latestResult = 0;
  for (outSent = 0; outSent < inSize; outSent += std::size_t(latestResult)) {

    const char* pData = static_cast<const char*>(inData) + outSent;
    const std::size_t sizeToSend = inSize - outSent;

    latestResult = ::send(this->_handle, pData, sizeToSend, MSG_NOSIGNAL);

    if (latestResult == 0) {
      // D_MYERR("failed to send data");
      AbstractSocket::close();
      return socket::status::error;
    }

    if (latestResult < 0L) {
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
  }

  return socket::status::done;
}

socket::status TcpSocket::receive(void* outData, std::size_t inMaxSize, std::size_t& outReceived) {
  if (!AbstractSocket::validate()) {
    D_MYERR("failed validation");
    return socket::status::error;
  }

  if (!outData || inMaxSize == 0) {
    D_MYERR("cannot receive empty data");
    return socket::status::error;
  }

  outReceived = 0;

  const int64_t sizeReceived = ::recv(this->_handle, static_cast<char*>(outData), inMaxSize, MSG_NOSIGNAL);

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
        // D_MYERR("failed to receive data");
        AbstractSocket::close();
      }
      return newStatus;
    }
  }

  outReceived = static_cast<std::size_t>(sizeReceived);
  return socket::status::done;
}

}
}
