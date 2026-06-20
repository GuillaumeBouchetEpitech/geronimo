
#pragma once

#include "../AbstractBrickModel.hpp"

struct BrickInstance {

  glm::vec3 pos;
  glm::quat quat;

  AbstractBrickModelWeakRef ref;
};

