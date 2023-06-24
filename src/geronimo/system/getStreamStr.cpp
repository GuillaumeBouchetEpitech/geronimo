
#include "getStreamStr.hpp"

#include <sstream>
// #include "string-utils/stream-formatter.hpp"

namespace gero {

std::string getStreamStr(const std::ostream& stream) {

  std::string data = dynamic_cast<const std::ostringstream&>(stream).str();
  return data;

  // StreamFormatter streamFormatter;
  // streamFormatter << dynamic_cast<const std::ostringstream&>(stream);
  // return streamFormatter.get();
}

} // namespace gero
