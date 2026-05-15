
#pragma once

#include "geronimo/helpers/GLMath.hpp"

class FreeFlyCameraController {
private:

  bool _isEnabled = true;

  float _horizontalAngle = 0.0f;
  float _verticalAngle = 0.0f;

  glm::vec3 _position = {0, 0, 2};
  glm::vec3 _upAxis = {0, 0, 1};
  glm::vec3 _forwardAxis = {1, 0, 0};

  glm::vec3 _target = {1, 0, 0};

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

public:
  void enable() { this->_isEnabled = true; }
  void disable() { this->_isEnabled = false; }
  bool isEnabled() const { return this->_isEnabled; }

public:
  void setPosition(const glm::vec3& inValue);
  void setForwardAxis(const glm::vec3& inValue);
  void setUpAxis(const glm::vec3& inValue);
  void setTarget(const glm::vec3& inValue);

public:
  const glm::vec3& getPosition() const { return this->_position; }
  const glm::vec3& getForwardAxis() const { return this->_forwardAxis; }
  const glm::vec3& getUpAxis() const { return this->_upAxis; }
  const glm::vec3& getTarget() const { return this->_target; }

  // public:
  //   const glm::vec3& getTarget() const;
};
