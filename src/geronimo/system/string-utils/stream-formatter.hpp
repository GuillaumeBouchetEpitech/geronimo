
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <cstring> // <= strrchr()
#include <sstream> // <= std::stringstream

#include <cstdint>
#include <mutex>
#include <ostream>
#include <vector>

namespace gero {

class StreamFormatter {

public:
  static void log(const std::string& msg);

private:
  std::stringstream _sstr;

public:
  StreamFormatter() = default;

public:
  std::string getData() const;

public:
  template <typename T> StreamFormatter& operator<<(T data) {
    _sstr << data;
    return *this;
  }
};

template <> StreamFormatter& StreamFormatter::operator<< <bool>(bool data);

template <> StreamFormatter& StreamFormatter::operator<< <float>(float data);

template <> StreamFormatter& StreamFormatter::operator<< <double>(double data);

template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec2>(glm::ivec2 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec3>(glm::ivec3 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::ivec4>(glm::ivec4 data);

template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec2>(glm::uvec2 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec3>(glm::uvec3 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::uvec4>(glm::uvec4 data);

template <> StreamFormatter& StreamFormatter::operator<< <glm::vec2>(glm::vec2 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::vec3>(glm::vec3 data);
template <> StreamFormatter& StreamFormatter::operator<< <glm::vec4>(glm::vec4 data);

template <> StreamFormatter& StreamFormatter::operator<< <glm::quat>(glm::quat data);

} // namespace gero
