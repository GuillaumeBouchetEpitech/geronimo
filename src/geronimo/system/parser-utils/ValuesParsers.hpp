
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <optional>
#include <regex>

namespace gero {
namespace valuesParsers {

//
//

class IntValueParser {
private:
  std::regex _regexp;

public:
  IntValueParser();

public:
  std::optional<int32_t> validate(const std::string_view toSearch);
};

//
//

class IntVec2ValueParser {
private:
  std::regex _regexp;

public:
  IntVec2ValueParser();

public:
  std::optional<glm::ivec2> validate(const std::string_view toSearch);
};

//
//

class IntVec3ValueParser {
private:
  std::regex _regexp;

public:
  IntVec3ValueParser();

public:
  std::optional<glm::ivec3> validate(const std::string_view toSearch);
};

//
//

class IntVec4ValueParser {
private:
  std::regex _regexp;

public:
  IntVec4ValueParser();

public:
  std::optional<glm::ivec4> validate(const std::string_view toSearch);
};

//
//

class FloatValueParser {
private:
  std::regex _regexp;

public:
  FloatValueParser();

public:
  std::optional<float> validate(const std::string_view toSearch);
};

//
//

class FloatVec2ValueParser {
private:
  std::regex _regexp;

public:
  FloatVec2ValueParser();

public:
  std::optional<glm::vec2> validate(const std::string_view toSearch);
};

//
//

//
//

class FloatVec3ValueParser {
private:
  std::regex _regexp;

public:
  FloatVec3ValueParser();

public:
  std::optional<glm::vec3> validate(const std::string_view toSearch);
};

//
//

//
//

class FloatVec4ValueParser {
private:
  std::regex _regexp;

public:
  FloatVec4ValueParser();

public:
  std::optional<glm::vec4> validate(const std::string_view toSearch);
};

//
//

} // namespace valuesParsers
} // namespace gero
