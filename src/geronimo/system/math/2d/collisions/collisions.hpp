
#pragma once

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace math {


bool collisionLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
float rawCollisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
bool collisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
bool pointIsLeftOfLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);


}
}
