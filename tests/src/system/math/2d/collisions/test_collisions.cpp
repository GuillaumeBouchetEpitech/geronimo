
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/2d/collisions/collisions.hpp"

#include "gtest/gtest.h"

// bool collisionRectRect(const glm::vec2& posA, const glm::vec2& sizeA, const glm::vec2& posB, const glm::vec2& sizeB);

TEST(system_collisions, collisionRectRect) {

  {
    ASSERT_EQ(gero::math::collisionRectRect(glm::vec2(10, 10), glm::vec2(20, 10), glm::vec2(15, 15), glm::vec2(20, 10)),
              true);
  }

  {
    // bellow
    ASSERT_EQ(gero::math::collisionRectRect(glm::vec2(10, 10), glm::vec2(20, 10), glm::vec2(5, 5), glm::vec2(6, 3)),
              false);
    // above
    ASSERT_EQ(gero::math::collisionRectRect(glm::vec2(10, 10), glm::vec2(20, 10), glm::vec2(15, 25), glm::vec2(6, 3)),
              false);
    // left
    ASSERT_EQ(gero::math::collisionRectRect(glm::vec2(10, 10), glm::vec2(20, 10), glm::vec2(0, 15), glm::vec2(6, 3)),
              false);
    // right
    ASSERT_EQ(gero::math::collisionRectRect(glm::vec2(10, 10), glm::vec2(20, 10), glm::vec2(35, 15), glm::vec2(6, 3)),
              false);
  }
}

TEST(system_collisions, collisionLineSegment) {

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionLineSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(+10, 0);
    glm::vec2 B = glm::vec2(-10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionLineSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(+10, 0);
    glm::vec2 B = glm::vec2(-10, 0);

    glm::vec2 O = glm::vec2(0, +10);
    glm::vec2 P = glm::vec2(0, -10);

    ASSERT_EQ(gero::math::collisionLineSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 O = glm::vec2(0, +10);
    glm::vec2 P = glm::vec2(0, -10);

    ASSERT_EQ(gero::math::collisionLineSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(-10, -20);
    glm::vec2 B = glm::vec2(+10, -20);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionLineSegment(A, B, O, P), false);
  }
}

TEST(system_collisions, rawCollisionSegmentSegment) {

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    const float expected = 0.5f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A, B, O, P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-5, 0);
    glm::vec2 B = glm::vec2(+15, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    const float expected = 0.25f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A, B, O, P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-15, 0);
    glm::vec2 B = glm::vec2(+5, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    const float expected = 0.75f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A, B, O, P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-20, 0);
    glm::vec2 B = glm::vec2(-10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    const float expected = 2.0f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A, B, O, P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(+20, 0);
    glm::vec2 B = glm::vec2(+30, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    const float expected = -2.0f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A, B, O, P), expected, 0.001f);
  }
}

TEST(system_collisions, collisionSegmentSegment) {

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(-5, 0);
    glm::vec2 B = glm::vec2(+15, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(-15, 0);
    glm::vec2 B = glm::vec2(+5, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A, B, O, P), true);
  }

  {
    glm::vec2 A = glm::vec2(-20, 0);
    glm::vec2 B = glm::vec2(-10, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A, B, O, P), false);
  }

  {
    glm::vec2 A = glm::vec2(+20, 0);
    glm::vec2 B = glm::vec2(+30, 0);

    glm::vec2 O = glm::vec2(0, -10);
    glm::vec2 P = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A, B, O, P), false);
  }
}

TEST(system_collisions, collisionPointIsLeftOfLine) {

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 C = glm::vec2(0, -10);

    ASSERT_EQ(gero::math::collisionPointIsLeftOfLine(A, B, C), false);
  }

  {
    glm::vec2 A = glm::vec2(-10, 0);
    glm::vec2 B = glm::vec2(+10, 0);

    glm::vec2 C = glm::vec2(0, +10);

    ASSERT_EQ(gero::math::collisionPointIsLeftOfLine(A, B, C), true);
  }
}

TEST(system_collisions, collisionPointCircle) {

  {
    glm::vec2 P = glm::vec2(-10, -10);
    glm::vec2 C = glm::vec2(10, 10);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionPointCircle(P, C, radius), false);
  }

  {
    glm::vec2 P = glm::vec2(8, 8);
    glm::vec2 C = glm::vec2(10, 10);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionPointCircle(P, C, radius), true);
  }
}

TEST(system_collisions, collisionLineCircle) {

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(15, 20);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionLineCircle(A, B, C, radius), false);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(15, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionLineCircle(A, B, C, radius), true);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(10, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionLineCircle(A, B, C, radius), true);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(12, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionLineCircle(A, B, C, radius), true);
  }
}

TEST(system_collisions, collisionSegmentCircle) {

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(15, 20);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionSegmentCircle(A, B, C, radius), false);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(15, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionSegmentCircle(A, B, C, radius), true);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(10, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionSegmentCircle(A, B, C, radius), true);
  }

  {
    glm::vec2 A = glm::vec2(10, 10);
    glm::vec2 B = glm::vec2(20, 10);
    glm::vec2 C = glm::vec2(12, 12);
    float radius = 5.0f;

    ASSERT_EQ(gero::math::collisionSegmentCircle(A, B, C, radius), true);
  }
}
