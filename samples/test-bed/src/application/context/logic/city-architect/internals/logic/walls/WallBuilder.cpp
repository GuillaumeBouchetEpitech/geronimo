
#include "WallBuilder.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <algorithm> // std::sort

// WallBuilder::WallBuilder()
// {
//   this->_model->getWallsManager()._wallQuads.reserve(256);
// }

//MARK:addWallFromOrigin
WallBuilder::ExpectGenericQuadRef WallBuilder::addWallFromOrigin(
  const glm::vec3& inOrigin,
  const glm::vec2& inSize,
  WallOrientation inWallOrientation
) {
  glm::vec3 size = glm::vec3(inSize.x, 0.1f, inSize.y);
  if (inWallOrientation == WallOrientation::posY || inWallOrientation == WallOrientation::negY) {
    std::swap(size.x, size.y);
  }
  const glm::vec3 center = inOrigin + size * 0.5f;
  if (this->_model->getWallsManager().collideWallQuads(center, size)) {
    D_MYLOG("is blocked");
    return std::unexpected(QuadCreateError::is_blocked);
  }

  this->_model->getWallsManager()._wallQuads.push_back(WallQuad::makeWallFromOrigin(inOrigin, inSize, inWallOrientation));
  return this->_model->getWallsManager()._wallQuads.back();
}

//MARK:makeWallAdjacentToFloor
WallBuilder::ExpectGenericQuadRef WallBuilder::makeWallAdjacentToFloor(
  const FloorQuad& inFloorQuad,
  WallOrientation inWallOrientation,
  float inHeight
) {
  // glm::vec3 size = glm::vec3(inSize.x, 0.1f, inSize.y);
  // if (inWallOrientation == WallOrientation::posY || inWallOrientation == WallOrientation::negY) {
  //   std::swap(size.x, size.y);
  // }
  // const glm::vec3 center = inOrigin + size * 0.5f;
  // if (this->_model->collideWallQuads(center, size)) {
  //   D_MYLOG("is blocked");
  //   return std::unexpected(QuadCreateError::is_blocked);
  // }

  this->_model->getWallsManager()._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(inFloorQuad, inWallOrientation, inHeight));
  return this->_model->getWallsManager()._wallQuads.back();
}

WallBuilder::ExpectGenericQuadRef WallBuilder::makeWallAdjacentToFloor(
  const glm::vec3& inCenter,
  const glm::vec3& inSize,
  WallOrientation inWallOrientation,
  float inHeight
) {
  std::vector<std::size_t> matchingQuads;
  if (!this->_model->getFloorsManager().findFloorQuads(inCenter, inSize, matchingQuads)) {
    D_MYLOG("no quad found");
    return std::unexpected(QuadCreateError::out_of_range);
  }
  const FloorQuad& floorQuad = this->_model->getFloorsManager()._floorQuads.at(matchingQuads.front());
  return this->makeWallAdjacentToFloor(floorQuad, inWallOrientation, inHeight);
}

WallBuilder::ExpectGenericQuadRef WallBuilder::makeWallAdjacentToFloor(
  const glm::vec3& inCenter,
  float inRadius,
  WallOrientation inWallOrientation,
  float inHeight
) {
  return this->makeWallAdjacentToFloor(inCenter, glm::vec3(inRadius,inRadius,inRadius), inWallOrientation, inHeight);
}

//MARK:removewallFromOrigin
bool WallBuilder::removeWallFromOrigin(const glm::vec3& inOrigin, const glm::vec3& inSize)
{
  const glm::vec3 center = inOrigin + inSize * 0.5f;

  std::vector<std::size_t> matchingQuads;
  if (!this->_model->getWallsManager().findWallQuads(center, inSize, matchingQuads)) {
    D_MYLOG("no quad found");
    return false;
  }

  D_MYLOG("quads found " << matchingQuads.size());

  // std::vector<glm::vec2> allCutCoords;
  // allCutCoords.reserve(4);
  // allCutCoords.push_back(glm::vec2(inOrigin.x, inOrigin.y));
  // allCutCoords.push_back(glm::vec2(inOrigin.x + inSize.x, inOrigin.y));
  // allCutCoords.push_back(glm::vec2(inOrigin.x, inOrigin.y + inSize.y));
  // allCutCoords.push_back(glm::vec2(inOrigin.x + inSize.x, inOrigin.y + inSize.y));

  std::vector<WallQuad> newSubWallQuads;
  newSubWallQuads.reserve(16);

  std::vector<std::size_t> toDeleteQuads;
  toDeleteQuads.reserve(matchingQuads.size());

  for (std::size_t index : matchingQuads) {

    auto& currQuad = this->_model->getWallsManager()._wallQuads.at(index);

    const bool isFacingX = glm::epsilonEqual(currQuad.getVertex(WallQuad::VertexType::posAxis_negZ).x, currQuad.getVertex(WallQuad::VertexType::negAxis_negZ).x, 0.1f);
    const std::size_t hAxis = isFacingX ? 1 : 0;
    const std::size_t fAxis = isFacingX ? 0 : 1;

    std::vector<glm::vec2> allCutCoords;
    allCutCoords.reserve(4);
    allCutCoords.push_back(glm::vec2(inOrigin[hAxis], inOrigin.z));
    allCutCoords.push_back(glm::vec2(inOrigin[hAxis] + inSize[hAxis], inOrigin.z));
    allCutCoords.push_back(glm::vec2(inOrigin[hAxis], inOrigin.z + inSize.z));
    allCutCoords.push_back(glm::vec2(inOrigin[hAxis] + inSize[hAxis], inOrigin.z + inSize.z));

    newSubWallQuads.clear();
    if (!currQuad.divideFromCoords(allCutCoords, newSubWallQuads)) {
      continue;
    }

    toDeleteQuads.push_back(index);

    for (const auto& newSubQuad : newSubWallQuads) {
      // D_MYLOG("try add");
      if (newSubQuad.isColliding(center, inSize - 0.1f)) {
        // D_MYLOG("    cannot add -> collided");
        continue;
      }

      this->_model->getWallsManager()._wallQuads.push_back(newSubQuad);
    }
  }

  // sort -> ascending order
  std::sort(toDeleteQuads.begin(), toDeleteQuads.end());
  // from "last item" to "first item"
  for (auto it = toDeleteQuads.rbegin(); it != toDeleteQuads.rend(); ++it) {
    this->_model->getWallsManager()._wallQuads.erase(this->_model->getWallsManager()._wallQuads.begin() + *it);
  }

  D_MYLOG("end");
  return true;
}


//MARK:mergeAdjacent
void WallBuilder::mergeAllAdjacentQuads()
{
  bool keepGoing = false;
  do {

    keepGoing = false;

    for (std::size_t ii = 0; !keepGoing && ii < this->_model->getWallsManager()._wallQuads.size(); ++ii)
    {
      const WallQuad& currQuad = this->_model->getWallsManager()._wallQuads.at(ii);
      for (std::size_t jj = ii + 1; !keepGoing && jj < this->_model->getWallsManager()._wallQuads.size(); ++jj)
      {
        const WallQuad& testQuad = this->_model->getWallsManager()._wallQuads.at(jj);

        std::optional<WallQuad> result = currQuad.getMergedQuad(testQuad);
        if (result.has_value() == false) {
          continue;
        }

        // erase test quad
        this->_model->getWallsManager()._wallQuads.erase(_model->getWallsManager()._wallQuads.begin() + int32_t(jj));
        // erase current quad
        this->_model->getWallsManager()._wallQuads.erase(_model->getWallsManager()._wallQuads.begin() + int32_t(ii));

        // add new quad
        this->_model->getWallsManager()._wallQuads.push_back(result.value());

        keepGoing = true;

        break;
      }
    }

  } while (keepGoing);

}
