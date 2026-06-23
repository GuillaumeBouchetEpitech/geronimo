
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

  this->_instancedBrickModels.initialize();

  {
    const std::string_view tmpFilename = "./assets/data/city-architect.jsonc";
    this->loadJson(tmpFilename);
  }

  {
    // MARK: floor bridges
    // AbstractBrickModelWeakRef currModelRef = this->_brickModelsManager._brickModelsPool.acquire("test-floors-with-bridges");
    ExpectBrickRef brickResult = this->_brickModelsManager.createNewBrick("test-floors-with-bridges");
    if (!brickResult) {
      D_MYERR("===> could not create a brick");
      switch (brickResult.error()) {
      case BrickCreateError::duplicated_name: D_MYERR(" -> duplicated_name"); break;
      default: D_MYERR(" -> unknown error?"); break;
      }
      return;
    }
    AbstractBrickModelWeakRef currModelRef = *brickResult;


    auto& currBuilder = this->_brickModelsManager._floorBuilder;
    auto& currWBuilder = this->_brickModelsManager._wallBuilder;

    {
      AbstractBrickModel& currModel = *currModelRef;
      currBuilder.setBrickModel(&currModel);
      currWBuilder.setBrickModel(&currModel);
    }

    const AbstractBrickModel& constModel = *currModelRef;

    {
      currBuilder.addFloorFromOrigin(glm::vec3(2.5f,5.0f, 5.0f), glm::vec2(10.0f,10.0f));
      currBuilder.addFloorFromOrigin(glm::vec3(0.0f,25.0f,10.0f), glm::vec2(20.0f,20.0f));
      currBuilder.addFloorFromOrigin(glm::vec3(30.0f,27.5f,15.0f), glm::vec2(10.0f,10.0f));
      currBuilder.addFloorFromOrigin(glm::vec3(30.0f,5.0f,10.0f), glm::vec2(10.0f,10.0f));

      currBuilder.connectFloors(
        constModel.getFloorsManager().getFloorQuads().at(0).getCenter(), 0.5f,
        constModel.getFloorsManager().getFloorQuads().at(1).getCenter(), 0.5f
      );

      currBuilder.connectFloors(
        constModel.getFloorsManager().getFloorQuads().at(1).getCenter(), 0.5f,
        constModel.getFloorsManager().getFloorQuads().at(2).getCenter(), 0.5f,
        FloorBuilder::ConnectOpts(constModel.getFloorsManager().getFloorQuads().at(2).getCenter().y, 6.0f)
      );

      currBuilder.connectFloors(
        constModel.getFloorsManager().getFloorQuads().at(0).getCenter(), 0.5f,
        constModel.getFloorsManager().getFloorQuads().at(3).getCenter(), 0.5f
      );

      currBuilder.connectFloors(
        constModel.getFloorsManager().getFloorQuads().at(2).getCenter(), 0.5f,
        constModel.getFloorsManager().getFloorQuads().at(3).getCenter(), 0.5f,
        FloorBuilder::ConnectOpts(constModel.getFloorsManager().getFloorQuads().at(2).getCenter().x, 6.0f)
      );
    }

    {

      {
        auto& quadA = constModel.getFloorsManager().getFloorQuads().at(2);
        auto& quadB = constModel.getFloorsManager().getFloorQuads().at(3);
        const glm::vec3 searchPos = (quadA.getCenter() + quadB.getCenter()) * 0.5f;

        currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::negX, 5.0f);
        currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::posX, 5.0f);

        currWBuilder.makeWallAdjacentToFloor(quadA, WallOrientation::posY, 5.0f);
        currWBuilder.makeWallAdjacentToFloor(quadB, WallOrientation::negY, 5.0f);
      }

      {
        auto& quadA = constModel.getFloorsManager().getFloorQuads().at(1);
        auto& quadB = constModel.getFloorsManager().getFloorQuads().at(2);
        const glm::vec3 searchPos = quadA.getCenter() + (quadB.getCenter() - quadA.getCenter()) * 0.7f;

        currWBuilder.makeWallAdjacentToFloor(quadA, WallOrientation::negX, 5.0f);
        currWBuilder.makeWallAdjacentToFloor(quadB, WallOrientation::posX, 5.0f);

        currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::negY, 5.0f);
        currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::posY, 5.0f);

        {
          auto& targetQuad = constModel.getFloorsManager().getFloorQuads().at(2);
          currWBuilder.makeWallAdjacentToFloor(targetQuad, WallOrientation::negX, 5.0f);
          glm::vec3 targetpos = constModel.getWallsManager().getWallQuads().back().getCenter();
          targetpos.x -= 1.0f;
          targetpos.y -= 2.0f;
          targetpos.z -= 2.5f;
          currWBuilder.removeWallFromOrigin(targetpos, glm::vec3(3,4,3.5));

          {
            currWBuilder.makeWallAdjacentToFloor(targetQuad, WallOrientation::negY, 5.0f);

            glm::vec3 targetpos = constModel.getWallsManager().getWallQuads().back().getCenter();
            targetpos.x -= 2.0f;
            targetpos.y -= 1.0f;
            targetpos.z -= 2.5f;
            currWBuilder.removeWallFromOrigin(targetpos, glm::vec3(4,3,3.5));
          }

          currWBuilder.mergeAllAdjacentQuads();
        }

      }
    }

    {
      this->_wireFramesStackRenderer._vertices.clear();
      this->_trianglesAccumulator._vertices.clear();
      constModel.buildVertices(this->_wireFramesStackRenderer, this->_trianglesAccumulator);

      this->_instancedBrickModels.createAlias(currModelRef->getName(), this->_wireFramesStackRenderer._vertices, this->_trianglesAccumulator._vertices);

      InstancedBrickModels::GeometryInstance instance;

      instance.position = glm::vec3(100,10,10);
      instance.orientation = glm::identity<glm::quat>();
      instance.color = glm::vec4(1,1,1, 1);
      instance.light = false;
      instance.scale = glm::vec3(1,1,1);

      this->_instancedBrickModels.pushAlias(currModelRef->getName(), instance);
    }

  }

  {
    // MARK: debug
    ExpectBrickRef brickResult = this->_brickModelsManager.createNewBrick("test-debug-1");
    if (!brickResult) {
      D_MYERR("===> could not create a brick");
      switch (brickResult.error()) {
      case BrickCreateError::duplicated_name: D_MYERR(" -> duplicated_name"); break;
      default: D_MYERR(" -> unknown error?"); break;
      }
      return;
    }
    AbstractBrickModelWeakRef currModelRef = *brickResult;

    auto& currBuilder = this->_brickModelsManager._floorBuilder;
    auto& currWBuilder = this->_brickModelsManager._wallBuilder;

    {
      AbstractBrickModel& currModel = *currModelRef;
      currBuilder.setBrickModel(&currModel);
      currWBuilder.setBrickModel(&currModel);
    }

    const AbstractBrickModel& constModel = *currModelRef;

    {
      currBuilder.addFloorFromOrigin(glm::vec3(5.0f,5.0f,5.0f), glm::vec2(20.0f,20.0f));
      currBuilder.removeFloorFromOrigin(glm::vec3(10.0f,10.0f,2.5f), glm::vec3(5.0f,5.0f,5.0f));
    }

    {
      currWBuilder.addWallFromOrigin(glm::vec3(30,5,10), glm::vec2(20,20), WallOrientation::negX);
      currWBuilder.removeWallFromOrigin(glm::vec3(28,10,20), glm::vec3(5,5,5));
    }

    {
      this->_wireFramesStackRenderer._vertices.clear();
      this->_trianglesAccumulator._vertices.clear();
      constModel.buildVertices(this->_wireFramesStackRenderer, this->_trianglesAccumulator);

      this->_instancedBrickModels.createAlias(currModelRef->getName(), this->_wireFramesStackRenderer._vertices, this->_trianglesAccumulator._vertices);

      InstancedBrickModels::GeometryInstance instance;

      instance.position = glm::vec3(100,90,10);
      instance.orientation = glm::identity<glm::quat>();
      instance.color = glm::vec4(1,1,1, 1);
      instance.light = false;
      instance.scale = glm::vec3(1,1,1);

      this->_instancedBrickModels.pushAlias(currModelRef->getName(), instance);
    }

  }

  {
    // MARK: debug2
    ExpectBrickRef brickResult = this->_brickModelsManager.createNewBrick("test-debug-2");
    if (!brickResult) {
      D_MYERR("===> could not create a brick");
      switch (brickResult.error()) {
      case BrickCreateError::duplicated_name: D_MYERR(" -> duplicated_name"); break;
      default: D_MYERR(" -> unknown error?"); break;
      }
      return;
    }
    AbstractBrickModelWeakRef currModelRef = *brickResult;

    auto& currBuilder = this->_brickModelsManager._floorBuilder;
    auto& currWBuilder = this->_brickModelsManager._wallBuilder;

    {
      AbstractBrickModel& currModel = *currModelRef;
      currBuilder.setBrickModel(&currModel);
      currWBuilder.setBrickModel(&currModel);
    }

    const AbstractBrickModel& constModel = *currModelRef;

    {
      currBuilder.addFloorFromOrigin(glm::vec3(5.0f,5.0f,5.0f), glm::vec2(20.0f,20.0f));
      currBuilder.removeFloorFromOrigin(glm::vec3(10.0f,10.0f,2.5f), glm::vec3(5.0f,5.0f,5.0f));

      currBuilder.mergeAllAdjacentQuads();
    }

    {
      currWBuilder.addWallFromOrigin(glm::vec3(30,5,10), glm::vec2(20,20), WallOrientation::negX);
      currWBuilder.removeWallFromOrigin(glm::vec3(28,10,20), glm::vec3(5,5,5));

      currWBuilder.mergeAllAdjacentQuads();
    }

    {
      this->_wireFramesStackRenderer._vertices.clear();
      this->_trianglesAccumulator._vertices.clear();
      constModel.buildVertices(this->_wireFramesStackRenderer, this->_trianglesAccumulator);

      this->_instancedBrickModels.createAlias(currModelRef->getName(), this->_wireFramesStackRenderer._vertices, this->_trianglesAccumulator._vertices);

      InstancedBrickModels::GeometryInstance instance;

      instance.position = glm::vec3(100,60,10);
      instance.orientation = glm::identity<glm::quat>();
      instance.color = glm::vec4(1,1,1, 1);
      instance.light = false;
      instance.scale = glm::vec3(1,1,1);

      this->_instancedBrickModels.pushAlias(currModelRef->getName(), instance);
    }

  }

}


//MARK: update
void CityArchitect::update(float deltaTimeSec) {
}

//MARK: render
void CityArchitect::render() {

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

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

  // this->_brickModelsManager.render(glm::identity<glm::mat4>());

  this->_instancedBrickModels.setMatricesData(camInstance.getMatricesData());
  this->_instancedBrickModels.renderAll(false);

}
