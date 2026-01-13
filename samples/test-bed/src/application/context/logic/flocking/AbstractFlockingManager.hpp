
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <memory>

class AbstractBoid {
public:
  virtual ~AbstractBoid() = default;

public:
  virtual const glm::vec3& getPosition() const = 0;
};

class AbstractFlockingManager {
public:
  virtual ~AbstractFlockingManager() = default;

public:
  virtual void update(float elapsedTime) = 0;
  virtual void render() = 0;

public:
  // virtual const std::vector<AbstractBoid>& allBoids() const = 0;
  virtual const AbstractBoid& getBoid(std::size_t index) const = 0;
  virtual std::size_t getTotalBoids() const = 0;

public:
  static std::unique_ptr<AbstractFlockingManager> create();
};
