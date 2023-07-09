
#include "clamp.hpp"

#include <algorithm>

namespace gero {
namespace math {


int32_t clamp(int32_t inValue, int32_t minValue, int32_t maxValue)
{
  return std::min(std::max(inValue, minValue), maxValue);
}
uint32_t clamp(uint32_t inValue, uint32_t minValue, uint32_t maxValue)
{
  return std::min(std::max(inValue, minValue), maxValue);
}

float clamp(float inValue, float minValue, float maxValue)
{
  return std::min(std::max(inValue, minValue), maxValue);
}

glm::vec2 clamp(const glm::vec2& inValue, const glm::vec2& minValue, const glm::vec2& maxValue)
{
  return glm::min(glm::max(inValue, minValue), maxValue);
}
glm::vec3 clamp(const glm::vec3& inValue, const glm::vec3& minValue, const glm::vec3& maxValue)
{
  return glm::min(glm::max(inValue, minValue), maxValue);
}
glm::vec4 clamp(const glm::vec4& inValue, const glm::vec4& minValue, const glm::vec4& maxValue)
{
  return glm::min(glm::max(inValue, minValue), maxValue);
}


}
}

