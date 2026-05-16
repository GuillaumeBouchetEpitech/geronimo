
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
  float intersect(const glm::vec3& inFrom, const glm::vec3& inTo, gero::math::RayCastResult& outRayCastResult) const;

public:
  void render(const glm::vec3& position) const;

public:
  std::vector<TrimeshVertex> vertices;

};
