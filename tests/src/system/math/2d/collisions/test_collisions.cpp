
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/2d/collisions/collisions.hpp"

#include "gtest/gtest.h"

TEST(system_collisions, collisionLineSegment) {

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionLineSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(+10,0);
    glm::vec2 B = glm::vec2(-10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionLineSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(+10,0);
    glm::vec2 B = glm::vec2(-10,0);

    glm::vec2 O = glm::vec2(0,+10);
    glm::vec2 P = glm::vec2(0,-10);

    ASSERT_EQ(gero::math::collisionLineSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 O = glm::vec2(0,+10);
    glm::vec2 P = glm::vec2(0,-10);

    ASSERT_EQ(gero::math::collisionLineSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(-10,-20);
    glm::vec2 B = glm::vec2(+10,-20);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionLineSegment(A,B, O,P), false);
  }

}

TEST(system_collisions, rawCollisionSegmentSegment) {

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    const float expected = 0.5f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A,B, O,P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-5,0);
    glm::vec2 B = glm::vec2(+15,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    const float expected = 0.25f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A,B, O,P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-15,0);
    glm::vec2 B = glm::vec2(+5,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    const float expected = 0.75f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A,B, O,P), expected, 0.001f);
  }

  {
    glm::vec2 A = glm::vec2(-20,0);
    glm::vec2 B = glm::vec2(-10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    const float expected = 2.0f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A,B, O,P), expected, 0.001f);
  }


  {
    glm::vec2 A = glm::vec2(+20,0);
    glm::vec2 B = glm::vec2(+30,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    const float expected = -2.0f;

    ASSERT_NEAR(gero::math::rawCollisionSegmentSegment(A,B, O,P), expected, 0.001f);
  }

}



TEST(system_collisions, collisionSegmentSegment) {

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(-5,0);
    glm::vec2 B = glm::vec2(+15,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(-15,0);
    glm::vec2 B = glm::vec2(+5,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A,B, O,P), true);
  }

  {
    glm::vec2 A = glm::vec2(-20,0);
    glm::vec2 B = glm::vec2(-10,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A,B, O,P), false);
  }


  {
    glm::vec2 A = glm::vec2(+20,0);
    glm::vec2 B = glm::vec2(+30,0);

    glm::vec2 O = glm::vec2(0,-10);
    glm::vec2 P = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::collisionSegmentSegment(A,B, O,P), false);
  }

}


TEST(system_collisions, pointIsLeftOfLine) {

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 C = glm::vec2(0,-10);

    ASSERT_EQ(gero::math::pointIsLeftOfLine(A,B, C), false);
  }

  {
    glm::vec2 A = glm::vec2(-10,0);
    glm::vec2 B = glm::vec2(+10,0);

    glm::vec2 C = glm::vec2(0,+10);

    ASSERT_EQ(gero::math::pointIsLeftOfLine(A,B, C), true);
  }


}



// bool pointIsLeftOfLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

