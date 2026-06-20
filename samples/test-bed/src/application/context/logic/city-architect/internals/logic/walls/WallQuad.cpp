
#include "WallQuad.hpp"

#include "../floors/FloorQuad.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/asValue.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <unordered_set>
#include <algorithm> // std::sort


WallQuad WallQuad::makeWallFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize, WallOrientation inWallOrientation) {

  if (inWallOrientation == WallOrientation::posX) {
    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 0.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 1.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 0.0f, inSize.y * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 1.0f, inSize.y * 1.0f);
    return newWallQuad;
  }

  if (inWallOrientation == WallOrientation::negX) {
    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 1.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 0.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 1.0f, inSize.y * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = inOrigin + glm::vec3(0.0f, inSize.x * 0.0f, inSize.y * 1.0f);
    return newWallQuad;
  }

  if (inWallOrientation == WallOrientation::posY) {
    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = inOrigin + glm::vec3(inSize.x * 0.0f, 0.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = inOrigin + glm::vec3(inSize.x * 1.0f, 0.0f, inSize.y * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = inOrigin + glm::vec3(inSize.x * 0.0f, 0.0f, inSize.y * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = inOrigin + glm::vec3(inSize.x * 1.0f, 0.0f, inSize.y * 1.0f);
    return newWallQuad;
  }

  WallQuad newWallQuad;
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = inOrigin + glm::vec3(inSize.x * 1.0f, 0.0f, inSize.y * 0.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = inOrigin + glm::vec3(inSize.x * 0.0f, 0.0f, inSize.y * 0.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = inOrigin + glm::vec3(inSize.x * 1.0f, 0.0f, inSize.y * 1.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = inOrigin + glm::vec3(inSize.x * 0.0f, 0.0f, inSize.y * 1.0f);
  return newWallQuad;
}

WallQuad WallQuad::makeWallAdjacentToFloor(
  const FloorQuad& inFloorQuad,
  WallOrientation inWallOrientation,
  float inHeight
) {

  if (inWallOrientation == WallOrientation::posX) {
    const glm::vec3& negY = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_negY);
    const glm::vec3& posY = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY);

    // normal on the inside -> negX

    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = posY + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = negY + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = posY + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = negY + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    return newWallQuad;
  }

  if (inWallOrientation == WallOrientation::negX) {
    const glm::vec3& negY = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::negX_negY);
    const glm::vec3& posY = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::negX_posY);

    // normal on the inside -> posX

    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = negY + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = posY + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = negY + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = posY + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    return newWallQuad;
  }

  if (inWallOrientation == WallOrientation::posY) {
    const glm::vec3& negX = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::negX_posY);
    const glm::vec3& posX = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY);

    // normal on the inside -> negY

    WallQuad newWallQuad;
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = negX + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = posX + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = negX + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = posX + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
    return newWallQuad;
  }

  const glm::vec3& negX = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::negX_negY);
  const glm::vec3& posX = inFloorQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_negY);

  // normal on the inside -> negY

  WallQuad newWallQuad;
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = posX + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = negX + glm::vec3(0.0f, 0.0f, inHeight * 0.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = posX + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = negX + glm::vec3(0.0f, 0.0f, inHeight * 1.0f);
  return newWallQuad;
}

//MARK: divideFromCoords
bool WallQuad::divideFromCoords(const std::vector<glm::vec2>& inCutCoords, std::vector<WallQuad>& outWallQuads) const {
  /**
   * FROM
   *   *----------*
   *   |          |
   *   |  x       |
   *   |          |
   *   |     x    |
   *   |          |
   *   |          |
   *   *----------*
   * TO
   *   *--*--*----*
   *   |  |  |    |
   *   *--*--*----*
   *   |  |  |    |
   *   *--*--*----*
   *   |  |  |    |
   *   |  |  |    |
   *   *--*--*----*
   */

  glm::vec3 this_maxCoord = this->getVertex(WallQuad::VertexType::posAxis_posZ);
  glm::vec3 this_minCoord = this->getVertex(WallQuad::VertexType::negAxis_negZ);

  D_MYLOG("this_maxCoord " << this_maxCoord);
  D_MYLOG("this_minCoord " << this_minCoord);

  const bool isFacingX = glm::epsilonEqual(this_maxCoord.x, this_minCoord.x, 0.1f);
  const std::size_t hAxis = isFacingX ? 1 : 0;
  const std::size_t fAxis = isFacingX ? 0 : 1;

  D_MYLOG(" -> hAxis " << hAxis);

  if (this_minCoord[hAxis] > this_maxCoord[hAxis]) {
    std::swap(this_minCoord[hAxis], this_maxCoord[hAxis]);
    D_MYLOG(" -> this_maxCoord " << this_maxCoord);
    D_MYLOG(" -> this_minCoord " << this_minCoord);
  }

  std::unordered_set<float> allCoordSetAxis;
  std::unordered_set<float> allCoordSetZ;

  for (const glm::vec3& vertex : this->_vertices)
  {
    allCoordSetAxis.insert(vertex[hAxis]);
    allCoordSetZ.insert(vertex.z);
  }

  constexpr float k_rangeEpsilon = 0.1f;

  for (const glm::vec2& currCutCoord : inCutCoords)
  {
    D_MYLOG(" -> currCutCoord " << currCutCoord.x);

    const bool isInRangeAxis = (
      currCutCoord.x > this_minCoord[hAxis] + k_rangeEpsilon &&
      currCutCoord.x < this_maxCoord[hAxis] - k_rangeEpsilon
    );
    const bool isInRangeZ = (
      currCutCoord.y > this_minCoord.z + k_rangeEpsilon &&
      currCutCoord.y < this_maxCoord.z - k_rangeEpsilon
    );

    D_MYLOG(" -> isInRangeAxis " << isInRangeAxis);

    if (isInRangeAxis && isInRangeZ) {
      allCoordSetAxis.insert(currCutCoord.x);
      allCoordSetZ.insert(currCutCoord.y);
    }
    else if (isInRangeAxis) {
      allCoordSetAxis.insert(currCutCoord.x);
    }
    else if (isInRangeZ) {
      allCoordSetZ.insert(currCutCoord.y);
    }
  }

  if (
    allCoordSetAxis.size() == 2 &&
    allCoordSetZ.size() == 2
  ) {
    // nothing to cut -> skip
    D_MYLOG("nothing to cut -> skip");
    return false;
  }

  std::vector<float> allCoordAxis;
  allCoordAxis.reserve(allCoordSetAxis.size());
  for (float val : allCoordSetAxis)
    allCoordAxis.push_back(val);

  std::vector<float> allCoordZ;
  allCoordZ.reserve(allCoordSetZ.size());
  for (float val : allCoordSetZ)
    allCoordZ.push_back(val);

  std::sort(allCoordAxis.begin(), allCoordAxis.end());
  std::sort(allCoordZ.begin(), allCoordZ.end());

  D_MYLOG("allCoordAxis.size() " << allCoordAxis.size());
  D_MYLOG("allCoordZ.size() " << allCoordZ.size());

  const glm::vec3 this_normal = this->getNormal();

  WallOrientation wallOrientation = WallOrientation::posX;
  if (this_normal.x < 0) { wallOrientation = WallOrientation::negX; }
  else if (this_normal.y > 0) { wallOrientation = WallOrientation::posY; }
  else if (this_normal.y < 0) { wallOrientation = WallOrientation::negY; }

  for (std::size_t xx = 0; xx + 1 < allCoordAxis.size(); ++xx)
  for (std::size_t yy = 0; yy + 1 < allCoordZ.size(); ++yy)
  {
    const glm::vec2 minCoord = glm::vec2(allCoordAxis.at(xx + 0), allCoordZ.at(yy + 0));
    const glm::vec2 maxCoord = glm::vec2(allCoordAxis.at(xx + 1), allCoordZ.at(yy + 1));

    glm::vec3 newPos = glm::vec3(0,0,minCoord.y);
    newPos[hAxis] = minCoord.x;
    newPos[fAxis] = this_minCoord[fAxis];

    outWallQuads.push_back(WallQuad::makeWallFromOrigin(newPos, maxCoord - minCoord, wallOrientation));

    const glm::vec3 out_normal = outWallQuads.back().getNormal();

    if (!glm::all(glm::epsilonEqual(this_normal, out_normal, 0.1f))) {

      std::swap(
        outWallQuads.back()._vertices.at(gero::asValue(VertexType::posAxis_posZ))[hAxis],
        outWallQuads.back()._vertices.at(gero::asValue(VertexType::negAxis_posZ))[hAxis]
      );
      std::swap(
        outWallQuads.back()._vertices.at(gero::asValue(VertexType::posAxis_negZ))[hAxis],
        outWallQuads.back()._vertices.at(gero::asValue(VertexType::negAxis_negZ))[hAxis]
      );
    }

    // auto& latestVertices = outWallQuads.back()._vertices;
    // for (auto& currVertex : latestVertices) {
    //   currVertex.z += this->getWallZ(currVertex[hAxis]);
    // }
  }

  return true;
}

//MARK: merge
std::optional<WallQuad> WallQuad::getMergedQuad(const WallQuad& other) const
{
  // check if both quads are connected
  uint32_t totalConnected = 0;
  for (const glm::vec3& currVertex : this->_vertices)
  for (const glm::vec3& otherVertex : other._vertices)
    if (glm::all(glm::epsilonEqual(currVertex, otherVertex, 0.1f)))
      ++totalConnected;

  if (totalConnected != 2) {
    return std::nullopt;
  }

  // check if both quads are aligned on their orientation
  if (!glm::all(glm::epsilonEqual(this->getNormal(), other.getNormal(), 0.1f))) {
    return std::nullopt;
  }

  // TODO: only works with...?
  // for (const glm::vec3& currVertex : this->_vertices)
  // for (const glm::vec3& otherVertex : other._vertices)
  //   if (!glm::epsilonEqual(currVertex.x, otherVertex.x, 0.1f) && !glm::epsilonEqual(currVertex.y, otherVertex.y, 0.1f))
  //     return std::nullopt;

  {
    if (
      !glm::epsilonEqual(this->getVertex(VertexType::negAxis_posZ).z, this->getVertex(VertexType::posAxis_posZ).z, 0.1f) ||
      !glm::epsilonEqual(other.getVertex(VertexType::negAxis_negZ).z, other.getVertex(VertexType::posAxis_negZ).z, 0.1f)
    ) {
      return std::nullopt;
    }
  }

  tests_vec3::hash_set notSharedVerticesSet;
  for (const glm::vec3& currVertex : this->_vertices)
      notSharedVerticesSet.insert(currVertex);
  for (const glm::vec3& otherVertex : other._vertices)
      notSharedVerticesSet.insert(otherVertex);

  for (const glm::vec3& currVertex : this->_vertices)
  for (const glm::vec3& otherVertex : other._vertices)
    if (glm::all(glm::epsilonEqual(currVertex, otherVertex, 0.1f)))
      notSharedVerticesSet.erase(currVertex);

  if (notSharedVerticesSet.size() != 4) {
    // D_MYLOG("notSharedVerticesSet.size() " << notSharedVerticesSet.size());
    return std::nullopt;
  }

  std::vector<glm::vec3> notSharedVertices;
  notSharedVertices.reserve(4);
  for (const glm::vec3& coord : notSharedVerticesSet)
    notSharedVertices.push_back(coord);


  const bool isFacingX = glm::epsilonEqual(this->getVertex(VertexType::posAxis_negZ).x, this->getVertex(VertexType::negAxis_negZ).x, 0.1f);
  const std::size_t hAxis = isFacingX ? 1 : 0;
  const std::size_t fAxis = isFacingX ? 0 : 1;

  auto _findVertex = [&notSharedVertices, hAxis](bool inDirX, bool inDirY) -> const glm::vec3& {

    uint32_t bestIndex = 0;
    for (uint32_t ii = 1; ii < uint32_t(notSharedVertices.size()); ++ii)
    {
      const glm::vec3& best = notSharedVertices.at(bestIndex);
      const glm::vec3& current = notSharedVertices.at(ii);

      if (inDirX && best[hAxis] > current[hAxis]) { continue; }
      if (!inDirX && best[hAxis] < current[hAxis]) { continue; }
      if (inDirY && best.z > current.z) { continue; }
      if (!inDirY && best.z < current.z) { continue; }

      bestIndex = ii;
    }

    return notSharedVertices.at(bestIndex);
  };

  WallQuad newWallQuad;
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ)) = _findVertex(false, false);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ)) = _findVertex(true, false);
  newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ)) = _findVertex(false, true);
  newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ)) = _findVertex(true, true);

  // if (
  //   // is negX aligned?
  //   !glm::epsilonEqual(newWallQuad.getVertex(VertexType::negAxis_negZ).x, newWallQuad.getVertex(VertexType::negAxis_posZ).x, 0.1f) ||
  //   // is posX aligned?
  //   !glm::epsilonEqual(newWallQuad.getVertex(VertexType::posAxis_negZ).x, newWallQuad.getVertex(VertexType::posAxis_posZ).x, 0.1f) ||
  //   // is negY aligned?
  //   !glm::epsilonEqual(newWallQuad.getVertex(VertexType::posAxis_negZ).y, newWallQuad.getVertex(VertexType::posAxis_negZ).y, 0.1f) ||
  //   // is posY aligned?
  //   !glm::epsilonEqual(newWallQuad.getVertex(VertexType::negAxis_posZ).y, newWallQuad.getVertex(VertexType::posAxis_posZ).y, 0.1f)
  // ) {
  //   return std::nullopt;
  // }

  // check if must swap pos/neg-Axis (inverted normal)
  if (!glm::all(glm::epsilonEqual(this->getNormal(), newWallQuad.getNormal(), 0.1f))) {

    std::swap(
      newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_posZ))[hAxis],
      newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_posZ))[hAxis]
    );
    std::swap(
      newWallQuad._vertices.at(gero::asValue(VertexType::posAxis_negZ))[hAxis],
      newWallQuad._vertices.at(gero::asValue(VertexType::negAxis_negZ))[hAxis]
    );
  }

  return newWallQuad;
}

// float WallQuad::getWallZ(float inAxis) const
// {
//   const glm::vec3& posAxis_negZ_Pos = this->getVertex(VertexType::posAxis_negZ);
//   const glm::vec3& negAxis_negZ_Pos = this->getVertex(VertexType::negAxis_negZ);

//   const bool isFacingX = glm::epsilonEqual(posAxis_negZ_Pos.x, negAxis_negZ_Pos.x, 0.1f);
//   const std::size_t hAxis = isFacingX ? 1 : 0;

//   const float k_origin = negAxis_negZ_Pos[hAxis];
//   const float k_size = posAxis_negZ_Pos[hAxis] - k_origin;

//   // get Z value for AXIS
//   const float ratioAxis = (inAxis - k_origin) / k_size; // [0..1]
//   return gero::math::lerp(negAxis_negZ_Pos.z, posAxis_negZ_Pos.z, ratioAxis);
// }

//MARK: isColliding
bool WallQuad::isColliding(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  const glm::vec3 colliderMinCoord = inCenter - inSize * 0.5f;
  const glm::vec3 colliderMaxCoord = inCenter + inSize * 0.5f;

  glm::vec3 maxCoord = this->getVertex(WallQuad::VertexType::posAxis_posZ);
  glm::vec3 minCoord = this->getVertex(WallQuad::VertexType::negAxis_negZ);

  if (maxCoord.x < minCoord.x) { std::swap(maxCoord.x, minCoord.x); }
  if (maxCoord.y < minCoord.y) { std::swap(maxCoord.y, minCoord.y); }

  const bool isFacingX = glm::epsilonEqual(maxCoord.x, minCoord.x, 0.1f);
  const std::size_t hAxis = isFacingX ? 1 : 0;
  const std::size_t fAxis = isFacingX ? 0 : 1;

  if (
    // too far backward
    colliderMaxCoord.x < minCoord.x ||
    // too far forward
    colliderMinCoord.x > maxCoord.x // ||
    // // too far right
    // colliderMaxCoord.y < minCoord.y ||
    // // too far left
    // colliderMinCoord.y > maxCoord.y // ||
    // // too far down
    // colliderMaxCoord.z < minCoord.z ||
    // // too far up
    // colliderMinCoord.z > maxCoord.z
  ) {
    // out of bounds (X/Y/Z)
    D_MYLOG("[] out of bounds (X)");
    D_MYLOG("   colliderMinCoord " << colliderMinCoord);
    D_MYLOG("   colliderMaxCoord " << colliderMaxCoord);
    D_MYLOG("   minCoord " << minCoord);
    D_MYLOG("   maxCoord " << maxCoord);
    return false;
  }

  if (
    // // too far backward
    // colliderMaxCoord.x < minCoord.x ||
    // // too far forward
    // colliderMinCoord.x > maxCoord.x ||
    // too far right
    colliderMaxCoord.y < minCoord.y ||
    // too far left
    colliderMinCoord.y > maxCoord.y // ||
    // // too far down
    // colliderMaxCoord.z < minCoord.z ||
    // // too far up
    // colliderMinCoord.z > maxCoord.z
  ) {
    // out of bounds (X/Y/Z)
    D_MYLOG("[] out of bounds (Y)");
    return false;
  }

  if (
    // // too far backward
    // colliderMaxCoord.x < minCoord.x ||
    // // too far forward
    // colliderMinCoord.x > maxCoord.x ||
    // // too far right
    // colliderMaxCoord.y < minCoord.y ||
    // // too far left
    // colliderMinCoord.y > maxCoord.y ||
    // too far down
    colliderMaxCoord.z < minCoord.z ||
    // too far up
    colliderMinCoord.z > maxCoord.z
  ) {
    // out of bounds (X/Y/Z)
    D_MYLOG("[] out of bounds (Z)");
    return false;
  }

  // const float quadCenterZ = this->getFloorZ(inCenter.x, inCenter.y);
  // if (
  //   // too low
  //   colliderMaxCoord.z < quadCenterZ ||
  //   // too high
  //   colliderMinCoord.z > quadCenterZ
  // ) {
  //   // out of bounds (Z)
  //   // D_MYLOG("[] out of bounds (Z)");
  //   // D_MYLOG(" -> quadCenterZ: " << quadCenterZ);
  //   // D_MYLOG(" -> colliderMaxCoord: " << colliderMaxCoord);
  //   // D_MYLOG(" -> colliderMinCoord: " << colliderMinCoord);
  //   return false;
  // }

  return true;
}

bool WallQuad::isColliding(const glm::vec3& inCenter, float inRadius) const
{
  return this->isColliding(inCenter, glm::vec3(inRadius, inRadius, inRadius));
}

//MARK: getNormal
glm::vec3 WallQuad::getNormal() const {
  glm::vec3 normal = glm::cross(
    this->getVertex(VertexType::posAxis_negZ) - this->getOrigin(),
    this->getVertex(VertexType::negAxis_posZ) - this->getOrigin()
  );
  const float magnitude = glm::length(normal);
  if (magnitude > 0.0f) {
    normal /= magnitude;
  }
  return normal;
}

//MARK: buildVertices
void WallQuad::buildVertices(IWireFramesStackRenderer& inWireFrames) const {

  const glm::vec3 center = this->getCenter();
  // const glm::vec3 size = this->getSize();
  const glm::vec3 normal = this->getNormal();

  const glm::vec3 tmpColor = glm::vec3(1.0f, 0.6f, 0.3f);

  inWireFrames.pushCross(center, tmpColor, 0.25f);

  for (std::size_t ii = 0; ii < this->_vertices.size(); ++ii) {
    const std::size_t jj = (ii + 1) % this->_vertices.size();
    inWireFrames.pushLine(
      this->_vertices.at(ii),
      this->_vertices.at(jj),
      tmpColor);
  }

  inWireFrames.pushLine(center, center + normal, glm::vec3(1.0f, 1.0f, 0.5f));
}

//MARK: render
void WallQuad::render() const {

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  auto& wireFrames = stackRenderers.getWireFramesStack();

  // const glm::vec3& minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);
  // const glm::vec3& maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3 center = this->getCenter();
  // const glm::vec3 size = this->getSize();
  const glm::vec3 normal = this->getNormal();

  const glm::vec3 tmpColor = glm::vec3(1.0f, 0.6f, 0.3f);

  wireFrames.pushCross(center, tmpColor, 0.25f);

  for (std::size_t ii = 0; ii < this->_vertices.size(); ++ii) {
    const std::size_t jj = (ii + 1) % this->_vertices.size();
    wireFrames.pushLine(
      this->_vertices.at(ii),
      this->_vertices.at(jj),
      tmpColor);
  }

  // // cross
  // wireFrames.pushLine(
  //   this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY),
  //   this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY),
  //   glm::vec3(1.0f, 0.6f, 1.0f));
  // wireFrames.pushLine(
  //   this->getFloorVertex(FloorQuad::FloorVertexType::posX_negY),
  //   this->getFloorVertex(FloorQuad::FloorVertexType::negX_posY),
  //   glm::vec3(1.0f, 0.6f, 1.0f));



  // constexpr float k_step = 0.2f;

  // std::array<glm::vec2, 4> innerVertices = {{
  //   glm::vec2(center.x - size.x * 0.5f + k_step, center.y - size.y * 0.5f + k_step),
  //   glm::vec2(center.x + size.x * 0.5f - k_step, center.y - size.y * 0.5f + k_step),
  //   glm::vec2(center.x + size.x * 0.5f - k_step, center.y + size.y * 0.5f - k_step),
  //   glm::vec2(center.x - size.x * 0.5f + k_step, center.y + size.y * 0.5f - k_step),
  // }};

  // for (std::size_t ii = 0; ii < innerVertices.size(); ++ii) {

  //   const glm::vec2& v0 = innerVertices.at(ii);
  //   const glm::vec2& v1 = innerVertices.at((ii + 1) % innerVertices.size());
  //   const glm::vec3 v0a = glm::vec3(v0.x, v0.y, this->getFloorZ(v0.x, v0.y));
  //   const glm::vec3 v1a = glm::vec3(v1.x, v1.y, this->getFloorZ(v1.x, v1.y));

  //   wireFrames.pushLine(v0a, v1a, glm::vec3(1.0f, 0.8f, 1.0f));
  // }


  // for (std::size_t ii = 0; ii < innerVertices.size(); ++ii) {
  //   const glm::vec2& v0 = innerVertices.at(ii);
  //   const glm::vec3 v0a = glm::vec3(v0.x, v0.y, this->getFloorZ(v0.x, v0.y));
  //   wireFrames.pushLine(v0a, v0a + normal, glm::vec3(1.0f, 1.0f, 0.5f));
  // }

  wireFrames.pushLine(center, center + normal, glm::vec3(1.0f, 1.0f, 0.5f));
}

