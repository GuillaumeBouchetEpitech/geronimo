
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

// forward declaration
class VoxelManager;

struct VoxelModelMatrixCell {
  uint16_t colorAlias = 0U;
  uint16_t shapeAlias = 0U;
};

struct VoxelModelMatrix {
  glm::uvec3 gridSize = glm::uvec3(1,1,1);
  std::vector<VoxelModelMatrixCell> values;

  VoxelModelMatrixCell getValue(int32_t x, int32_t y, int32_t z) const;
  VoxelModelMatrixCell getValue(const glm::ivec3& cursor) const;

  void debugRender(const VoxelManager& inVoxelManager) const;
};
