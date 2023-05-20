
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

template <typename T> T lerp(const T& valueA, const T& valueB, float coef) { return valueA + (valueB - valueA) * coef; }

glm::quat lerp(const glm::quat& valueA, const glm::quat& valueB, float coef) { return glm::slerp(valueA, valueB, coef); }

} // namespace math
} // namespace gero
