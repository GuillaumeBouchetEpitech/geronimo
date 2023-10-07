
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

// template <typename T> T lerp(const T& valueA, const T& valueB, float coef) { return valueA + (valueB - valueA) *
// coef; }

float lerp(float valueA, float valueB, float coef);
glm::vec2 lerp(const glm::vec2& valueA, const glm::vec2& valueB, float coef);
glm::vec3 lerp(const glm::vec3& valueA, const glm::vec3& valueB, float coef);
glm::vec4 lerp(const glm::vec4& valueA, const glm::vec4& valueB, float coef);
glm::quat lerp(const glm::quat& valueA, const glm::quat& valueB, float coef);
glm::mat4 lerp(const glm::mat4& valueA, const glm::mat4& valueB, float coef);

} // namespace math
} // namespace gero
