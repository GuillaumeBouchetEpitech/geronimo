
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

  this->_floorManager.addFloorFromOrigin(glm::vec3(62.5f,30.0f, 5.0f), glm::vec2(10.0f,10.0f));
  this->_floorManager.addFloorFromOrigin(glm::vec3(60.0f,50.0f,10.0f), glm::vec2(20.0f,20.0f));
  this->_floorManager.addFloorFromOrigin(glm::vec3(90.0f,52.5f,15.0f), glm::vec2(10.0f,10.0f));
  this->_floorManager.addFloorFromOrigin(glm::vec3(90.0f,30.0f,10.0f), glm::vec2(10.0f,10.0f));

  this->_floorManager.addFloorFromOrigin(glm::vec3(60.0f,80.0f,10.0f), glm::vec2(10.0f,10.0f));
  this->_floorManager.removeFloorFromOrigin(glm::vec3(61.0f,81.0f,9.0f), glm::vec3(5.0f,5.0f,5.0f));

  this->_floorManager.connectFloors(
    this->_floorManager.getFloorQuads().at(0),
    this->_floorManager.getFloorQuads().at(1),
    FloorManager::ConnectOpts(67.5f, 6.0f)
  );

  this->_floorManager.connectFloors(
    this->_floorManager.getFloorQuads().at(1),
    this->_floorManager.getFloorQuads().at(2),
    FloorManager::ConnectOpts(57.5f, 6.0f)
  );

  this->_floorManager.connectFloors(
    this->_floorManager.getFloorQuads().at(0),
    this->_floorManager.getFloorQuads().at(3)
  );

  this->_floorManager.connectFloors(
    this->_floorManager.getFloorQuads().at(2),
    this->_floorManager.getFloorQuads().at(3)
  );

  this->_floorManager.mergeAllAdjacentQuads();

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

  this->_floorManager.render();

}
