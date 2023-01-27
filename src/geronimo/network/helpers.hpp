
#pragma once

#include "AbstractSocket.hpp"
#include "IpAddress.hpp"

#include <arpa/inet.h> // inet_addr

namespace gero {
namespace network {
namespace helpers {

sockaddr_in getIpAddr(const IpAddress& address, uint16_t port);

socket::status getStatus();

}
}
}
