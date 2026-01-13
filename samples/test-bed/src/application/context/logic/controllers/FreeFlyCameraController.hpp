
#pragma once

#include "geronimo/helpers/GLMath.hpp"

class FreeFlyCameraController {
private:
  float _horizontalAngle = 0.0f;
  float _verticalAngle = 0.0f;

  glm::vec3 _position = {0, 0, 2};
  glm::vec3 _upAxis = {0, 0, 1};
  glm::vec3 _forwardAxis = {1, 0, 0};

  glm::vec3 _forwardMovement = {1, 0, 0};
  glm::vec3 _leftMovement = {0, 1, 0};

  // glm::vec3 _target = { 0, 0, 0 };

  // float _rayCastCooldown = 0.0f;

  float _doubleTapTimeLeft = 0.0f;
  bool _forceForward = false;

public:
  FreeFlyCameraController() = default;
  // ~FreeFlyCameraController();

public:
  void update(float elapsed_time);

  void setPosition(const glm::vec3& pos);

  // public:
  //   const glm::vec3& getTarget() const;
};
