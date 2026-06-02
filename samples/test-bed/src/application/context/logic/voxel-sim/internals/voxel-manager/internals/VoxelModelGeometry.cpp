
#include "VoxelModelGeometry.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/math/rayIntersectTriangle.hpp"

// MARK: intersect
bool VoxelModelGeometry::intersect(
  const glm::vec3& inFrom,
  const glm::vec3& inTo,
  gero::math::RayCastResult& outRayCastResult
) const {

  const GenericAABB rayAabb = GenericAABB(glm::min(inFrom, inTo), glm::max(inFrom, inTo));

  if (rayAabb.overlap(this->aabb) == false) {
    return false; // no ray/geometry aabb overlap -> skip
  }

  GenericAABB triangleAabb = this->aabb;

  const glm::vec3 rayDir = inTo - inFrom;

  gero::math::RayCastResult tmpRayCastResult;
  gero::math::RayCastResult stepRayCastResult;

  for (std::size_t ii = 0; ii < vertices.size(); ii += 3) {
    const glm::vec3& v0 = vertices.at(ii + 0).pos;
    const glm::vec3& v1 = vertices.at(ii + 1).pos;
    const glm::vec3& v2 = vertices.at(ii + 2).pos;

    if (rayAabb.overlap(triangleAabb) == false) {
      continue; // no ray/geometry aabb overlap -> skip
    }

    const bool hasHit = gero::math::intersectTriangle(inFrom, rayDir, v0, v1, v2, stepRayCastResult);
    if (hasHit && (tmpRayCastResult.distance < 0.0f || tmpRayCastResult.distance > stepRayCastResult.distance)) {
      tmpRayCastResult = stepRayCastResult;

      // shrink the aabb -> this might squeeze some extra performances
      const glm::vec3 impactPos = inFrom + rayDir * stepRayCastResult.distance;
      triangleAabb.set(
        glm::min(inFrom, impactPos),
        glm::max(inFrom, impactPos)
      );
    }

  }

  // gero::math::intersectTriangle();

  outRayCastResult = tmpRayCastResult;
  return tmpRayCastResult.distance >= 0.0f;
}

// MARK: render
void VoxelModelGeometry::debugRender(const glm::vec3& inPosition) const
{
  auto& context = Context::get();
  auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  for (std::size_t jj = 0; jj < vertices.size(); jj += 3) {
    const glm::vec3 v0 = vertices.at(jj + 0).pos;
    const glm::vec3 v1 = vertices.at(jj + 1).pos;
    const glm::vec3 v2 = vertices.at(jj + 2).pos;
    const glm::vec3 rawNorm = vertices.at(jj + 0).norm;
    const glm::vec3 tmpNorm = rawNorm * 0.3f;

    const glm::vec3 center = (v0 + v1 + v2) / 3.0f;

    glm::vec4 tmpColor = glm::vec4(0.5f,0.0f,0.5f, 1);
    const float dotX = std::abs(glm::dot(rawNorm, glm::vec3(1,0,0)));
    const float dotY = std::abs(glm::dot(rawNorm, glm::vec3(0,1,0)));
    const float dotZ = std::abs(glm::dot(rawNorm, glm::vec3(0,0,1)));
    // D_MYERR("dotX " << dotX << ", dotY " << dotY << ", dotZ " << dotZ);
    // glm::greaterThan(glm::vec3(1,1,1), glm::vec3(1,1,1));
    uint32_t tmpTotal = 0;
    if (dotX > 0.5f) { tmpTotal += 1; }
    if (dotY > 0.5f) { tmpTotal += 1; }
    if (dotZ > 0.5f) { tmpTotal += 1; }
    if (tmpTotal >= 2) {
      // angular -> yellow
      tmpColor = glm::vec4(0.5f,0.5f,0.0f, 1);
    }
    else if (dotX > 0.95f) {
      // X axis aligned -> red
      tmpColor = glm::vec4(0.5f,0.0f,0.0f, 1);
    }
    else if (dotY > 0.95f) {
      // Y axis aligned -> green
      tmpColor = glm::vec4(0.0f,0.5f,0.0f, 1);
    }
    else if (dotZ > 0.95f) {
      // Z axis aligned -> blue
      tmpColor = glm::vec4(0.2f,0.2f,0.5f, 1);
    }

    wireFramesStack.pushLine(inPosition + v0, inPosition + v1, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v1, inPosition + v2, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v2, inPosition + v0, glm::vec3(1,1,0));
    // wireFramesStack.pushLine(inPosition + v0, inPosition + v1, tmpColor * 2.0f);
    // wireFramesStack.pushLine(inPosition + v1, inPosition + v2, tmpColor * 2.0f);
    // wireFramesStack.pushLine(inPosition + v2, inPosition + v0, tmpColor * 2.0f);

    // normal
    wireFramesStack.pushLine(inPosition + center, inPosition + center + tmpNorm, glm::vec3(1,1,1));

    // triangle
    trianglesStack.pushTriangle(inPosition + v0, inPosition + v1, inPosition + v2, tmpColor);
  }

}
