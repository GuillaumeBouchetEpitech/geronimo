
#include "FloorManager.hpp"

#include "application/context/Context.hpp"

#include <algorithm> // std::sort

FloorManager::FloorManager()
{
  this->_floorQuads.reserve(256);
}

FloorManager::ExpectGenericQuadRef FloorManager::addFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize)
{
  // TODO: check for collision

  this->_floorQuads.push_back(GenericQuad::makeFloorFromOrigin(inOrigin, inSize));
  return this->_floorQuads.back();
}

//MARK:connectFloors
FloorManager::ExpectGenericQuadRef FloorManager::connectFloors(
  const GenericQuad& inFloorA,
  const GenericQuad& inFloorB,
  const FloorManager::ConnectOpts& inOpts /*= ConnectOpts(0.0, 0.0f)*/
) {

  // TODO: check if adjacent

  const glm::vec3& floorA_maxCoord = inFloorA.getFloorVertex(GenericQuad::FloorVertexType::posX_posY);
  const glm::vec3& floorA_minCoord = inFloorA.getFloorVertex(GenericQuad::FloorVertexType::negX_negY);

  const glm::vec3& floorB_maxCoord = inFloorB.getFloorVertex(GenericQuad::FloorVertexType::posX_posY);
  const glm::vec3& floorB_minCoord = inFloorB.getFloorVertex(GenericQuad::FloorVertexType::negX_negY);

  const bool isOutsideX = (floorA_maxCoord.x < floorB_minCoord.x || floorA_minCoord.x > floorB_maxCoord.x);
  const bool isOutsideY = (floorA_maxCoord.y < floorB_minCoord.y || floorA_minCoord.y > floorB_maxCoord.y);

  if (isOutsideX && isOutsideY) {
    // D_MYLOG("isOutsideX && isOutsideY");
    return std::unexpected(ConnectionError::not_aligned);
  }

  if (isOutsideY) {

    // connected on X axis

    /**
     * FROM
     *   *----*
     *   |A   |
     *   |    |    *----*
     *   |    |    |B   |
     *   |    |    |    |
     *   |    |    |    |
     *   *----*    |    |
     *             |    |
     *             *----*
     *
     * TO
     *   *----*
     *   |A   |
     *   |    |*--**----*
     *   |    ||##||B   |
     *   |    ||##||    |
     *   |    ||##||    |
     *   *----**--*|    |
     *             |    |
     *             *----*
    */

    std::array<float, 4> allCoordX = {{ floorA_minCoord.x, floorA_maxCoord.x, floorB_minCoord.x, floorB_maxCoord.x }};
    std::sort(allCoordX.begin(), allCoordX.end());

    float posX_min = allCoordX[1];
    float posX_max = allCoordX[2];


    if (inOpts.width > 0.0f) {

      // TODO: fix bug

      /**
       * FROM
       *    *----*
       *    |A   |
       *    |    |    *----*
       *    |    |    |B   |
       * -> |    |    |    | <-
       *    |    |    |    |
       *    *----*    |    |
       *              |    |
       *              *----*
       *
       * TO
       *    *----*
       *    |A   |
       *    |    |    *----*
       *    |    |*--*|B   |
       * -> |    ||##||    | <-
       *    |    |*--*|    |
       *    *----*    |    |
       *              |    |
       *              *----*
      */

      const float hWidth = inOpts.width * 0.5f;
      const float optX_min = inOpts.center - hWidth;
      const float optX_max = inOpts.center + hWidth;

      std::array<float, 4> allOptCoordX = {{ posX_min, posX_max, optX_min, optX_max }};
      std::sort(allOptCoordX.begin(), allOptCoordX.end());

      int32_t foundIndex = -1;
      for (std::size_t ii = 0; ii + 1 < allOptCoordX.size(); ++ii) {

        const float leftVal = allOptCoordX.at(ii + 0);
        const float rightVal = allOptCoordX.at(ii + 1);

        if (inOpts.center >= leftVal && inOpts.center < rightVal) {
          foundIndex = int32_t(ii);
          break;
        }
      }
      if (foundIndex < 0) {
        // D_MYLOG("    - NO JOY");
        return std::unexpected(ConnectionError::out_of_range);
      }

      // override the "connection range"
      posX_min = allOptCoordX[std::size_t(foundIndex)];
      posX_max = allOptCoordX[std::size_t(foundIndex) + 1];
    }

    if (floorA_maxCoord.y < floorB_maxCoord.y)
    {
      std::array<glm::vec2, 4> allNewVertices;
      allNewVertices[0] = glm::vec2(posX_min, floorA_maxCoord.y);
      allNewVertices[1] = glm::vec2(posX_max, floorA_maxCoord.y);
      allNewVertices[2] = glm::vec2(posX_min, floorB_minCoord.y);
      allNewVertices[3] = glm::vec2(posX_max, floorB_minCoord.y);

      this->_floorQuads.push_back(GenericQuad::makeFloorConnection(
        glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
        glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
        glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
        glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
      ));
      return this->_floorQuads.back();
    }

    std::array<glm::vec2, 4> allNewVertices;
    allNewVertices[0] = glm::vec2(posX_min, floorA_minCoord.y);
    allNewVertices[1] = glm::vec2(posX_max, floorA_minCoord.y);
    allNewVertices[2] = glm::vec2(posX_min, floorB_maxCoord.y);
    allNewVertices[3] = glm::vec2(posX_max, floorB_maxCoord.y);

    this->_floorQuads.push_back(GenericQuad::makeFloorConnection(
      glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
      glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
      glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
      glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
    ));
    return this->_floorQuads.back();

  }
  // else if (isOutsideX) {

  // connected on Y axis

  /**
   * FROM
   *   *-------*
   *   |A      |
   *   *-------*
   *
   *
   *
   *      *-------*
   *      |B      |
   *      *-------*
   *
   * FROM
   *   *-------*
   *   |A      |
   *   *-------*
   *      *----*
   *      |####|
   *      *----*
   *      *-------*
   *      |B      |
   *      *-------*
  */

  std::array<float, 4> allCoordY = {{ floorA_minCoord.y, floorA_maxCoord.y, floorB_minCoord.y, floorB_maxCoord.y }};
  std::sort(allCoordY.begin(), allCoordY.end());

  float posY_min = allCoordY[1];
  float posY_max = allCoordY[2];

  if (inOpts.width > 0.0f) {

    // TODO: fix bug

    /**
     * FROM
     *        |
     *        v
     *   *-------*
     *   |A      |
     *   *-------*
     *
     *
     *
     *      *-------*
     *      |B      |
     *      *-------*
     *        ^
     *        |
     *
     * FROM
     *        |
     *        v
     *   *-------*
     *   |A      |
     *   *-------*
     *       *-*
     *       |#|
     *       *-*
     *      *-------*
     *      |B      |
     *      *-------*
     *        ^
     *        |
    */

    const float hWidth = inOpts.width * 0.5f;
    const float optY_min = inOpts.center - hWidth;
    const float optY_max = inOpts.center + hWidth;

    std::array<float, 4> allOptCoordY = {{ posY_min, posY_max, optY_min, optY_max }};
    std::sort(allOptCoordY.begin(), allOptCoordY.end());

    int32_t foundIndex = -1;
    for (std::size_t ii = 0; ii + 1 < allOptCoordY.size(); ++ii) {

      const float leftVal = allOptCoordY.at(ii + 0);
      const float rightVal = allOptCoordY.at(ii + 1);

      if (inOpts.center >= leftVal && inOpts.center < rightVal) {
        foundIndex = int32_t(ii);
        break;
      }
    }
    if (foundIndex < 0) {
      // D_MYLOG("    - NO JOY");
      return std::unexpected(ConnectionError::out_of_range);
    }

    // override the "connection range"
    posY_min = allOptCoordY[std::size_t(foundIndex)];
    posY_max = allOptCoordY[std::size_t(foundIndex) + 1];
  }

  if (floorA_maxCoord.x < floorB_maxCoord.x)
  {
    std::array<glm::vec2, 4> allNewVertices;
    allNewVertices[0] = glm::vec2(floorA_maxCoord.x, posY_min);
    allNewVertices[1] = glm::vec2(floorA_maxCoord.x, posY_max);
    allNewVertices[2] = glm::vec2(floorB_minCoord.x, posY_min);
    allNewVertices[3] = glm::vec2(floorB_minCoord.x, posY_max);

    this->_floorQuads.push_back(GenericQuad::makeFloorConnection(
      glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
      glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
      glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
      glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
    ));
    return this->_floorQuads.back();
  }

  std::array<glm::vec2, 4> allNewVertices;
  allNewVertices[0] = glm::vec2(floorA_minCoord.x, posY_min);
  allNewVertices[1] = glm::vec2(floorA_minCoord.x, posY_max);
  allNewVertices[2] = glm::vec2(floorB_maxCoord.x, posY_min);
  allNewVertices[3] = glm::vec2(floorB_maxCoord.x, posY_max);

  this->_floorQuads.push_back(GenericQuad::makeFloorConnection(
    glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
    glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
    glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
    glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
  ));
  return this->_floorQuads.back();
}

//MARK:findQuads
bool FloorManager::findQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<GenericQuad>& outQuads) const
{
  outQuads.clear();
  outQuads.reserve(16);

  for (GenericQuad& currQuad : const_cast<FloorManager*>(this)->_floorQuads) {
    if (currQuad.collide(inCenter, inSize)) {
      outQuads.push_back(currQuad);
    }
  }

  return !outQuads.empty();
}

bool FloorManager::findQuads(const glm::vec3& inCenter, float inRadius, std::vector<GenericQuad>& outQuads) const
{
  return findQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

//MARK:render
void FloorManager::render()
{


  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  auto& wireFrames = stackRenderers.getWireFramesStack();

  // {
  //   auto& wireFrames = stackRenderers.getWireFramesStack();

  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(1000, 0, 0), glm::vec3(1, 0, 0));
  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0));
  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), glm::vec3(0, 0, 1));

  //   stackRenderers.flush();
  // }

  for (auto& currQuad : this->_floorQuads) {

    wireFrames.pushLine(
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::negX_negY),
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::posX_negY),
      glm::vec3(1.0f, 0.3f, 1.0f));

    wireFrames.pushLine(
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::negX_posY),
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::posX_posY),
      glm::vec3(1.0f, 0.3f, 1.0f));

    wireFrames.pushLine(
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::negX_negY),
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::negX_posY),
      glm::vec3(1.0f, 0.3f, 1.0f));

    wireFrames.pushLine(
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::posX_negY),
      currQuad.getFloorVertex(GenericQuad::FloorVertexType::posX_posY),
      glm::vec3(1.0f, 0.3f, 1.0f));

  }

  stackRenderers.flush();

}
