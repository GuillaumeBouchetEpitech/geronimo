
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

// forward declaration
class VoxelManager;

struct VoxelModelMatrix {
  glm::uvec3 gridSize = glm::uvec3(1,1,1);
  std::vector<uint32_t> values;

  uint32_t getValue(int32_t x, int32_t y, int32_t z) const;
  uint32_t getValue(const glm::ivec3& cursor) const;

  void render(const VoxelManager& inVoxelManager) const;
};
