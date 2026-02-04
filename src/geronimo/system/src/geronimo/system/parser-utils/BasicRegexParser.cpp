
#include "BasicRegexParser.hpp"

#include "geronimo/system/getStreamStr.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/math/validation-helpers.hpp"

#include "string_view_regexp.hpp"

namespace gero {
namespace parserUtils {

BasicRegexParser::BasicRegexParser() {

  const std::string_view matchStrName = R"(([\w\-]+))";
  const std::string_view matchStrFileName = R"(([\w\-\.\/]+))";
  const std::string_view matchStrValue = R"(\"(.+?)\")";
  const std::string matchMain = D_SSTR(matchStrName << "=" << matchStrValue);

  _regexps.regexpMain = std::regex(matchMain.data(), matchMain.size());
  _regexps.regexpName = std::regex(matchStrName.data(), matchStrName.size());
  _regexps.regexpFileName = std::regex(matchStrFileName.data(), matchStrFileName.size());
}

void BasicRegexParser::setErrorHint(const std::string_view errorHint) { _errorHint = errorHint; }

std::string_view BasicRegexParser::getName(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpName);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse name" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  std::string_view result = string_view_regexp::get_string_view(subMatch[1]);

  // if (result.empty())
  //   D_THROW(std::runtime_error, "invalid name, type=" << _errorHint);

  return result;
}

std::string_view BasicRegexParser::getFileName(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpFileName);
  if (subMatch.empty())
    D_THROW(std::runtime_error,
            "cannot parse file name" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return string_view_regexp::get_string_view(subMatch[1]);
}

uint32_t BasicRegexParser::get1UI(const std::string_view toSearch) {

  auto result = _intValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 1UI" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return uint32_t(*result);
}

uint32_t BasicRegexParser::get1UI(const std::string_view toSearch, uint32_t maxValue) {
  const uint32_t value = get1UI(toSearch);
  if (value > maxValue)
    D_THROW(std::runtime_error,
            "value of 1UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value=" << value << ", maxValue=" << maxValue);
  return value;
}

glm::uvec2 BasicRegexParser::get2UI(const std::string_view toSearch) {

  auto result = _intVec2ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 2UI" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return glm::uvec2(result->x, result->y);
}

glm::uvec2 BasicRegexParser::get2UI(const std::string_view toSearch, uint32_t maxValue) {
  const glm::uvec2 value = get2UI(toSearch);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 2UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.x=" << value.x << ", maxValue=" << maxValue);

  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 2UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.y=" << value.y << ", maxValue=" << maxValue);

  return value;
}

glm::uvec3 BasicRegexParser::get3UI(const std::string_view toSearch) {

  auto result = _intVec3ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 3UI" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return glm::uvec3(result->x, result->y, result->z);
}

glm::uvec3 BasicRegexParser::get3UI(const std::string_view toSearch, uint32_t maxValue) {
  const glm::uvec3 value = get3UI(toSearch);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 3UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.x=" << value.x << ", maxValue=" << maxValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 3UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.y=" << value.y << ", maxValue=" << maxValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error,
            "value.z of 3UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.z=" << value.z << ", maxValue=" << maxValue);
  return value;
}

glm::uvec4 BasicRegexParser::get4UI(const std::string_view toSearch) {

  auto result = _intVec4ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 4UI" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return glm::uvec4(result->x, result->y, result->z, result->w);
}

glm::uvec4 BasicRegexParser::get4UI(const std::string_view toSearch, uint32_t maxValue) {
  const glm::uvec4 value = get4UI(toSearch);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 4UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.x=" << value.x << ", maxValue=" << maxValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 4UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.y=" << value.y << ", maxValue=" << maxValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error,
            "value.z of 4UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.z=" << value.z << ", maxValue=" << maxValue);
  if (value.w > maxValue)
    D_THROW(std::runtime_error,
            "value.w of 4UI is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                         << ", value.w=" << value.w << ", maxValue=" << maxValue);
  return value;
}

float BasicRegexParser::get1F(const std::string_view toSearch) {

  auto result = _floatValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 1F" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return *result;
}

float BasicRegexParser::get1F(const std::string_view toSearch, float minValue, float maxValue) {
  float value = get1F(toSearch);

  if (value < minValue)
    D_THROW(std::runtime_error,
            "value of 1F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                     << ", value=" << value << ", minValue=" << minValue);
  if (value > maxValue)
    D_THROW(std::runtime_error,
            "value of 1F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                      << ", value=" << value << ", maxValue=" << maxValue);

  return value;
}

glm::vec2 BasicRegexParser::get2F(const std::string_view toSearch) {

  auto result = _floatVec2ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 2F" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return *result;
}

glm::vec2 BasicRegexParser::get2F(const std::string_view toSearch, float minValue, float maxValue) {
  glm::vec2 value = get2F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error,
            "value.x of 2F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.x=" << value.x << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 2F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.x=" << value.x << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error,
            "value.y of 2F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.y=" << value.y << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 2F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.y=" << value.y << ", maxValue=" << maxValue);

  return value;
}

glm::vec3 BasicRegexParser::get3F(const std::string_view toSearch) {

  auto result = _floatVec3ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 3F" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return *result;
}

glm::vec3 BasicRegexParser::get3F(const std::string_view toSearch, float minValue, float maxValue) {
  glm::vec3 value = get3F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error,
            "value.x of 3F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.x=" << value.x << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 3F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.x=" << value.x << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error,
            "value.y of 3F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.y=" << value.y << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 3F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.y=" << value.y << ", maxValue=" << maxValue);

  if (value.z < minValue)
    D_THROW(std::runtime_error,
            "value.z of 3F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.z=" << value.z << ", minValue=" << minValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error,
            "value.z of 3F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.z=" << value.z << ", maxValue=" << maxValue);

  return value;
}

glm::vec4 BasicRegexParser::get4F(const std::string_view toSearch) {

  auto result = _floatVec4ValueParser.validate(toSearch);

  if (!result)
    D_THROW(std::runtime_error, "cannot parse 4F" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return *result;
}

glm::vec4 BasicRegexParser::get4F(const std::string_view toSearch, float minValue, float maxValue) {
  glm::vec4 value = get4F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error,
            "value.x of 4F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.x=" << value.x << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error,
            "value.x of 4F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.x=" << value.x << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error,
            "value.y of 4F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.y=" << value.y << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error,
            "value.y of 4F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.y=" << value.y << ", maxValue=" << maxValue);

  if (value.z < minValue)
    D_THROW(std::runtime_error,
            "value.z of 4F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.z=" << value.z << ", minValue=" << minValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error,
            "value.z of 4F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.z=" << value.z << ", maxValue=" << maxValue);

  if (value.w < minValue)
    D_THROW(std::runtime_error,
            "value.w of 4F is too low" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                       << ", value.w=" << value.w << ", minValue=" << minValue);
  if (value.w > maxValue)
    D_THROW(std::runtime_error,
            "value.w of 4F is too high" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\""
                                        << ", value.w=" << value.w << ", maxValue=" << maxValue);

  return value;
}

uint32_t
BasicRegexParser::forEachArgs(const std::string_view toSearch,
                              const std::function<void(const std::string_view, const std::string_view)>& callback) {
  uint32_t totalArgs = 0;
  string_view_regexp::match match;
  std::string_view::const_iterator searchStart(toSearch.cbegin());
  while (string_view_regexp::regex_search(searchStart, toSearch.cend(), match, _regexps.regexpMain)) {
    std::string_view key = string_view_regexp::get_string_view(match[1]);
    std::string_view value = string_view_regexp::get_string_view(match[2]);

    callback(key, value);
    ++totalArgs;

    searchStart = match.suffix().first;
  }

  return totalArgs;
}

float BasicRegexParser::_getValidFloat(const std::string_view toSearch) {
  const float value = float(std::atof(toSearch.data()));
  if (math::isInvalidFloat(value))
    D_THROW(std::runtime_error,
            "invalid value (NaN/inf)" << ", type=" << _errorHint << ", toSearch=\"" << toSearch << "\"");

  return value;
}

} // namespace parserUtils
} // namespace gero
