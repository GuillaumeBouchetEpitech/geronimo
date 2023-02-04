
#include "BasicRegexParser.hpp"

#include "geronimo/system/getStreamStr.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/math/validation-helpers.hpp"

namespace string_view_regexp {

  using match = std::match_results<std::string_view::const_iterator>;
  using sub_match = std::sub_match<std::string_view::const_iterator>;

  inline std::string_view get_string_view(const sub_match& m)
  {
    return std::string_view(m.first, std::size_t(m.length()));
  }

  inline bool regex_match(
    std::string_view sv,
    match& m,
    const std::regex& e,
    std::regex_constants::match_flag_type flags = std::regex_constants::match_default)
  {
    return std::regex_match(sv.begin(), sv.end(), m, e, flags);
  }

  inline bool regex_match(
    std::string_view sv,
    const std::regex& e,
    std::regex_constants::match_flag_type flags = std::regex_constants::match_default)
  {
    return std::regex_match(sv.begin(), sv.end(), e, flags);
  }

  inline bool regex_search(
    std::string_view sv,
    match& m,
    std::regex& r)
  {
    return std::regex_search(sv.begin(), sv.end(), m, r);
  }

  inline bool regex_search(
    std::string_view::const_iterator start,
    std::string_view::const_iterator end,
    match& m,
    std::regex& r)
  {
    return std::regex_search(start, end, m, r);
  }

}

namespace gero {
namespace parserUtils {

BasicRegexParser::BasicRegexParser() {

  const std::string_view matchStrName = R"(([\w\-]+))";
  const std::string_view matchStrFileName = R"(([\w\-\.\/]+))";
  const std::string_view matchStrValue = R"(\"(.+?)\")";
  const std::string_view matchMaybeSpace = R"(\s*?)";
  const std::string matchMain = D_SSTR(matchStrName << "=" << matchStrValue);
  const std::string_view match1UI = R"(([+-]?\d+))";
  const std::string_view match1F = R"(([+-]?\d+(?:\.\d+)?))";
  const std::string matchS1UI = D_SSTR(matchMaybeSpace << match1UI << matchMaybeSpace);
  const std::string matchS2UI = D_SSTR(matchS1UI << "," << matchS1UI);
  const std::string matchS3UI = D_SSTR(matchS1UI << "," << matchS1UI << "," << matchS1UI);
  const std::string matchS1F = D_SSTR(matchMaybeSpace << match1F << matchMaybeSpace);
  const std::string matchS2F = D_SSTR(matchS1F << "," << matchS1F);
  const std::string matchS3F = D_SSTR(matchS1F << "," << matchS1F << "," << matchS1F);
  const std::string matchS4F = D_SSTR(matchS1F << "," << matchS1F << "," << matchS1F << "," << matchS1F);

  _regexps.regexpMain = std::regex(matchMain.data(), matchMain.size());
  _regexps.regexpName = std::regex(matchStrName.data(), matchStrName.size());
  _regexps.regexpFileName = std::regex(matchStrFileName.data(), matchStrFileName.size());
  _regexps.regexpS1UI = std::regex(matchS1UI.data(), matchS1UI.size());
  _regexps.regexpS2UI = std::regex(matchS2UI.data(), matchS2UI.size());
  _regexps.regexpS3UI = std::regex(matchS3UI.data(), matchS3UI.size());
  _regexps.regexpS1F = std::regex(match1F.data(), match1F.size());
  _regexps.regexpS2F = std::regex(matchS2F.data(), matchS2F.size());
  _regexps.regexpS3F = std::regex(matchS3F.data(), matchS3F.size());
  _regexps.regexpS4F = std::regex(matchS4F.data(), matchS4F.size());
}

void BasicRegexParser::setErrorHint(const std::string_view errorHint) {
  _errorHint = errorHint;
}

std::string_view BasicRegexParser::getName(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpName);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse name"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  std::string_view result = string_view_regexp::get_string_view(subMatch[1]);

  // if (result.empty())
  //   D_THROW(std::runtime_error, "invalid name, type=" << _errorHint);

  return result;
}

std::string_view BasicRegexParser::getFileName(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpFileName);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse file name"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return string_view_regexp::get_string_view(subMatch[1]);
}

uint32_t BasicRegexParser::get1UI(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS1UI);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 1UI"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return uint32_t(std::atoi(subMatch[1].str().c_str()));
}

uint32_t BasicRegexParser::get1UI(const std::string_view toSearch,
                                   uint32_t maxValue) {
  const uint32_t value = get1UI(toSearch);
  if (value > maxValue)
    D_THROW(std::runtime_error, "value of 1UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value=" << value
      << ", maxValue=" << maxValue);
  return value;
}

glm::uvec2 BasicRegexParser::get2UI(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS2UI);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 2UI"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return {uint32_t(std::atoi(subMatch[1].str().c_str())),
          uint32_t(std::atoi(subMatch[2].str().c_str()))};
}

glm::uvec2 BasicRegexParser::get2UI(const std::string_view toSearch,
                                     uint32_t maxValue) {
  const glm::uvec2 value = get2UI(toSearch);
  if (value.x > maxValue)
    D_THROW(std::runtime_error, "value.x of 2UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", maxValue=" << maxValue);

  if (value.y > maxValue)
    D_THROW(std::runtime_error, "value.y of 2UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", maxValue=" << maxValue);

  return value;
}

glm::uvec3 BasicRegexParser::get3UI(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS3UI);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 3UI"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return {uint32_t(std::atoi(subMatch[1].str().c_str())),
          uint32_t(std::atoi(subMatch[2].str().c_str())),
          uint32_t(std::atoi(subMatch[3].str().c_str()))};
}

glm::uvec3 BasicRegexParser::get3UI(const std::string_view toSearch,
                                     uint32_t maxValue) {
  const glm::uvec3 value = get3UI(toSearch);
  if (value.x > maxValue)
    D_THROW(std::runtime_error, "value.x of 3UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", maxValue=" << maxValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error, "value.y of 3UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", maxValue=" << maxValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error, "value.z of 3UI is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.z=" << value.z
      << ", maxValue=" << maxValue);
  return value;
}

float BasicRegexParser::get1F(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS1F);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 1F"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return _getValidFloat(subMatch[1].str());
}

float BasicRegexParser::get1F(const std::string_view toSearch, float minValue,
                               float maxValue) {
  float value = get1F(toSearch);

  if (value < minValue)
    D_THROW(std::runtime_error, "value of 1F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value=" << value
      << ", minValue=" << minValue);
  if (value > maxValue)
    D_THROW(std::runtime_error, "value of 1F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value=" << value
      << ", maxValue=" << maxValue);

  return value;
}

glm::vec2 BasicRegexParser::get2F(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS2F);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 2F"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return {_getValidFloat(subMatch[1].str()), _getValidFloat(subMatch[2].str())};
}

glm::vec2 BasicRegexParser::get2F(const std::string_view toSearch, float minValue,
                                   float maxValue) {
  glm::vec2 value = get2F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error, "value.x of 2F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error, "value.x of 2F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error, "value.y of 2F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error, "value.y of 2F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", maxValue=" << maxValue);

  return value;
}

glm::vec3 BasicRegexParser::get3F(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS3F);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 3F"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return {_getValidFloat(subMatch[1].str()), _getValidFloat(subMatch[2].str()),
          _getValidFloat(subMatch[3].str())};
}

glm::vec3 BasicRegexParser::get3F(const std::string_view toSearch, float minValue,
                                   float maxValue) {
  glm::vec3 value = get3F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error, "value.x of 3F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error, "value.x of 3F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error, "value.y of 3F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error, "value.y of 3F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", maxValue=" << maxValue);

  if (value.z < minValue)
    D_THROW(std::runtime_error, "value.z of 3F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.z=" << value.z
      << ", minValue=" << minValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error, "value.z of 3F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.z=" << value.z
      << ", maxValue=" << maxValue);

  return value;
}

glm::vec4 BasicRegexParser::get4F(const std::string_view toSearch) {
  string_view_regexp::match subMatch;
  string_view_regexp::regex_search(toSearch, subMatch, _regexps.regexpS4F);
  if (subMatch.empty())
    D_THROW(std::runtime_error, "cannot parse 4F"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return {_getValidFloat(subMatch[1].str()), _getValidFloat(subMatch[2].str()),
          _getValidFloat(subMatch[3].str()), _getValidFloat(subMatch[4].str())};
}

glm::vec4 BasicRegexParser::get4F(const std::string_view toSearch, float minValue,
                                   float maxValue) {
  glm::vec4 value = get4F(toSearch);

  if (value.x < minValue)
    D_THROW(std::runtime_error, "value.x of 4F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", minValue=" << minValue);
  if (value.x > maxValue)
    D_THROW(std::runtime_error, "value.x of 4F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.x=" << value.x
      << ", maxValue=" << maxValue);

  if (value.y < minValue)
    D_THROW(std::runtime_error, "value.y of 4F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", minValue=" << minValue);
  if (value.y > maxValue)
    D_THROW(std::runtime_error, "value.y of 4F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.y=" << value.y
      << ", maxValue=" << maxValue);

  if (value.z < minValue)
    D_THROW(std::runtime_error, "value.z of 4F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.z=" << value.z
      << ", minValue=" << minValue);
  if (value.z > maxValue)
    D_THROW(std::runtime_error, "value.z of 4F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.z=" << value.z
      << ", maxValue=" << maxValue);

  if (value.w < minValue)
    D_THROW(std::runtime_error, "value.w of 4F is too low"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.w=" << value.w
      << ", minValue=" << minValue);
  if (value.w > maxValue)
    D_THROW(std::runtime_error, "value.w of 4F is too high"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\""
      << ", value.w=" << value.w
      << ", maxValue=" << maxValue);

  return value;
}

void BasicRegexParser::forEachArgs(
  const std::string_view toSearch,
  const std::function<void(const std::string_view, const std::string_view)>& callback)
{
  string_view_regexp::match match;
  std::string_view::const_iterator searchStart(toSearch.cbegin());
  while (string_view_regexp::regex_search(searchStart, toSearch.cend(), match, _regexps.regexpMain)) {
    std::string_view key = string_view_regexp::get_string_view(match[1]);
    std::string_view value = string_view_regexp::get_string_view(match[2]);

    callback(key, value);

    searchStart = match.suffix().first;
  }
}

float BasicRegexParser::_getValidFloat(const std::string_view toSearch) {
  const float value = float(std::atof(toSearch.data()));
  if (math::isInvalidFloat(value))
    D_THROW(std::runtime_error, "invalid value (NaN/inf)"
      << ", type=" << _errorHint
      << ", toSearch=\"" << toSearch << "\"");

  return value;
}

} // namespace parserUtils
} // namespace gero
