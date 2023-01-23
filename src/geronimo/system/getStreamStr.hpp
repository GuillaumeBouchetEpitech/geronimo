
#pragma once

#include <sstream>

namespace gero {

std::string getStreamStr(const std::ostream& stream);

} // namespace gero

#define D_SSTR(stream) gero::getStreamStr(std::ostringstream() << stream)
