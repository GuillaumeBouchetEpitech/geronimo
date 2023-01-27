
#pragma once

#include <cstdint>
#include <string>

namespace gero {
namespace network {

class IpAddress {
private:
  uint32_t _addr = 0;

public:
  IpAddress() = default;
  IpAddress(uint32_t addr);
  IpAddress(const char* addr);

  IpAddress(const IpAddress& other);
  IpAddress(IpAddress&& other);
  IpAddress& operator=(const IpAddress& other);
  IpAddress& operator=(IpAddress&& other);

public:
  void reset();

public:
  std::string asString() const;
  uint32_t asInteger() const;
};

} // namespace network
} // namespace gero
