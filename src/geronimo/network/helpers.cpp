
#include "helpers.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"

#include <cstring>  // std::memset

namespace gero {
namespace network {
namespace helpers {

sockaddr_in getIpAddr(const IpAddress& address, uint16_t port) {
  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = address.asInteger();
  addr.sin_port = ::htons(port);
  return addr;
}

socket::status getStatus()
{

#if defined D_DEBUG_ABSTRACT_SOCKET

  {

    // D_MYDEBUG_ABSTRACT_SOCKET("[tcp socket] RECEIVE, sizeReceived=" << sizeReceived);
    D_MYDEBUG_ABSTRACT_SOCKET(" => errno = " << errno);
    D_MYDEBUG_ABSTRACT_SOCKET(" => ERR: " << std::strerror(errno));

    std::stringstream sstr;

    sstr << "[";
    if (errno == EAGAIN) sstr << " EAGAIN";
    if (errno == EWOULDBLOCK) sstr << " EWOULDBLOCK";
    if (errno == EINPROGRESS) sstr << " EINPROGRESS";
    if (errno == ECONNABORTED) sstr << " ECONNABORTED";
    if (errno == ECONNRESET) sstr << " ECONNRESET";
    if (errno == ETIMEDOUT) sstr << " ETIMEDOUT";
    if (errno == ENETRESET) sstr << " ENETRESET";
    if (errno == ENOTCONN) sstr << " ENOTCONN";
    if (errno == EPIPE) sstr << " EPIPE";
    sstr << " ]";

    std::string str = sstr.str();

    D_MYDEBUG_ABSTRACT_SOCKET(" => " << str);

  }

#endif

  // clang-format off
  if (
    (errno == EAGAIN) ||
    (errno == EWOULDBLOCK) ||
    (errno == EINPROGRESS)
  ) {
    return socket::status::notReady;
  }

  // clang-format off
  switch (errno)
  {
    case ECONNABORTED:
    case ECONNRESET:
    case ETIMEDOUT:
    case ENETRESET:
    case ENOTCONN:
    case EPIPE:
    default:
    {
      D_MYERR("errno error, message: " << std::strerror(errno));
      return socket::status::error;
    }
  }

  return socket::status::done;
}

}
}
}
