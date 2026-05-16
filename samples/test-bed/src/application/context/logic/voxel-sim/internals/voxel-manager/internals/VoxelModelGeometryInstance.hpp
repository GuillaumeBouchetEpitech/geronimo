
#pragma once

#include "./VoxelModelGeometry.hpp"

#include "geronimo/system/math/RayCastResult.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <memory>

struct VoxelModelGeometryInstance
{

public:
  bool intersect(const glm::vec3& inFrom, const glm::vec3& inTo, gero::math::RayCastResult& outRayCastResult) const;

public:
  void debugRender() const;

public:
  std::shared_ptr<VoxelModelGeometry> modelGeometry{nullptr};
  glm::vec3 position = glm::vec3(0,0,0);
  glm::quat orientation = glm::identity<glm::quat>();

};

