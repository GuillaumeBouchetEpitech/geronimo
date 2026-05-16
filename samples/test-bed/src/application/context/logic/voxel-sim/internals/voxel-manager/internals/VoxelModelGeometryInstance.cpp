
#include "VoxelModelGeometryInstance.hpp"

#include "application/context/Context.hpp"


bool VoxelModelGeometryInstance::intersect(
  const glm::vec3& inFrom,
  const glm::vec3& inTo,
  gero::math::RayCastResult& outRayCastResult
) const {

  if (!modelGeometry) {
    outRayCastResult.distance = -1.0f;
    return -1.0f;
  }

  // compute inverse mat4
  glm::mat4 tmpTransform = glm::identity<glm::mat4>();
  tmpTransform = glm::translate(tmpTransform, this->position);
  tmpTransform = glm::toMat4(this->orientation) * tmpTransform;
  const glm::mat4 invTransform = glm::inverse(tmpTransform);

  // -> bring the ray from/to value into the "origin space"
  const glm::vec3 invFrom = invTransform * glm::vec4(inFrom, 1.0f);
  const glm::vec3 invTo = invTransform * glm::vec4(inTo, 1.0f);

  // D_MYLOG(" - invFrom -> " << invFrom);
  // D_MYLOG("   invTo ---> " << invTo);

  //

  gero::math::RayCastResult tmpRayCastResult;
  const bool hasHit = modelGeometry->intersect(invFrom, invTo, tmpRayCastResult);

  // D_MYLOG("   tmpRayCastResult.distance -> " << tmpRayCastResult.distance);

  // did not intersect
  if (!hasHit) {
    return outRayCastResult.distance;
  }

  // collision but not an any closer collision
  if (outRayCastResult.distance > 0.0f && tmpRayCastResult.distance > outRayCastResult.distance) {
    return outRayCastResult.distance;
  }

  // D_MYLOG("   tmpRayCastResult.distance -> " << tmpRayCastResult.distance);

  // from this point:
  // -> is a closer collision

  // bring the normal in the "model space"
  tmpRayCastResult.normal = this->orientation * tmpRayCastResult.normal;

  outRayCastResult = tmpRayCastResult;
  return tmpRayCastResult.distance;
}

void VoxelModelGeometryInstance::debugRender() const
{
  if (!modelGeometry) {
    return;
  }

  glm::mat4 tmpTransform = glm::identity<glm::mat4>();
  tmpTransform = glm::translate(tmpTransform, this->position);
  tmpTransform *= glm::toMat4(this->orientation);

  auto& context = Context::get();
  auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  const auto& vertices = modelGeometry->vertices;

  for (std::size_t jj = 0; jj < vertices.size(); jj += 3) {
    const glm::vec3 v0 = this->position + this->orientation * vertices.at(jj + 0).pos;
    const glm::vec3 v1 = this->position + this->orientation * vertices.at(jj + 1).pos;
    const glm::vec3 v2 = this->position + this->orientation * vertices.at(jj + 2).pos;
    const glm::vec3 rawColor = vertices.at(jj + 0).color;
    const glm::vec3 rawNorm = vertices.at(jj + 0).norm;

    const glm::vec3 center = (v0 + v1 + v2) / 3.0f;
    const glm::vec3 tmpNorm = this->orientation * rawNorm * 0.3f;

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

    wireFramesStack.pushLine(v0, v1, glm::vec3(1,1,0));
    wireFramesStack.pushLine(v1, v2, glm::vec3(1,1,0));
    wireFramesStack.pushLine(v2, v0, glm::vec3(1,1,0));

    // wireFramesStack.pushLine(v0, v1, tmpColor * 2.0f);
    // wireFramesStack.pushLine(v1, v2, tmpColor * 2.0f);
    // wireFramesStack.pushLine(v2, v0, tmpColor * 2.0f);

    // normal
    wireFramesStack.pushLine(center, center + tmpNorm, glm::vec3(1,1,1));

    // triangle
    trianglesStack.pushTriangle(v0, v1, v2, tmpColor);
    // trianglesStack.pushTriangle(v0, v1, v2, glm::vec4(rawColor, 1.0f));

  }

}
