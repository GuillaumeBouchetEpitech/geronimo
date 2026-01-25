

#include "stream-formatter.hpp"

// #include "ErrorHandler.hpp"

#include <iomanip>
#include <memory>

#include <ctime>

namespace gero {

std::string StreamFormatter::getData() const { return _sstr.str(); }

template <> StreamFormatter& StreamFormatter::operator<< <bool>(bool data) {
  _sstr << std::boolalpha << data;
  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <float>(float data) {
  _sstr << std::fixed << std::setprecision(2) << data;
  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <double>(double data) {
  _sstr << std::fixed << std::setprecision(2) << data;
  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec2>(glm::ivec2 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec3>(glm::ivec3 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec4>(glm::ivec4 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << " / ";
  (*this) << data.w;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec2>(glm::uvec2 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec3>(glm::uvec3 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec4>(glm::uvec4 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << " / ";
  (*this) << data.w;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::vec2>(glm::vec2 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::vec3>(glm::vec3 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::vec4>(glm::vec4 data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << " / ";
  (*this) << data.w;
  _sstr << "]";

  return *this;
}

template <> StreamFormatter& StreamFormatter::operator<< <glm::quat>(glm::quat data) {
  _sstr << "[";
  (*this) << data.x;
  _sstr << " / ";
  (*this) << data.y;
  _sstr << " / ";
  (*this) << data.z;
  _sstr << " / ";
  (*this) << data.w;
  _sstr << "]";

  return *this;
}

} // namespace gero
