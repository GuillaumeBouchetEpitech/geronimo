
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <memory>
#include <vector>

class AbstractBasicScene {
public:
  virtual ~AbstractBasicScene() = default;

public:
  virtual void update(float deltaTimeSec) = 0;
  virtual void render() = 0;

public:
  static std::unique_ptr<AbstractBasicScene> create();
};
