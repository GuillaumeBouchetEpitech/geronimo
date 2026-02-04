
#pragma once

#include "geronimo/helpers/GLMath.hpp"

// #include <optional>

namespace gero {
namespace math {

bool collisionRectRect(const glm::vec2& posA, const glm::vec2& sizeA, const glm::vec2& posB, const glm::vec2& sizeB);
bool collisionLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
float rawCollisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
bool collisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P);
bool collisionPointIsLeftOfLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

bool collisionPointCircle(const glm::vec2& P, const glm::vec2& C, float radius);
bool collisionLineCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float radius);
// std::pair<float, float> rawCollisionSegmentCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float
// radius);
bool collisionSegmentCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float radius);

} // namespace math
} // namespace gero
