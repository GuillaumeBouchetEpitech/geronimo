
#pragma once

#include "../AbstractFlockingManager.hpp"

#include "geronimo/graphics/camera/Camera.hpp"

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <array>
#include <memory>
#include <vector>

constexpr std::size_t k_trailSize = 15;
// constexpr std::size_t k_trailSize = 5;

struct Boid : public AbstractBoid {
  glm::vec3 position = {0, 0, 0};
  glm::vec3 velocity = {0, 0, 0};
  glm::vec3 acceleration = {0, 0, 0};
  glm::vec3 color = {1, 1, 1};

  glm::vec3 aabbMin{0.0f};
  glm::vec3 aabbMax{0.0f};

  float horizontalAngle = 0.0f;
  float verticalAngle = 0.0f;

  struct TrailData {
    glm::vec3 position;
  };
  std::array<TrailData, k_trailSize> trail;

  struct GraphicData {
    struct TrailData {
      glm::vec3 center;
      glm::quat orientation;
      float length;
    };

    std::vector<TrailData> trailData;
  } graphicData;

  Boid();

  void seek(const glm::vec3& target, float coef);
  void flee(const glm::vec3& target, float coef);
  void separate(const std::vector<Boid*>& boids, float coef);
  void strafe(const glm::vec3& target, float horizontalAngle, float verticalAngle, float coef);
  void wander(float coef);

  void computeAabb();

  void updateTrail();

  void applyAcceleration(float maxAcceleration, float maxVelocity, float elapsedTime);

  bool operator==(const Boid& other) const;

  const glm::vec3& getPosition() const override;

};
