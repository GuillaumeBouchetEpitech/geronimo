
#pragma once

namespace gero {
namespace math {

template <typename T> T sign(T val) { return (T(0) < val) ? -T(1) : T(1); }

} // namespace math
} // namespace gero
