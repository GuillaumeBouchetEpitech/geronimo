
#include "CityArchitect.hpp"

#include "application/context/Context.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/system/asValue.hpp"
#include "geronimo/system/easing/easingFunctions.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/math/constants.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/math/safe-normalize.hpp"
// #include "geronimo/system/rng/RandomNumberGenerator.hpp"
#include "geronimo/system/rng/DeterministicRng.hpp"

CityArchitect::CityArchitect() {

  // const glm::vec3 k_origin = glm::vec3(80,30,0);

  this->_buildingBrickModelsManager._buildingBrickModels.emplace_back();
  auto& currModel = this->_buildingBrickModelsManager._buildingBrickModels.at(0);

  auto& currBuilder = this->_buildingBrickModelsManager._floorBuilder;
  currBuilder.setBuildingBrickModel(&currModel);

  currBuilder.addFloorFromOrigin(glm::vec3(62.5f,30.0f, 5.0f), glm::vec2(10.0f,10.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(60.0f,50.0f,10.0f), glm::vec2(20.0f,20.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(90.0f,52.5f,15.0f), glm::vec2(10.0f,10.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(90.0f,30.0f,10.0f), glm::vec2(10.0f,10.0f));

  currBuilder.addFloorFromOrigin(glm::vec3(60.0f,80.0f,10.0f), glm::vec2(20.0f,20.0f));
  currBuilder.removeFloorFromOrigin(glm::vec3(65.0f,85.0f,7.5f), glm::vec3(5.0f,5.0f,5.0f));

  currBuilder.connectFloors(
    currModel.getFloorQuads().at(0).getCenter(), 0.5f,
    currModel.getFloorQuads().at(1).getCenter(), 0.5f,
    FloorBuilder::ConnectOpts(currModel.getFloorQuads().at(0).getCenter().x, 6.0f)
  );

  currBuilder.connectFloors(
    currModel.getFloorQuads().at(1).getCenter(), 0.5f,
    currModel.getFloorQuads().at(2).getCenter(), 0.5f,
    FloorBuilder::ConnectOpts(currModel.getFloorQuads().at(2).getCenter().y, 6.0f)
  );

  currBuilder.connectFloors(
    currModel.getFloorQuads().at(0).getCenter(), 0.5f,
    currModel.getFloorQuads().at(3).getCenter(), 0.5f
  );

  currBuilder.connectFloors(
    currModel.getFloorQuads().at(2).getCenter(), 0.5f,
    currModel.getFloorQuads().at(3).getCenter(), 0.5f
  );

  currBuilder.mergeAllAdjacentQuads();

  currBuilder.addFloorFromOrigin(glm::vec3(60.0f+25.0f,80.0f,10.0f), glm::vec2(20.0f,20.0f));
  currBuilder.removeFloorFromOrigin(glm::vec3(65.0f+25.0f,85.0f,7.5f), glm::vec3(5.0f,5.0f,5.0f));

  {
    currModel._wallQuads.push_back(WallQuad::makeWallFromOrigin(glm::vec3(62,28,10), glm::vec2(20,20)));
    std::vector<glm::vec2> cutCoords;
    cutCoords.push_back(glm::vec2(38, 20));
    cutCoords.push_back(glm::vec2(43, 20));
    cutCoords.push_back(glm::vec2(43, 25));
    std::vector<WallQuad> outWallQuads;
    if (currModel._wallQuads.back().divideFromCoords(cutCoords, outWallQuads)) {
      currModel._wallQuads.pop_back();
      for (const auto& currWallQuad : outWallQuads) {
        currModel._wallQuads.push_back(currWallQuad);
      }
    }
    D_MYLOG("outWallQuads.size() " << outWallQuads.size());
  }

  {
    auto& quadA = currModel.getFloorQuads().at(2);
    auto& quadB = currModel.getFloorQuads().at(3);
    const glm::vec3 searchPos = (quadA.getCenter() + quadB.getCenter()) * 0.5f;

    std::vector<const FloorQuad*> outQuads;
    if (currModel.findFloorQuads(searchPos, 0.5f, outQuads)) {
      auto& connQuad = *outQuads.at(0);
      currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(connQuad, WallOrientation::negX, 5.0f));
      currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(connQuad, WallOrientation::posX, 5.0f));
    }

    currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(quadA, WallOrientation::posY, 5.0f));
    currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(quadB, WallOrientation::negY, 5.0f));
  }

  {
    auto& quadA = currModel.getFloorQuads().at(1);
    auto& quadB = currModel.getFloorQuads().at(2);
    const glm::vec3 searchPos = quadA.getCenter() + (quadB.getCenter() - quadA.getCenter()) * 0.7f;

    std::vector<const FloorQuad*> outQuads;
    if (currModel.findFloorQuads(searchPos, 0.5f, outQuads)) {
      auto& connQuad = *outQuads.at(0);
      currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(connQuad, WallOrientation::negY, 5.0f));
      currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(connQuad, WallOrientation::posY, 5.0f));

      const float width1 = quadB.getFloorVertex(FloorQuad::FloorVertexType::posX_posY).y - connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY).y;
      currModel._wallQuads.push_back(WallQuad::makeWallFromOrigin(
        connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY),
        glm::vec2(width1, 5.0f)
      ));

      const float width2 = connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_negY).y - quadB.getFloorVertex(FloorQuad::FloorVertexType::posX_negY).y;
      currModel._wallQuads.push_back(WallQuad::makeWallFromOrigin(
        quadB.getFloorVertex(FloorQuad::FloorVertexType::negX_negY),
        glm::vec2(width2, 5.0f)
      ));
    }

    currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(quadA, WallOrientation::negX, 5.0f));
    currModel._wallQuads.push_back(WallQuad::makeWallAdjacentToFloor(quadB, WallOrientation::posX, 5.0f));

    {
      std::vector<glm::vec2> cutCoords;
      cutCoords.push_back(glm::vec2(currModel._wallQuads.back().getCenter().y-2.0f, currModel._wallQuads.back().getCenter().z+1.0f));
      cutCoords.push_back(glm::vec2(currModel._wallQuads.back().getCenter().y+2.0f, currModel._wallQuads.back().getCenter().z+1.0f));
      // cutCoords.push_back(glm::vec2(quadB.getCenter().y, quadB.getCenter().z + 2));
      // cutCoords.push_back(glm::vec2(quadB.getCenter().y, quadB.getCenter().z + 2));
      std::vector<WallQuad> outWallQuads;
      if (currModel._wallQuads.back().divideFromCoords(cutCoords, outWallQuads)) {
        currModel._wallQuads.pop_back();
        for (const auto& currWallQuad : outWallQuads) {
          currModel._wallQuads.push_back(currWallQuad);
        }
      }
    }

  }

}

void CityArchitect::update(float deltaTimeSec) {

}

void CityArchitect::render() {

  // auto& context = Context::get();
  // auto& renderer = context.graphic.renderer;
  // // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  // auto& scene = renderer.getSceneRenderer();

  // {

  //   auto& stackRenderers = scene.getStackRenderers();

  //   {
  //     auto& wireFrames = stackRenderers.getWireFramesStack();

  //     wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(1000, 0, 0), glm::vec3(1, 0, 0));
  //     wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0));
  //     wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), glm::vec3(0, 0, 1));

  //     stackRenderers.flush();
  //   }
  // }

  for (const auto& currModel : this->_buildingBrickModelsManager._buildingBrickModels) {
    currModel.render();
  }

}
