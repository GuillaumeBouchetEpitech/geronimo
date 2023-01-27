
#include "IpAddress.hpp"

#include <arpa/inet.h> // inet_addr

namespace gero {
namespace network {

IpAddress::IpAddress(uint32_t inAddr) : _addr(inAddr) {}

IpAddress::IpAddress(const char* inAddr) : _addr(::inet_addr(inAddr)) {}

IpAddress::IpAddress(const IpAddress& other) { _addr = other._addr; }
IpAddress::IpAddress(IpAddress&& other) { std::swap(_addr, other._addr); }
IpAddress& IpAddress::operator=(const IpAddress& other) { _addr = other._addr; return *this; }
IpAddress& IpAddress::operator=(IpAddress&& other) { std::swap(_addr, other._addr); return *this; }

//

void IpAddress::reset() { _addr = 0; }

//

std::string IpAddress::asString() const {
  struct in_addr tmpAddr;
  // tmpAddr.s_addr = ::htonl(_addr);
  tmpAddr.s_addr = _addr;
  return ::inet_ntoa(tmpAddr);
}

uint32_t IpAddress::asInteger() const { return _addr; }

} // namespace network
} // namespace gero
