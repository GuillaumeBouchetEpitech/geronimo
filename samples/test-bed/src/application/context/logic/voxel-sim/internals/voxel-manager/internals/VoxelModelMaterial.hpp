
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <string>

struct VoxelModelMaterial
{
  std::string name;
  uint32_t alias;
  glm::vec3 color;
};
