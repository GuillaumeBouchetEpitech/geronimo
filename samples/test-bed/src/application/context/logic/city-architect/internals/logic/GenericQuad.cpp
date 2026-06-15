
#include "GenericQuad.hpp"

#include "geronimo/system/asValue.hpp"
#include "geronimo/system/math/lerp.hpp"


GenericQuad GenericQuad::makeFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize)
{
  GenericQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = inOrigin + glm::vec3(+inSize.x * 0.0f, +inSize.y * 0.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = inOrigin + glm::vec3(+inSize.x * 1.0f, +inSize.y * 0.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = inOrigin + glm::vec3(+inSize.x * 0.0f, +inSize.y * 1.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = inOrigin + glm::vec3(+inSize.x * 1.0f, +inSize.y * 1.0f, 0);
  return newFloorQuad;
}

GenericQuad GenericQuad::makeFloorConnection(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
  const std::array<glm::vec3, 4> tmpVertices = {{ v0, v1, v2, v3 }};

  auto _findVertex = [&tmpVertices](bool inDirX, bool inDirY) -> const glm::vec3& {

    uint32_t bestIndex = 0;
    for (uint32_t ii = 1; ii < uint32_t(tmpVertices.size()); ++ii)
    {
      const glm::vec3& best = tmpVertices.at(bestIndex);
      const glm::vec3& current = tmpVertices.at(ii);

      if (inDirX && best.x > current.x) {
        continue;
      }
      if (!inDirX && best.x < current.x) {
        continue;
      }
      if (inDirY && best.y > current.y) {
        continue;
      }
      if (!inDirY && best.y < current.y) {
        continue;
      }

      bestIndex = ii;
    }

    return tmpVertices.at(bestIndex);
  };

  GenericQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = _findVertex(true, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = _findVertex(false, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = _findVertex(true, true);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = _findVertex(false, true);

  return newFloorQuad;
}


float GenericQuad::getFloorZ(float inX, float inY) const
{
  const glm::vec3& posX_posY_Pos = this->getFloorVertex(GenericQuad::FloorVertexType::posX_posY);
  const glm::vec3& posX_negY_Pos = this->getFloorVertex(GenericQuad::FloorVertexType::posX_negY);
  const glm::vec3& negX_posX_Pos = this->getFloorVertex(GenericQuad::FloorVertexType::negX_posY);
  const glm::vec3& negX_negY_Pos = this->getFloorVertex(GenericQuad::FloorVertexType::negX_negY);

  const glm::vec2& k_origin = negX_negY_Pos;
  const glm::vec2 k_size = posX_posY_Pos - negX_negY_Pos;

  // get Z values for LEFT and RIGHT
  const float ratioY = (inY - k_origin.y) / k_size.y; // [0..1]
  const float zPosY = gero::math::lerp(posX_negY_Pos.z, posX_posY_Pos.z, ratioY);
  const float zNegY = gero::math::lerp(negX_negY_Pos.z, negX_posX_Pos.z, ratioY);

  // get Z value for CENTER
  const float ratioX = (inX - k_origin.x) / k_size.x; // [0..1]
  return gero::math::lerp(zNegY, zPosY, ratioX);
}

bool GenericQuad::collide(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  const glm::vec3& maxCoord = this->getFloorVertex(GenericQuad::FloorVertexType::posX_posY);
  const glm::vec3& minCoord = this->getFloorVertex(GenericQuad::FloorVertexType::negX_negY);

  if (
    // too far backward
    inCenter.x + inSize.x < minCoord.x ||
    // too far right
    inCenter.y + inSize.y < minCoord.y ||
    // too far forward
    inCenter.x - inSize.x > maxCoord.x ||
    // too far left
    inCenter.y - inSize.y > maxCoord.y
  ) {
    // out of bounds (X/Y)
    return false;
  }

  const float floorZ = this->getFloorZ(inCenter.x, inCenter.y);
  if (
    // too low
    inCenter.z + inSize.z < floorZ ||
    // too high
    inCenter.z - inSize.z > floorZ
  ) {
    // out of bounds (Z)
    return false;
  }

  return true;
}

bool GenericQuad::collide(const glm::vec3& inCenter, float inRadius) const
{
  return this->collide(inCenter, glm::vec3(inRadius, inRadius, inRadius));
}
