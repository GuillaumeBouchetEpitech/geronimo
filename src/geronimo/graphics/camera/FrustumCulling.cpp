
#include "FrustumCulling.hpp"

#include "geronimo/system/asValue.hpp"

namespace gero {
namespace graphics {

void FrustumCulling::calculateFrustum(const glm::mat4& inProj, const glm::mat4& inView) {
  calculateFrustum(inProj * inView);
}

void FrustumCulling::calculateFrustum(const glm::mat4& inComposed) {

  const glm::vec4& row0 = {
    inComposed[0][0],
    inComposed[1][0],
    inComposed[2][0],
    inComposed[3][0],
  };
  const glm::vec4& row1 = {
    inComposed[0][1],
    inComposed[1][1],
    inComposed[2][1],
    inComposed[3][1],
  };
  const glm::vec4& row2 = {
    inComposed[0][2],
    inComposed[1][2],
    inComposed[2][2],
    inComposed[3][2],
  };
  const glm::vec4& row3 = {
    inComposed[0][3],
    inComposed[1][3],
    inComposed[2][3],
    inComposed[3][3],
  };

  //

  _setPlane(FrustumSide::eRight, row3 - row0);
  _setPlane(FrustumSide::eLeft, row3 + row0);
  _setPlane(FrustumSide::eBottom, row3 + row1);
  _setPlane(FrustumSide::eTop, row3 - row1);
  _setPlane(FrustumSide::eBack, row3 - row2);
  _setPlane(FrustumSide::eFront, row3 + row2);
}

bool FrustumCulling::pointInFrustum(const glm::vec3& inCenter) const { return sphereInFrustum(inCenter, 0.0f); }

bool FrustumCulling::sphereInFrustum(const glm::vec3& inCenter, float inRadius) const {
  for (const auto& side : _planes)
    if (side.x * inCenter.x + side.y * inCenter.y + side.z * inCenter.z + side.w <= -inRadius)
      return false;

  return true;
}

bool FrustumCulling::cubeInFrustum(const glm::vec3& inCenter, const glm::vec3& inCubeSize) const {
  const glm::vec3 cubeHSize = inCubeSize * 0.5f;

  const float minX = inCenter.x - cubeHSize.x;
  const float minY = inCenter.y - cubeHSize.y;
  const float minZ = inCenter.z - cubeHSize.z;
  const float maxX = inCenter.x + cubeHSize.x;
  const float maxY = inCenter.y + cubeHSize.y;
  const float maxZ = inCenter.z + cubeHSize.z;

  for (const auto& side : _planes) {

    if (side.x * minX + side.y * minY + side.z * minZ + side.w > 0 ||
        side.x * maxX + side.y * minY + side.z * minZ + side.w > 0 ||
        side.x * minX + side.y * maxY + side.z * minZ + side.w > 0 ||
        side.x * maxX + side.y * maxY + side.z * minZ + side.w > 0 ||
        side.x * minX + side.y * minY + side.z * maxZ + side.w > 0 ||
        side.x * maxX + side.y * minY + side.z * maxZ + side.w > 0 ||
        side.x * minX + side.y * maxY + side.z * maxZ + side.w > 0 ||
        side.x * maxX + side.y * maxY + side.z * maxZ + side.w > 0) {
      continue;
    }

    return false;
  }

  return true;
}

void FrustumCulling::_setPlane(FrustumSide inSide, const glm::vec4& inValue) {
  const float magnitude = glm::length(glm::vec3(inValue));

  if (magnitude == 0.0f) {
    _planes[asValue(inSide)] = inValue;
  } else {
    _planes[asValue(inSide)] = inValue / magnitude;
  }
}

} // namespace graphics
} // namespace gero
