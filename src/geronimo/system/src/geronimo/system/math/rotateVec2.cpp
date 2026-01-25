
#include "./rotateVec2.hpp"

namespace gero {
namespace math {

glm::vec2	rotateVec2(const glm::vec2& point, const glm::vec2& center, float angle)
{
	const glm::vec2 diff = point - center;
	const float cosVal = std::cos(angle);
	const float sinVal = std::sin(angle);

	return {
    center.x + diff.x * cosVal + diff.y * sinVal,
    center.y + diff.x * sinVal - diff.y * cosVal
  };
}

} // namespace math
} // namespace gero
