
#pragma once

#include "./TrimeshVertex.hpp"
#include "./VoxelModelMatrix.hpp"

#include "geronimo/system/math/RayCastResult.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <vector>

// forward declaration
class VoxelManager;

struct VoxelModelGeometry
{
public:
  void build(const VoxelManager& inVoxelManager, const VoxelModelMatrix& inMatrix);

public:
  bool intersect(const glm::vec3& inFrom, const glm::vec3& inTo, gero::math::RayCastResult& outRayCastResult) const;

public:
  void debugRender(const glm::vec3& position) const;

public:
  std::vector<TrimeshVertex> vertices;

};
