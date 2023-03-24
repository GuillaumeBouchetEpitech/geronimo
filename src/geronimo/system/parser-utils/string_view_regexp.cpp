
#include "string_view_regexp.hpp"

namespace gero {
namespace parserUtils {
namespace string_view_regexp {

using match = std::match_results<std::string_view::const_iterator>;
using sub_match = std::sub_match<std::string_view::const_iterator>;

std::string_view get_string_view(const sub_match& m) { return std::string_view(m.first, std::size_t(m.length())); }

bool regex_match(std::string_view sv,
                 match& m,
                 const std::regex& e,
                 std::regex_constants::match_flag_type flags /*= std::regex_constants::match_default*/) {
  return std::regex_match(sv.begin(), sv.end(), m, e, flags);
}

bool regex_match(std::string_view sv,
                 const std::regex& e,
                 std::regex_constants::match_flag_type flags /*= std::regex_constants::match_default*/) {
  return std::regex_match(sv.begin(), sv.end(), e, flags);
}

bool regex_search(std::string_view sv, match& m, std::regex& r) {
  return std::regex_search(sv.begin(), sv.end(), m, r);
}

bool regex_search(std::string_view::const_iterator start,
                  std::string_view::const_iterator end,
                  match& m,
                  std::regex& r) {
  return std::regex_search(start, end, m, r);
}

} // namespace string_view_regexp
} // namespace parserUtils
} // namespace gero
