
#include "FloorBuilder.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <algorithm> // std::sort

// FloorBuilder::FloorBuilder()
// {
//   this->_model->getFloorsManager()._floorQuads.reserve(256);
// }

//MARK:addFloorFromOrigin
FloorBuilder::ExpectGenericQuadRef FloorBuilder::addFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize)
{
  const glm::vec3 size = glm::vec3(inSize, 0.1f);
  const glm::vec3 center = inOrigin + size * 0.5f;
  if (this->_model->getFloorsManager().collideFloorQuads(center, size)) {
    D_MYLOG("is blocked");
    return std::unexpected(QuadCreateError::is_blocked);
  }

  this->_model->getFloorsManager()._floorQuads.push_back(FloorQuad::makeFloorFromOrigin(inOrigin, inSize));
  return this->_model->getFloorsManager()._floorQuads.back();
}

//MARK:removeFloorFromOrigin
bool FloorBuilder::removeFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec3& inSize)
{
  const glm::vec3 center = inOrigin + inSize * 0.5f;

  std::vector<std::size_t> matchingQuads;
  if (!this->_model->getFloorsManager().findFloorQuads(center, inSize, matchingQuads)) {
    D_MYLOG("no quad found");
    return false;
  }

  std::vector<glm::vec2> allCutCoords;
  allCutCoords.reserve(4);
  allCutCoords.push_back(glm::vec2(inOrigin.x, inOrigin.y));
  allCutCoords.push_back(glm::vec2(inOrigin.x + inSize.x, inOrigin.y));
  allCutCoords.push_back(glm::vec2(inOrigin.x, inOrigin.y + inSize.y));
  allCutCoords.push_back(glm::vec2(inOrigin.x + inSize.x, inOrigin.y + inSize.y));

  std::vector<FloorQuad> newSubFloorQuads;
  newSubFloorQuads.reserve(16);

  std::vector<std::size_t> toDeleteQuads;
  toDeleteQuads.reserve(matchingQuads.size());

  for (std::size_t index : matchingQuads) {

    newSubFloorQuads.clear();
    if (!this->_model->getFloorsManager()._floorQuads.at(index).divideFromCoords(allCutCoords, newSubFloorQuads)) {
      continue;
    }

    toDeleteQuads.push_back(index);

    for (const auto& newSubQuad : newSubFloorQuads) {
      // D_MYLOG("try add");
      if (newSubQuad.isColliding(center, inSize - 0.1f)) {
        // D_MYLOG("    cannot add -> collided");
        continue;
      }

      this->_model->getFloorsManager()._floorQuads.push_back(newSubQuad);
    }
  }

  // sort -> ascending order
  std::sort(toDeleteQuads.begin(), toDeleteQuads.end());
  // from "last item" to "first item"
  for (auto it = toDeleteQuads.rbegin(); it != toDeleteQuads.rend(); ++it) {
    this->_model->getFloorsManager()._floorQuads.erase(this->_model->getFloorsManager()._floorQuads.begin() + *it);
  }

  return true;
}

//MARK:connectFloors
bool FloorBuilder::connectFloors(
  const FloorQuad& inFloorA,
  const FloorQuad& inFloorB,
  const FloorBuilder::ConnectOpts& inOpts /*= ConnectOpts(0.0, 0.0f)*/
) {

  // TODO: check if adjacent

  const glm::vec3& floorA_maxCoord = inFloorA.getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3& floorA_minCoord = inFloorA.getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

  const glm::vec3& floorB_maxCoord = inFloorB.getFloorVertex(FloorQuad::FloorVertexType::posX_posY);
  const glm::vec3& floorB_minCoord = inFloorB.getFloorVertex(FloorQuad::FloorVertexType::negX_negY);

  const bool isOutsideX = (floorA_maxCoord.x < floorB_minCoord.x || floorA_minCoord.x > floorB_maxCoord.x);
  const bool isOutsideY = (floorA_maxCoord.y < floorB_minCoord.y || floorA_minCoord.y > floorB_maxCoord.y);

  if (isOutsideX && isOutsideY) {
    D_MYLOG("quads are not aligned");
    // D_MYLOG("isOutsideX && isOutsideY");
    // return std::unexpected(QuadCreateError::not_aligned);
    return false;
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
        // return std::unexpected(QuadCreateError::out_of_range);
        D_MYLOG("out of range");
        return false;
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

      this->_model->getFloorsManager()._floorQuads.push_back(FloorQuad::makeFloorConnection(
        glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
        glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
        glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
        glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
      ));
      // return this->_model->getFloorsManager()._floorQuads.back();
      return true;
    }

    std::array<glm::vec2, 4> allNewVertices;
    allNewVertices[0] = glm::vec2(posX_min, floorA_minCoord.y);
    allNewVertices[1] = glm::vec2(posX_max, floorA_minCoord.y);
    allNewVertices[2] = glm::vec2(posX_min, floorB_maxCoord.y);
    allNewVertices[3] = glm::vec2(posX_max, floorB_maxCoord.y);

    this->_model->getFloorsManager()._floorQuads.push_back(FloorQuad::makeFloorConnection(
      glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
      glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
      glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
      glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
    ));
    // return this->_model->getFloorsManager()._floorQuads.back();
    return true;

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
      // return std::unexpected(QuadCreateError::out_of_range);
      D_MYLOG("out of range");
      return false;
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

    this->_model->getFloorsManager()._floorQuads.push_back(FloorQuad::makeFloorConnection(
      glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
      glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
      glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
      glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
    ));
    // return this->_model->getFloorsManager()._floorQuads.back();
    return true;
  }

  std::array<glm::vec2, 4> allNewVertices;
  allNewVertices[0] = glm::vec2(floorA_minCoord.x, posY_min);
  allNewVertices[1] = glm::vec2(floorA_minCoord.x, posY_max);
  allNewVertices[2] = glm::vec2(floorB_maxCoord.x, posY_min);
  allNewVertices[3] = glm::vec2(floorB_maxCoord.x, posY_max);

  this->_model->getFloorsManager()._floorQuads.push_back(FloorQuad::makeFloorConnection(
    glm::vec3(allNewVertices[0], inFloorA.getFloorZ(allNewVertices[0].x, allNewVertices[0].y)),
    glm::vec3(allNewVertices[1], inFloorA.getFloorZ(allNewVertices[1].x, allNewVertices[1].y)),
    glm::vec3(allNewVertices[2], inFloorB.getFloorZ(allNewVertices[2].x, allNewVertices[2].y)),
    glm::vec3(allNewVertices[3], inFloorB.getFloorZ(allNewVertices[3].x, allNewVertices[3].y))
  ));
  // return this->_model->getFloorsManager()._floorQuads.back();
  return true;
}

//MARK:connectFloors
bool FloorBuilder::connectFloors(
  const glm::vec3& inCenterA, const glm::vec3& inSizeA,
  const glm::vec3& inCenterB, const glm::vec3& inSizeB,
  const FloorBuilder::ConnectOpts& inOpts /*= ConnectOpts(0.0, 0.0f)*/
) {
  std::vector<FloorQuad> foundQuadsA;
  std::vector<FloorQuad> foundQuadsB;
  foundQuadsA.reserve(16);
  foundQuadsB.reserve(16);

  if (
    // here using the BrickModel::findFloorQuads
    // -> useful to connect 2 floor quads from 2 different instances
    !this->_model->findFloorQuads(inCenterA, inSizeA, foundQuadsA) ||
    !this->_model->findFloorQuads(inCenterB, inSizeB, foundQuadsB) ||
    foundQuadsA.size() > 1 ||
    foundQuadsB.size() > 1
  ) {
    return false;
  }

  return this->connectFloors(
    foundQuadsA.at(0),
    foundQuadsB.at(0),
    inOpts);
}

//MARK:connectFloors
bool FloorBuilder::connectFloors(
  const glm::vec3& inCenterA, float inRadiusA,
  const glm::vec3& inCenterB, float inRadiusB,
  const FloorBuilder::ConnectOpts& inOpts /*= ConnectOpts(0.0, 0.0f)*/
) {
  return this->connectFloors(
    inCenterA, glm::vec3(inRadiusA, inRadiusA, inRadiusA),
    inCenterB, glm::vec3(inRadiusB, inRadiusB, inRadiusB),
    inOpts
  );
}


//MARK:mergeAdjacent
void FloorBuilder::mergeAllAdjacentQuads()
{
  bool keepGoing = false;
  do {

    keepGoing = false;

    for (std::size_t ii = 0; !keepGoing && ii < this->_model->getFloorsManager()._floorQuads.size(); ++ii)
    {
      const FloorQuad& currQuad = this->_model->getFloorsManager()._floorQuads.at(ii);
      for (std::size_t jj = ii + 1; !keepGoing && jj < this->_model->getFloorsManager()._floorQuads.size(); ++jj)
      {
        const FloorQuad& testQuad = this->_model->getFloorsManager()._floorQuads.at(jj);

        std::optional<FloorQuad> result = currQuad.getMergedQuad(testQuad);
        if (result.has_value() == false) {
          continue;
        }

        // erase test quad
        this->_model->getFloorsManager()._floorQuads.erase(_model->getFloorsManager()._floorQuads.begin() + int32_t(jj));
        // erase current quad
        this->_model->getFloorsManager()._floorQuads.erase(_model->getFloorsManager()._floorQuads.begin() + int32_t(ii));

        // add new quad
        this->_model->getFloorsManager()._floorQuads.push_back(result.value());

        keepGoing = true;

        break;
      }
    }

  } while (keepGoing);

}
