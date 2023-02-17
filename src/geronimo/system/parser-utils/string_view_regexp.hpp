
#pragma once

#include <regex>

namespace gero {
namespace parserUtils {
namespace string_view_regexp {

  using match = std::match_results<std::string_view::const_iterator>;
  using sub_match = std::sub_match<std::string_view::const_iterator>;

  std::string_view get_string_view(const sub_match& m);
  bool regex_match(
    std::string_view sv,
    match& m,
    const std::regex& e,
    std::regex_constants::match_flag_type flags = std::regex_constants::match_default);

  bool regex_match(
    std::string_view sv,
    const std::regex& e,
    std::regex_constants::match_flag_type flags = std::regex_constants::match_default);

  bool regex_search(
    std::string_view sv,
    match& m,
    std::regex& r);

  bool regex_search(
    std::string_view::const_iterator start,
    std::string_view::const_iterator end,
    match& m,
    std::regex& r);

}
}
}
