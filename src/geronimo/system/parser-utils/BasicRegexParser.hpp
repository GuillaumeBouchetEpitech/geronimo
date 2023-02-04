
#pragma once

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
    std::regex regexpS1UI;
    std::regex regexpS2UI;
    std::regex regexpS3UI;
    std::regex regexpS1F;
    std::regex regexpS2F;
    std::regex regexpS3F;
    std::regex regexpS4F;
  } _regexps;

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

  float get1F(const std::string_view toSearch);
  float get1F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec2 get2F(const std::string_view toSearch);
  glm::vec2 get2F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec3 get3F(const std::string_view toSearch);
  glm::vec3 get3F(const std::string_view toSearch, float minValue, float maxValue);

  glm::vec4 get4F(const std::string_view toSearch);
  glm::vec4 get4F(const std::string_view toSearch, float minValue, float maxValue);

  void forEachArgs(const std::string_view toSearch,
                   const std::function<void(const std::string_view,
                                            const std::string_view)>& callback);

private:
  float _getValidFloat(const std::string_view toSearch);
};

} // namespace parserUtils
} // namespace gero
