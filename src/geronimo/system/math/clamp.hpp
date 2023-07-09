
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {

int32_t clamp(int32_t inValue, int32_t minValue, int32_t maxValue);
uint32_t clamp(uint32_t inValue, uint32_t minValue, uint32_t maxValue);

float clamp(float inValue, float minValue, float maxValue);
glm::vec2 clamp(const glm::vec2& inValue, const glm::vec2& minValue, const glm::vec2& maxValue);
glm::vec3 clamp(const glm::vec3& inValue, const glm::vec3& minValue, const glm::vec3& maxValue);
glm::vec4 clamp(const glm::vec4& inValue, const glm::vec4& minValue, const glm::vec4& maxValue);

} // namespace math
} // namespace gero