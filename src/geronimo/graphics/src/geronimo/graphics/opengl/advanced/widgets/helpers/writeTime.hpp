
#pragma once

#include <sstream>
#include <string>

namespace gero {
namespace graphics {
namespace opengl {

namespace helpers {

void writeTime(std::stringstream& stream, int64_t time, int32_t leftPadValue = 5);

}

} // namespace opengl
} // namespace graphics
} // namespace gero
