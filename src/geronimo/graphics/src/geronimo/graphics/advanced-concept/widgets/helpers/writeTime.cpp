
#include <iomanip>
#include <sstream>
#include <string>

namespace gero {
namespace graphics {

namespace helpers {
void writeTime(std::stringstream& stream, int64_t time, int32_t leftPadValue /*= 5*/) {

  stream << std::setw(std::max(0, leftPadValue));
  stream << std::fixed;
  stream << std::setprecision(1);

  if (time < 1000) {
    stream << time << "ms";
  } else {
    stream << (float(time) * 0.001f) << "s";
  }
}

} // namespace helpers

} // namespace graphics
} // namespace gero
