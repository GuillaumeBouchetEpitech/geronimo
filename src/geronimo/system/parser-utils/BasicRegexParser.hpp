
#pragma once

#include "ValuesParsers.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <regex>

namespace gero {
namespace parserUtils {

class BasicRegexParser {
private:
  struct Regexps {
    std::regex regexpMain;
    std::regex regexpName;
    std::regex regexpFileName;
  } _regexps;

  valuesParsers::IntValueParser _intValueParser;
  valuesParsers::IntVec2ValueParser _intVec2ValueParser;
  valuesParsers::IntVec3ValueParser _intVec3ValueParser;
  valuesParsers::IntVec4ValueParser _intVec4ValueParser;
  valuesParsers::FloatValueParser _floatValueParser;
  valuesParsers::FloatVec2ValueParser _floatVec2ValueParser;
  valuesParsers::FloatVec3ValueParser _floatVec3ValueParser;
  valuesParsers::FloatVec4ValueParser _floatVec4ValueParser;

  std::string _errorHint;

public:
  BasicRegexParser();

  void setErrorHint(const std::string_view errorHint);

  std::string_view getName(const std::string_view toSearch);
  std::string_view getFileName(const std::string_view toSearch);

  uint32_t get1UI(const std::string_view toSearch);
  uint32_t get1UI(const std::string_view toSearch, uint32_t maxValue);

  glm::uvec2 get2UI(const std::string_view toSearch);
  glm::uvec2 get2UI(const std::string_view toSearch, uint32_t maxValue);

  glm::uvec3 get3UI(const std::string_view toSearch);
  glm::uvec3 get3UI(const std::string_view toSearch, uint32_t maxValue);

  glm::uvec4 get4UI(const std::string_view toSearch);
  glm::uvec4 get4UI(const std::string_view toSearch, uint32_t maxValue);

  float get1F(const std::string_view toSearch);
  float get1F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec2 get2F(const std::string_view toSearch);
  glm::vec2 get2F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec3 get3F(const std::string_view toSearch);
  glm::vec3 get3F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec4 get4F(const std::string_view toSearch);
  glm::vec4 get4F(const std::string_view toSearch, float minValue, float maxValue);

  uint32_t forEachArgs(const std::string_view toSearch,
                   const std::function<void(const std::string_view, const std::string_view)>& callback);

private:
  float _getValidFloat(const std::string_view toSearch);
};

} // namespace parserUtils
} // namespace gero
