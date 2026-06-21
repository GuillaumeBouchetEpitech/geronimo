
#include "FloorQuad.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/asValue.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <unordered_set>
#include <algorithm> // std::sort

//MARK: make from origin
FloorQuad FloorQuad::makeFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize)
{
  FloorQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = inOrigin + glm::vec3(inSize.x * 0.0f, inSize.y * 0.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = inOrigin + glm::vec3(inSize.x * 1.0f, inSize.y * 0.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = inOrigin + glm::vec3(inSize.x * 0.0f, inSize.y * 1.0f, 0);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = inOrigin + glm::vec3(inSize.x * 1.0f, inSize.y * 1.0f, 0);
  return newFloorQuad;
}

//MARK: make connection
FloorQuad FloorQuad::makeFloorConnection(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
  const std::array<glm::vec3, 4> tmpVertices = {{ v0, v1, v2, v3 }};

  // this logic allow us to match the right vec3
  // -> and easily get the z value that any other attempted logic failed to provide
  auto _findVertex = [&tmpVertices](bool inDirX, bool inDirY) -> const glm::vec3& {

    uint32_t bestIndex = 0;
    for (uint32_t ii = 1; ii < uint32_t(tmpVertices.size()); ++ii)
    {
      const glm::vec3& best = tmpVertices.at(bestIndex);
      const glm::vec3& current = tmpVertices.at(ii);

      if (inDirX && best.x > current.x) { continue; }
      if (!inDirX && best.x < current.x) { continue; }
      if (inDirY && best.y > current.y) { continue; }
      if (!inDirY && best.y < current.y) { continue; }

      bestIndex = ii;
    }

    return tmpVertices.at(bestIndex);
  };

  FloorQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = _findVertex(false, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = _findVertex(true, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = _findVertex(false, true);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = _findVertex(true, true);

  return newFloorQuad;
}


FloorQuad FloorQuad::makeFloorFromMat4(const FloorQuad& inQuad, const glm::mat4& inTransform) {
  FloorQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = inTransform * glm::vec4(inQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)), 1.0f);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = inTransform * glm::vec4(inQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)), 1.0f);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = inTransform * glm::vec4(inQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)), 1.0f);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = inTransform * glm::vec4(inQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)), 1.0f);
  return newFloorQuad;
}

//MARK: divideFromCoords
bool FloorQuad::divideFromCoords(const std::vector<glm::vec2>& inCutCoords, std::vector<FloorQuad>& outFloorQuads) const
{
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

  std::unordered_set<float> allCoordSetX;
  std::unordered_set<float> allCoordSetY;

  for (const glm::vec3& vertex : this->_vertices)
  {
    allCoordSetX.insert(vertex.x);
    allCoordSetY.insert(vertex.y);
  }

  const glm::vec3& this_maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3& this_minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

  constexpr float k_rangeEpsilon = 0.1f;

  for (const glm::vec2& currCutCoord : inCutCoords)
  {
    const bool isInRangeX = (
      currCutCoord.x > this_minCoord.x + k_rangeEpsilon &&
      currCutCoord.x < this_maxCoord.x - k_rangeEpsilon
    );
    const bool isInRangeY = (
      currCutCoord.y > this_minCoord.y + k_rangeEpsilon &&
      currCutCoord.y < this_maxCoord.y - k_rangeEpsilon
    );

    if (isInRangeX && isInRangeY) {
      allCoordSetX.insert(currCutCoord.x);
      allCoordSetY.insert(currCutCoord.y);
    }
    else if (isInRangeX) {
      allCoordSetX.insert(currCutCoord.x);
    }
    else if (isInRangeY) {
      allCoordSetY.insert(currCutCoord.y);
    }
  }

  if (
    allCoordSetX.size() == 2 &&
    allCoordSetY.size() == 2
  ) {
    // nothing to cut -> skip
    return false;
  }

  std::vector<float> allCoordX;
  allCoordX.reserve(allCoordSetX.size());
  for (float val : allCoordSetX)
    allCoordX.push_back(val);

  std::vector<float> allCoordY;
  allCoordY.reserve(allCoordSetY.size());
  for (float val : allCoordSetY)
    allCoordY.push_back(val);

  std::sort(allCoordX.begin(), allCoordX.end());
  std::sort(allCoordY.begin(), allCoordY.end());

  for (std::size_t xx = 0; xx + 1 < allCoordX.size(); ++xx)
  for (std::size_t yy = 0; yy + 1 < allCoordY.size(); ++yy)
  {
    const glm::vec2 minCoord = glm::vec2(allCoordX.at(xx + 0), allCoordY.at(yy + 0));
    const glm::vec2 maxCoord = glm::vec2(allCoordX.at(xx + 1), allCoordY.at(yy + 1));

    outFloorQuads.push_back(FloorQuad::makeFloorFromOrigin(glm::vec3(minCoord, 0), maxCoord - minCoord));
    auto& latestVertices = outFloorQuads.back()._vertices;
    for (auto& currVertex : latestVertices) {
      currVertex.z = this->getFloorZ(currVertex.x, currVertex.y);
    }
  }

  return true;
}

//MARK: merge
std::optional<FloorQuad> FloorQuad::getMergedQuad(const FloorQuad& other) const
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

  // // TODO: only works with
  // for (const glm::vec3& currVertex : this->_vertices)
  // for (const glm::vec3& otherVertex : other._vertices)
  //   if (!glm::epsilonEqual(currVertex.z, otherVertex.z, 0.1f))
  //     return std::nullopt;

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

  auto _findVertex = [&notSharedVertices](bool inDirX, bool inDirY) -> const glm::vec3& {

    uint32_t bestIndex = 0;
    for (uint32_t ii = 1; ii < uint32_t(notSharedVertices.size()); ++ii)
    {
      const glm::vec3& best = notSharedVertices.at(bestIndex);
      const glm::vec3& current = notSharedVertices.at(ii);

      if (inDirX && best.x > current.x) { continue; }
      if (!inDirX && best.x < current.x) { continue; }
      if (inDirY && best.y > current.y) { continue; }
      if (!inDirY && best.y < current.y) { continue; }

      bestIndex = ii;
    }

    return notSharedVertices.at(bestIndex);
  };

  FloorQuad newFloorQuad;
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_negY)) = _findVertex(false, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_negY)) = _findVertex(true, false);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::negX_posY)) = _findVertex(false, true);
  newFloorQuad._vertices.at(gero::asValue(FloorVertexType::posX_posY)) = _findVertex(true, true);

  if (
    // is negX aligned?
    !glm::epsilonEqual(newFloorQuad.getFloorVertex(FloorVertexType::negX_negY).x, newFloorQuad.getFloorVertex(FloorVertexType::negX_posY).x, 0.1f) ||
    // is posX aligned?
    !glm::epsilonEqual(newFloorQuad.getFloorVertex(FloorVertexType::posX_negY).x, newFloorQuad.getFloorVertex(FloorVertexType::posX_posY).x, 0.1f) ||
    // is negY aligned?
    !glm::epsilonEqual(newFloorQuad.getFloorVertex(FloorVertexType::posX_negY).y, newFloorQuad.getFloorVertex(FloorVertexType::posX_negY).y, 0.1f) ||
    // is posY aligned?
    !glm::epsilonEqual(newFloorQuad.getFloorVertex(FloorVertexType::negX_posY).y, newFloorQuad.getFloorVertex(FloorVertexType::posX_posY).y, 0.1f)
  ) {
    return std::nullopt;
  }

  return newFloorQuad;
}

//MARK: getFloorZ
float FloorQuad::getFloorZ(float inX, float inY) const
{
  const glm::vec3& posX_posY_Pos = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3& posX_negY_Pos = this->getFloorVertex(FloorQuad::FloorVertexType::posX_negY);
  const glm::vec3& negX_posX_Pos = this->getFloorVertex(FloorQuad::FloorVertexType::negX_posY);
  const glm::vec3& negX_negY_Pos = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

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

//MARK: isColliding
bool FloorQuad::isColliding(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  const glm::vec3 colliderMinCoord = inCenter - inSize * 0.5f;
  const glm::vec3 colliderMaxCoord = inCenter + inSize * 0.5f;

  const glm::vec3& maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3& minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

  if (
    // too far backward
    colliderMaxCoord.x < minCoord.x ||
    // too far forward
    colliderMinCoord.x > maxCoord.x ||
    // too far right
    colliderMaxCoord.y < minCoord.y ||
    // too far left
    colliderMinCoord.y > maxCoord.y
  ) {
    // out of bounds (X/Y)
    // D_MYLOG("[] out of bounds (X/Y)");
    return false;
  }

  const float quadCenterZ = this->getFloorZ(inCenter.x, inCenter.y);
  if (
    // too low
    colliderMaxCoord.z < quadCenterZ ||
    // too high
    colliderMinCoord.z > quadCenterZ
  ) {
    // out of bounds (Z)
    // D_MYLOG("[] out of bounds (Z)");
    // D_MYLOG(" -> quadCenterZ: " << quadCenterZ);
    // D_MYLOG(" -> colliderMaxCoord: " << colliderMaxCoord);
    // D_MYLOG(" -> colliderMinCoord: " << colliderMinCoord);
    return false;
  }

  return true;
}

bool FloorQuad::isColliding(const glm::vec3& inCenter, float inRadius) const
{
  return this->isColliding(inCenter, glm::vec3(inRadius, inRadius, inRadius));
}

// //MARK: isIntersecting
// bool FloorQuad::isIntersecting(const FloorQuad& other, float epsilon /*= 0.1f*/) const
// {
//   constexpr float k_rangeEpsilon = 0.1f;

//   const glm::vec3& this_maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
//   const glm::vec3& this_minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

//   const glm::vec3& other_maxCoord = other.getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
//   const glm::vec3& other_minCoord = other.getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

//   const bool k_intersect = (!(
//     this_maxCoord.x < other_minCoord.x + epsilon ||
//     this_minCoord.x > other_maxCoord.x - epsilon ||
//     this_maxCoord.y < other_minCoord.y + epsilon ||
//     this_minCoord.y > other_maxCoord.y - epsilon
//   ));

//   if (!k_intersect) {
//     return false;
//   }

//   // get overlapping rectangle

//   /**
//    * FROM
//    *   *--------*
//    *   |A       |
//    *   |  *----------*
//    *   |  |     |    |
//    *   *--|-----*    |
//    *      |         B|
//    *      *----------*
//    *
//    * TO
//    *      *-----*
//    *      |     |
//    *      *-----*
//   */

//   const float minX = std::max(this_minCoord.x, other_minCoord.x);
//   const float minY = std::max(this_minCoord.y, other_minCoord.y);
//   const float maxX = std::min(this_maxCoord.x, other_maxCoord.x);
//   const float maxY = std::min(this_maxCoord.y, other_maxCoord.y);

//   // use the intersecting rectangle vertices to compare the Z axis values

//   return (
//     glm::epsilonEqual(this->getFloorZ(minX, minY), other.getFloorZ(minX, minY), k_rangeEpsilon) &&
//     glm::epsilonEqual(this->getFloorZ(minX, maxY), other.getFloorZ(minX, maxY), k_rangeEpsilon) &&
//     glm::epsilonEqual(this->getFloorZ(maxX, minY), other.getFloorZ(maxX, minY), k_rangeEpsilon) &&
//     glm::epsilonEqual(this->getFloorZ(maxX, maxY), other.getFloorZ(maxX, maxY), k_rangeEpsilon)
//   );
// }

//MARK: getNormal
glm::vec3 FloorQuad::getNormal() const {
  glm::vec3 normal = glm::cross(
    this->getFloorVertex(FloorVertexType::posX_negY) - this->getOrigin(),
    this->getFloorVertex(FloorVertexType::negX_posY) - this->getOrigin()
  );
  const float magnitude = glm::length(normal);
  if (magnitude > 0.0f) {
    normal /= magnitude;
  }
  return normal;
}

//MARK: buildVertices
void FloorQuad::buildVertices(IWireFramesStackRenderer& inWireFrames) const {

  const glm::vec3& minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);
  const glm::vec3& maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3 center = this->getCenter();
  const glm::vec3 size = this->getSize();
  const glm::vec3 normal = this->getNormal();

  const glm::vec3 tmpColor = glm::vec3(1.0f, 0.3f, 1.0f);

  inWireFrames.pushCross(center, tmpColor, 0.25f);

  for (std::size_t ii = 0; ii < this->_vertices.size(); ++ii) {
    const std::size_t jj = (ii + 1) % this->_vertices.size();
    inWireFrames.pushLine(
      this->_vertices.at(ii),
      this->_vertices.at(jj),
      tmpColor);
  }

  constexpr float k_step = 0.2f;

  const glm::vec2 hSize = glm::vec2(size.x, size.y) * 0.5f - k_step;

  const std::array<glm::vec2, 4> innerVertices = {{
    glm::vec2(center.x - hSize.x, center.y - hSize.y),
    glm::vec2(center.x + hSize.x, center.y - hSize.y),
    glm::vec2(center.x + hSize.x, center.y + hSize.y),
    glm::vec2(center.x - hSize.x, center.y + hSize.y),
  }};

  for (std::size_t ii = 0; ii < innerVertices.size(); ++ii) {
    const std::size_t jj = (ii + 1) % this->_vertices.size();

    const glm::vec2& v0 = innerVertices.at(ii);
    const glm::vec2& v1 = innerVertices.at(jj);
    const glm::vec3 v0a = glm::vec3(v0.x, v0.y, this->getFloorZ(v0.x, v0.y));
    const glm::vec3 v1a = glm::vec3(v1.x, v1.y, this->getFloorZ(v1.x, v1.y));

    inWireFrames.pushLine(v0a, v1a, glm::vec3(1.0f, 0.8f, 1.0f));
  }

  inWireFrames.pushLine(center, center + normal, glm::vec3(1.0f, 1.0f, 0.5f));

}

//MARK: render
void FloorQuad::render() const
{

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  auto& wireFrames = stackRenderers.getWireFramesStack();

  const glm::vec3& minCoord = this->getFloorVertex(FloorQuad::FloorVertexType::negX_negY);
  const glm::vec3& maxCoord = this->getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3 center = this->getCenter();
  const glm::vec3 size = this->getSize();
  const glm::vec3 normal = this->getNormal();

  const glm::vec3 tmpColor = glm::vec3(1.0f, 0.3f, 1.0f);

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

  constexpr float k_step = 0.2f;

  const glm::vec2 hSize = glm::vec2(size.x, size.y) * 0.5f - k_step;

  const std::array<glm::vec2, 4> innerVertices = {{
    glm::vec2(center.x - hSize.x, center.y - hSize.y),
    glm::vec2(center.x + hSize.x, center.y - hSize.y),
    glm::vec2(center.x + hSize.x, center.y + hSize.y),
    glm::vec2(center.x - hSize.x, center.y + hSize.y),
  }};

  for (std::size_t ii = 0; ii < innerVertices.size(); ++ii) {
    const std::size_t jj = (ii + 1) % this->_vertices.size();

    const glm::vec2& v0 = innerVertices.at(ii);
    const glm::vec2& v1 = innerVertices.at(jj);
    const glm::vec3 v0a = glm::vec3(v0.x, v0.y, this->getFloorZ(v0.x, v0.y));
    const glm::vec3 v1a = glm::vec3(v1.x, v1.y, this->getFloorZ(v1.x, v1.y));

    wireFrames.pushLine(v0a, v1a, glm::vec3(1.0f, 0.8f, 1.0f));
  }

  // for (std::size_t ii = 0; ii < innerVertices.size(); ++ii) {
  //   const glm::vec2& v0 = innerVertices.at(ii);
  //   const glm::vec3 v0a = glm::vec3(v0.x, v0.y, this->getFloorZ(v0.x, v0.y));
  //   wireFrames.pushLine(v0a, v0a + normal, glm::vec3(1.0f, 1.0f, 0.5f));
  // }

  wireFrames.pushLine(center, center + normal, glm::vec3(1.0f, 1.0f, 0.5f));
}



