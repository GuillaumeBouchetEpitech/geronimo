
#include "geronimo/system/math/2d/collisions/collisions.hpp"
#include "geronimo/system/TraceLogger.hpp"

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

// std::pair<float, float> rawCollisionSegmentCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float
// radius);

// std::pair<float, float> _rawCollisionSegmentCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float
// radius) {

//   glm::vec2 d = B - A; // ( Direction vector of ray, from start to end )
//   glm::vec2 f = C - radius; // ( Vector from center sphere to ray start )

//   // float a = d.Dot( d ) ;
//   float a = glm::dot(d, d);
//   // float b = 2*f.Dot( d ) ;
//   float b = 2.0f * glm::dot(f, d);
//   // float c = f.Dot( f ) - r*r ;
//   float c = glm::dot(f, f) - radius * radius;

//   float discriminant = b*b-4*a*c;
//   if( discriminant < 0 )
//   {
//     // no intersection
//     return std::make_pair(-1.0f, -1.0f);
//   }
//   else
//   {
//     // ray didn't totally miss sphere,
//     // so there is a solution to
//     // the equation.

//     discriminant = std::sqrt( discriminant );

//     // either solution may be on or off the ray so need to test both
//     // t1 is always the smaller value, because BOTH discriminant and
//     // a are nonnegative.
//     float t1 = (-b - discriminant)/(2*a);
//     float t2 = (-b + discriminant)/(2*a);

//     // 3x HIT cases:
//     //          -o->             --|-->  |            |  --|->
//     // Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit),

//     // 3x MISS cases:
//     //       ->  o                     o ->              | -> |
//     // FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

//     if( t1 >= 0 && t1 <= 1 )
//     {
//       // t1 is the intersection, and it's closer than t2
//       // (since t1 uses -b - discriminant)
//       // Impale, Poke
//       // return true ;
//     }
//     else {
//       t1 = -1.0f;
//     }

//     // here t1 didn't intersect so we are either started
//     // inside the sphere or completely past it
//     if( t2 >= 0 && t2 <= 1 )
//     {
//       // ExitWound
//       // return true ;
//     }
//     else {
//       t2 = -1.0f;
//     }

//     // no intn: FallShort, Past, CompletelyInside
//     return std::make_pair(t1, t2);
//   }

// }

// TEST(system_collisions, rawCollisionSegmentCircle) {

//   {
//     glm::vec2 A = glm::vec2(10, 10);
//     glm::vec2 B = glm::vec2(20, 10);
//     glm::vec2 C = glm::vec2(15, 20);
//     float radius = 5.0f;

//     ASSERT_EQ(_rawCollisionSegmentCircle(A, B, C, radius), std::make_pair(-1.0f, -1.0f));
//   }

//   {
//     glm::vec2 A = glm::vec2(0, 10);
//     glm::vec2 B = glm::vec2(40, 10);
//     glm::vec2 C = glm::vec2(20, 10);
//     float radius = 10.0f;

//     ASSERT_EQ(_rawCollisionSegmentCircle(A, B, C, radius), std::make_pair(-1.0f, 0.0f));
//   }

// }

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
