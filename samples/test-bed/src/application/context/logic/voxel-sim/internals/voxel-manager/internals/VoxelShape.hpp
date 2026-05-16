
#pragma once

#include "./TrimeshVertex.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <vector>

struct VoxelShape {
  std::string name;
  uint32_t alias;
  glm::ivec3 extrusionAxis = glm::ivec3(0,0,0);
  glm::uvec3 gridSize = glm::uvec3(1,1,1);

  // TODO: store the +x/-x/+y/-y/+z/-z faces? -> easy discard when modelling? (can rely on the normals)

  std::vector<TrimeshVertex> vertices;

  void debugRender(const glm::vec3& position) const;
};

