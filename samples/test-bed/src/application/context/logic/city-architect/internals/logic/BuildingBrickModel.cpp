
#include "BuildingBrickModel.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <algorithm> // std::sort

BuildingBrickModel::BuildingBrickModel()
{
  this->_floorQuads.reserve(256);
}

bool BuildingBrickModel::collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      return true;
    }
  }

  return false;
}

//MARK:findQuads
bool BuildingBrickModel::findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const
{
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(ii);
    }
  }

  return !outQuads.empty();
}

bool BuildingBrickModel::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const
{
  return findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

bool BuildingBrickModel::findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const FloorQuad*>& outQuads) const {
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(&this->_floorQuads.at(ii));
    }
  }

  return !outQuads.empty();
}

bool BuildingBrickModel::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<const FloorQuad*>& outQuads) const {
  return findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

//MARK:render
void BuildingBrickModel::render() const
{

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  // auto& wireFrames = stackRenderers.getWireFramesStack();

  // {
  //   auto& wireFrames = stackRenderers.getWireFramesStack();

  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(1000, 0, 0), glm::vec3(1, 0, 0));
  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0));
  //   wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), glm::vec3(0, 0, 1));

  //   stackRenderers.flush();
  // }

  for (auto& currQuad : this->_floorQuads) {
    currQuad.render();
  }

  for (auto& currQuad : this->_wallQuads) {
    currQuad.render();
  }

  stackRenderers.flush();

}
