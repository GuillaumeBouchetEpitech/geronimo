
#include "ValuesParsers.hpp"

#include "string_view_regexp.hpp"

#include "geronimo/system/math/validation-helpers.hpp"
#include "geronimo/system/ErrorHandler.hpp"

namespace {

std::optional<float> _getValidFloat(const std::string_view toSearch) {
  const float value = float(std::atof(toSearch.data()));
  if (gero::math::isInvalidFloat(value))
    return {};

  return value;
}

}

namespace gero {
namespace valuesParsers {

//
//
//

IntValueParser::IntValueParser()
  : _regexp(R"(\s*?([+-]?\d+)\s*?)")
{}

std::optional<int32_t> IntValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  return int32_t(std::atoi(subMatch[1].str().c_str()));
}

//
//
//

IntVec2ValueParser::IntVec2ValueParser()
  : _regexp(R"(\s*?([+-]?\d+)\s*?,\s*?([+-]?\d+)\s*?)")
{}

std::optional<glm::ivec2> IntVec2ValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  return glm::ivec2(
    uint32_t(std::atoi(subMatch[1].str().c_str())),
    uint32_t(std::atoi(subMatch[2].str().c_str()))
  );

}

//
//
//

IntVec3ValueParser::IntVec3ValueParser()
  : _regexp(R"(\s*?([+-]?\d+)\s*?,\s*?([+-]?\d+)\s*?,\s*?([+-]?\d+)\s*?)")
{}

std::optional<glm::ivec3> IntVec3ValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  return glm::ivec3(
    uint32_t(std::atoi(subMatch[1].str().c_str())),
    uint32_t(std::atoi(subMatch[2].str().c_str())),
    uint32_t(std::atoi(subMatch[3].str().c_str()))
  );

}

//
//
//

FloatValueParser::FloatValueParser()
  : _regexp(R"(\s*?([+-]?\d+(?:\.\d+)?)\s*?)")
{}

std::optional<float> FloatValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  return _getValidFloat(subMatch[1].str());
}

//
//
//

FloatVec2ValueParser::FloatVec2ValueParser()
  : _regexp(R"(\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?)")
{}

std::optional<glm::vec2> FloatVec2ValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  auto x = _getValidFloat(subMatch[1].str());
  auto y = _getValidFloat(subMatch[2].str());

  if (!x || !y)
    return {};

  return glm::vec2(*x, *y);
}

//
//
//

FloatVec3ValueParser::FloatVec3ValueParser()
  : _regexp(R"(\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?)")
{}

std::optional<glm::vec3> FloatVec3ValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  auto x = _getValidFloat(subMatch[1].str());
  auto y = _getValidFloat(subMatch[2].str());
  auto z = _getValidFloat(subMatch[3].str());

  if (!x || !y || !z)
    return {};

  return glm::vec3(*x, *y, *z);
}

//
//
//

FloatVec4ValueParser::FloatVec4ValueParser()
  : _regexp(R"(\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?,\s*?([+-]?\d+(?:\.\d+)?)\s*?)")
{}

std::optional<glm::vec4> FloatVec4ValueParser::validate(const std::string_view toSearch)
{
  parserUtils::string_view_regexp::match subMatch;
  parserUtils::string_view_regexp::regex_search(toSearch, subMatch, _regexp);
  if (subMatch.empty())
    return {};

  auto x = _getValidFloat(subMatch[1].str());
  auto y = _getValidFloat(subMatch[2].str());
  auto z = _getValidFloat(subMatch[3].str());
  auto w = _getValidFloat(subMatch[4].str());

  if (!x || !y || !z || !w)
    return {};

  return glm::vec4(*x, *y, *z, *w);
}

//
//
//

}
}

