
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/constants.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include "gtest/gtest.h"

//
//
//

// #define D_ASSERT_RAD_ANGLE(in_angle, in_dst_angle, epsilon) \
// { \
//   if (tmpVal > in_dst_angle - 0.3f && tmpVal < in_dst_angle + 0.3f) \
//     ASSERT_NEAR(in_angle, in_dst_angle, epsilon); \
//   else \
//     ASSERT_NEAR(in_angle - gero::math::pi2, in_dst_angle, epsilon); \
// }

#define D_ASSERT_RAD_ANGLE(in_angle, in_dst_angle, epsilon)                                                            \
  {                                                                                                                    \
    float tmpDiff = in_angle - gero::math::pi2;                                                                        \
    if (tmpDiff > -0.05f && tmpDiff < 0.05f)                                                                           \
      ASSERT_NEAR(tmpDiff, in_dst_angle, epsilon);                                                                     \
    else                                                                                                               \
      ASSERT_NEAR(in_angle, in_dst_angle, epsilon);                                                                    \
  }

//
//
//

#define D_ASSERT_SANITIZE_ANGLE(in_angle, epsilon)                                                                     \
  {                                                                                                                    \
    for (int ii = -5; ii <= 5; ++ii) {                                                                                 \
      const float step = float(ii) * gero::math::pi2;                                                                  \
      const float tmpVal = gero::math::sanitizeAngle(in_angle + step);                                                 \
      D_ASSERT_RAD_ANGLE(tmpVal, in_angle, epsilon);                                                                   \
    }                                                                                                                  \
  }

TEST(angles, sanitizeAngle__0_degrees) { D_ASSERT_SANITIZE_ANGLE(0.0f, 0.01f); }

TEST(angles, sanitizeAngle__45_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::qpi, 0.01f); }

TEST(angles, sanitizeAngle__90_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::hpi, 0.01f); }

TEST(angles, sanitizeAngle__135_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::hpi + gero::math::qpi, 0.01f); }

TEST(angles, sanitizeAngle__180_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::pi, 0.01f); }

TEST(angles, sanitizeAngle__225_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::pi + gero::math::qpi, 0.01f); }

TEST(angles, sanitizeAngle__270_degrees) { D_ASSERT_SANITIZE_ANGLE(gero::math::pi + gero::math::hpi, 0.01f); }

TEST(angles, sanitizeAngle__315_degrees) {
  D_ASSERT_SANITIZE_ANGLE(gero::math::pi + gero::math::hpi + gero::math::qpi, 0.01f);
}

#undef D_ASSERT_SANITIZE_ANGLE

//
//
//

TEST(angles, get_angle__0_degrees) { ASSERT_NEAR(gero::math::getAngle(glm::vec2(1, 0)), 0.0f, 0.01f); }

TEST(angles, get_angle__45_degrees) {
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(1, -1)), -gero::math::qpi, 0.01f);
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(1, +1)), gero::math::qpi, 0.01f);
}

TEST(angles, get_angle__90_degrees) {
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(0, -1)), -gero::math::hpi, 0.01f);
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(0, +1)), gero::math::hpi, 0.01f);
}

TEST(angles, get_angle__135_degrees) {
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(-1, -1)), -gero::math::hpi - gero::math::qpi, 0.01f);
  ASSERT_NEAR(gero::math::getAngle(glm::vec2(-1, +1)), gero::math::hpi + gero::math::qpi, 0.01f);
}

TEST(angles, get_angle__180_degrees) { ASSERT_NEAR(gero::math::getAngle(glm::vec2(-1, 0)), gero::math::pi, 0.01f); }

//
//
//

#define D_ASSERT_VEC2(in_vec2, in_x, in_y, epsilon)                                                                    \
  {                                                                                                                    \
    ASSERT_NEAR(in_vec2.x, in_x, epsilon);                                                                             \
    ASSERT_NEAR(in_vec2.y, in_y, epsilon);                                                                             \
  }

#define D_ASSERT_DIR_VEC2(in_angle, in_x, in_y, epsilon)                                                               \
  {                                                                                                                    \
    for (int ii = -5; ii <= 5; ++ii) {                                                                                 \
      const float step = float(ii) * gero::math::pi2;                                                                  \
      D_ASSERT_VEC2(gero::math::getDirection(in_angle + step), in_x, in_y, epsilon);                                   \
    }                                                                                                                  \
  }

TEST(angles, get_direction__0_degrees) { D_ASSERT_DIR_VEC2(0, 1.0f, 0.0f, 0.01f); }

TEST(angles, get_direction__45_degrees) {
  D_ASSERT_DIR_VEC2(+gero::math::qpi, 0.7f, +0.7f, 0.01f);
  D_ASSERT_DIR_VEC2(-gero::math::qpi, 0.7f, -0.7f, 0.01f);
}

TEST(angles, get_direction__90_degrees) {
  D_ASSERT_DIR_VEC2(+gero::math::hpi, 0.0f, +1.0f, 0.01f);
  D_ASSERT_DIR_VEC2(-gero::math::hpi, 0.0f, -1.0f, 0.01f);
}

TEST(angles, get_direction__135_degrees) {
  D_ASSERT_DIR_VEC2(+gero::math::hpi + gero::math::qpi, -0.7f, +0.7f, 0.01f);
  D_ASSERT_DIR_VEC2(-gero::math::hpi - gero::math::qpi, -0.7f, -0.7f, 0.01f);
}

TEST(angles, get_direction__180_degrees) { D_ASSERT_DIR_VEC2(gero::math::pi, -1.0f, +0.0f, 0.01f); }

#undef D_ASSERT_DIR_VEC2
#undef D_ASSERT_VEC2

//
//
//

#define D_ASSERT_DELTA_ANGLE(in_curr_angle, in_desired_angle, in_dst_angle, epsilon)                                   \
  {                                                                                                                    \
    for (int ii = -5; ii <= 5; ++ii) {                                                                                 \
      const float step = float(ii) * gero::math::pi2;                                                                  \
      ASSERT_NEAR(gero::math::getDeltaAngleFromAngles(step + in_curr_angle, in_desired_angle), in_dst_angle, epsilon); \
      ASSERT_NEAR(gero::math::getDeltaAngleFromAngles(in_curr_angle, step + in_desired_angle), in_dst_angle, epsilon); \
      ASSERT_NEAR(                                                                                                     \
        gero::math::getDeltaAngleFromAngles(step + in_curr_angle, step + in_desired_angle), in_dst_angle, epsilon);    \
    }                                                                                                                  \
  }

TEST(angles, delta_angle__same_angles) { D_ASSERT_DELTA_ANGLE(gero::math::hpi, gero::math::hpi, 0.0f, 0.01f); }

TEST(angles, delta_angle__90degrees_angles) {
  D_ASSERT_DELTA_ANGLE(gero::math::hpi, gero::math::pi, +gero::math::hpi, 0.01f);
  D_ASSERT_DELTA_ANGLE(gero::math::pi, gero::math::hpi, -gero::math::hpi, 0.01f);
}

TEST(angles, delta_angle__45degrees_angles_on_the_limits) {
  D_ASSERT_DELTA_ANGLE(0, +gero::math::qpi, +gero::math::qpi, 0.01f);
  D_ASSERT_DELTA_ANGLE(0, -gero::math::qpi, -gero::math::qpi, 0.01f);
}

TEST(angles, delta_angle__90degrees_angles_across_the_limits) {
  D_ASSERT_DELTA_ANGLE(gero::math::qpi, gero::math::pi2 - gero::math::qpi, -gero::math::hpi, 0.01f);
  D_ASSERT_DELTA_ANGLE(gero::math::pi2 - gero::math::qpi, gero::math::qpi, gero::math::hpi, 0.01f);
}

#undef D_ASSERT_DELTA_ANGLE

//
//
//

TEST(angles, desired_angle__same_angles__no_rotation_speed) {

  constexpr float in_speed_rotation = 0.0f;
  constexpr float in_curr_angle = gero::math::hpi;
  constexpr float in_desired_angle = gero::math::hpi;
  constexpr float in_dst_angle = gero::math::hpi;
  constexpr float epsilon = 0.1f;

  for (int ii = -5; ii < 5; ++ii) {
    const float step = float(ii) * gero::math::pi2;

    std::array<float, 2> currAngles = {in_curr_angle, in_curr_angle + step};
    std::array<float, 2> currDesired = {in_desired_angle, in_desired_angle + step};

    for (std::size_t jj = 0; jj < currAngles.size(); ++jj) {
      for (std::size_t kk = 0; kk < currDesired.size(); ++kk) {
        const float curr = currAngles.at(jj);
        const float desired = currDesired.at(kk);

        const float delta = gero::math::getDeltaAngleFromAngles(curr, desired);
        const float current = gero::math::getDesiredAngleFromDelta(curr, delta, in_speed_rotation);
        const float val = gero::math::getDesiredAngleFromAngle(curr, desired, in_speed_rotation);
        ASSERT_NEAR(delta, 0.0f, epsilon);
        ASSERT_NEAR(current, in_dst_angle, epsilon);
        ASSERT_NEAR(val, in_dst_angle, epsilon);
      }
    }
  }
}

TEST(angles, desired_angle__90degree_angles__90degree_rotation_speed) {

  constexpr float in_speed_rotation = gero::math::hpi;
  constexpr float in_curr_angle = gero::math::pi;
  constexpr float in_desired_angle = gero::math::hpi;
  constexpr float in_dst_angle = gero::math::hpi;
  constexpr float epsilon = 0.1f;

  for (int ii = -5; ii < 5; ++ii) {
    const float step = float(ii) * gero::math::pi2;

    std::array<float, 2> currAngles = {in_curr_angle, in_curr_angle + step};
    std::array<float, 2> currDesired = {in_desired_angle, in_desired_angle + step};

    for (std::size_t jj = 0; jj < currAngles.size(); ++jj) {
      for (std::size_t kk = 0; kk < currDesired.size(); ++kk) {
        const float curr = currAngles.at(jj);
        const float desired = currDesired.at(kk);

        const float delta = gero::math::getDeltaAngleFromAngles(curr, desired);
        const float current = gero::math::getDesiredAngleFromDelta(curr, delta, in_speed_rotation);
        const float val = gero::math::getDesiredAngleFromAngle(curr, desired, in_speed_rotation);
        ASSERT_NEAR(delta, -gero::math::hpi, epsilon);
        ASSERT_NEAR(current, in_dst_angle, epsilon);
        ASSERT_NEAR(val, in_dst_angle, epsilon);
      }
    }
  }
}

TEST(angles, desired_angle__90degree_angles__90degree_rotation_speed__across_the_limits____pos_to_neg) {

  constexpr float in_speed_rotation = gero::math::hpi;
  constexpr float in_curr_angle = gero::math::qpi;
  constexpr float in_desired_angle = -gero::math::qpi;
  constexpr float in_dst_angle = -gero::math::qpi;
  constexpr float epsilon = 0.1f;

  for (int ii = -5; ii < 5; ++ii) {
    const float step = float(ii) * gero::math::pi2;

    std::array<float, 2> currAngles = {in_curr_angle, in_curr_angle + step};
    std::array<float, 2> currDesired = {in_desired_angle, in_desired_angle + step};

    for (std::size_t jj = 0; jj < currAngles.size(); ++jj) {
      for (std::size_t kk = 0; kk < currDesired.size(); ++kk) {
        const float curr = currAngles.at(jj);
        const float desired = currDesired.at(kk);

        const float delta = gero::math::getDeltaAngleFromAngles(curr, desired);
        const float current = gero::math::getDesiredAngleFromDelta(curr, delta, in_speed_rotation);
        const float val = gero::math::getDesiredAngleFromAngle(curr, desired, in_speed_rotation);
        ASSERT_NEAR(delta, -gero::math::hpi, epsilon);
        ASSERT_NEAR(current, in_dst_angle, epsilon);
        ASSERT_NEAR(val, in_dst_angle, epsilon);
      }
    }
  }
}

TEST(angles, desired_angle__90degree_angles__90degree_rotation_speed__across_the_limits____neg_to_pos) {

  constexpr float in_speed_rotation = gero::math::hpi;
  constexpr float in_curr_angle = -gero::math::qpi;
  constexpr float in_desired_angle = +gero::math::qpi;
  constexpr float in_dst_angle = +gero::math::qpi;
  constexpr float epsilon = 0.1f;

  for (int ii = -5; ii < 5; ++ii) {
    const float step = float(ii) * gero::math::pi2;

    std::array<float, 2> currAngles = {in_curr_angle, in_curr_angle + step};
    std::array<float, 2> currDesired = {in_desired_angle, in_desired_angle + step};

    for (std::size_t jj = 0; jj < currAngles.size(); ++jj) {
      for (std::size_t kk = 0; kk < currDesired.size(); ++kk) {
        const float curr = currAngles.at(jj);
        const float desired = currDesired.at(kk);

        const float delta = gero::math::getDeltaAngleFromAngles(curr, desired);
        const float current = gero::math::getDesiredAngleFromDelta(curr, delta, in_speed_rotation);
        const float val = gero::math::getDesiredAngleFromAngle(curr, desired, in_speed_rotation);
        ASSERT_NEAR(delta, gero::math::hpi, epsilon);
        ASSERT_NEAR(current, in_dst_angle, epsilon);
        ASSERT_NEAR(val, in_dst_angle, epsilon);
      }
    }
  }
}

// TEST(angles, get_clamped_angle__0degree_angles__min_is_neg45degree__max_is_pos45degree) {

//   {
//     const float startAngle = 0.0f;

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + 0.0f, -gero::math::qpi, gero::math::qpi), 0.0f, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::opi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::opi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::opi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::opi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::qpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::qpi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::hpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::hpi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::pi * 0.99f, -gero::math::qpi,
//     +gero::math::qpi), -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::pi
//     * 0.99f, -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);

//     // ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::pi * 1.5f, -gero::math::qpi,
//     +gero::math::qpi), -gero::math::qpi, 0.01f);
//     // ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::pi * 1.5f, -gero::math::qpi,
//     +gero::math::qpi), -gero::math::qpi, 0.01f);
//   }

//   //
//   //
//   //

//   {
//     const float startAngle = -gero::math::opi;

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + 0.0f, -gero::math::qpi, gero::math::qpi), startAngle + 0.0f,
//     0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::opi, -gero::math::qpi, +gero::math::qpi),
//     startAngle + -gero::math::opi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::opi,
//     -gero::math::qpi, +gero::math::qpi), startAngle + +gero::math::opi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::qpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::qpi,
//     -gero::math::qpi, +gero::math::qpi), startAngle + +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::hpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::hpi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::pi * 0.99f, -gero::math::qpi,
//     +gero::math::qpi), +gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::pi
//     * 0.99f, -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);
//   }

//   {
//     const float startAngle = -gero::math::hpi;

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + 0.0f, -gero::math::qpi, gero::math::qpi), -gero::math::qpi,
//     0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::opi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::opi,
//     -gero::math::qpi, +gero::math::qpi), -gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::qpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::qpi,
//     -gero::math::qpi, +gero::math::qpi), startAngle + +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::hpi * 0.99f, -gero::math::qpi,
//     +gero::math::qpi), -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle +
//     +gero::math::hpi * 1.00f, -gero::math::qpi, +gero::math::qpi), 0.0f, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::pi * 0.99f, -gero::math::qpi,
//     +gero::math::qpi), -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::pi
//     * 0.99f, -gero::math::qpi, +gero::math::qpi), 0.0f, 0.01f);
//   }

//   //
//   //
//   //

//   {
//     const float startAngle = +gero::math::opi;

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + 0.0f, -gero::math::qpi, gero::math::qpi), startAngle + 0.0f,
//     0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::opi, -gero::math::qpi, +gero::math::qpi),
//     startAngle + -gero::math::opi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::opi,
//     -gero::math::qpi, +gero::math::qpi), startAngle + +gero::math::opi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::qpi, -gero::math::qpi, +gero::math::qpi),
//     startAngle + -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::qpi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::hpi, -gero::math::qpi, +gero::math::qpi),
//     -gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::hpi,
//     -gero::math::qpi, +gero::math::qpi), +gero::math::qpi, 0.01f);
//   }

//   {
//     const float startAngle = +gero::math::hpi;

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + 0.0f, -gero::math::qpi, gero::math::qpi), gero::math::qpi,
//     0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::opi, -gero::math::qpi, +gero::math::qpi),
//     gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::opi, -gero::math::qpi,
//     +gero::math::qpi), gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::qpi, -gero::math::qpi, +gero::math::qpi),
//     gero::math::qpi, 0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::qpi, -gero::math::qpi,
//     +gero::math::qpi), gero::math::qpi, 0.01f);

//     ASSERT_NEAR(gero::math::getClampedAngle(startAngle + -gero::math::hpi, -gero::math::qpi, +gero::math::qpi), 0.0f,
//     0.01f); ASSERT_NEAR(gero::math::getClampedAngle(startAngle + +gero::math::hpi * 0.75f, -gero::math::qpi,
//     +gero::math::qpi), +gero::math::qpi, 0.01f);
//   }

//   //
//   //
//   //

// }

// TEST(angles, get_min_angle) {

//   ASSERT_NEAR(gero::math::getMinAngle(+gero::math::pi2, -gero::math::qpi), +gero::math::pi2, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(+gero::math::pi, -gero::math::qpi), +gero::math::pi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(+gero::math::hpi, -gero::math::qpi), +gero::math::hpi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(+gero::math::qpi, -gero::math::qpi), +gero::math::qpi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(0.0f, -gero::math::qpi), 0.0f, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(-gero::math::qpi, -gero::math::qpi), -gero::math::qpi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(-gero::math::hpi, -gero::math::qpi), -gero::math::qpi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(-gero::math::pi, -gero::math::qpi), -gero::math::qpi, 0.01f);
//   ASSERT_NEAR(gero::math::getMinAngle(-gero::math::pi2, -gero::math::qpi), -gero::math::qpi, 0.01f);
// }

//
//
//

#undef D_ASSERT_RAD_ANGLE

//
//
//
