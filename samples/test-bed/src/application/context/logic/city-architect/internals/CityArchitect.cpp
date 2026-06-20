
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
    AbstractBrickModelWeakRef currModelRef = this->_brickModelsManager._brickModelsPool.acquire();
    auto& currBuilder = this->_brickModelsManager._floorBuilder;
    auto& currWBuilder = this->_brickModelsManager._wallBuilder;

    {
      AbstractBrickModel& currModel = *currModelRef;
      currBuilder.setBrickModel(&currModel);
      currWBuilder.setBrickModel(&currModel);
    }

    const AbstractBrickModel& constModel = *currModelRef;

    {
      // pre-merge
      currBuilder.addFloorFromOrigin(glm::vec3(5.0f,35.0f,5.0f), glm::vec2(20.0f,20.0f));
      currBuilder.removeFloorFromOrigin(glm::vec3(10.0f,40.0f,2.5f), glm::vec3(5.0f,5.0f,5.0f));

      currBuilder.mergeAllAdjacentQuads();

      // post-merge
      currBuilder.addFloorFromOrigin(glm::vec3(5.0f,5.0f,5.0f), glm::vec2(20.0f,20.0f));
      currBuilder.removeFloorFromOrigin(glm::vec3(10.0f,10.0f,2.5f), glm::vec3(5.0f,5.0f,5.0f));
    }

    {
      // pre-merge
      currWBuilder.addWallFromOrigin(glm::vec3(30,35,10), glm::vec2(20,20), WallOrientation::negX);
      currWBuilder.removeWallFromOrigin(glm::vec3(28,40,20), glm::vec3(5,5,5));

      currWBuilder.mergeAllAdjacentQuads();

      // post-merge
      currWBuilder.addWallFromOrigin(glm::vec3(30,5,10), glm::vec2(20,20), WallOrientation::negX);
      currWBuilder.removeWallFromOrigin(glm::vec3(28,10,20), glm::vec3(5,5,5));
    }

    {
      // this->_wireFramesStackRenderer

      constModel.buildVertices(this->_wireFramesStackRenderer);

      const int32_t k_alias = 2001;

      this->_instancedBrickModels.createAlias(k_alias, this->_wireFramesStackRenderer._vertices);

      InstancedBrickModels::GeometryInstance instance;

      instance.position = glm::vec3(100,10,10);
      instance.orientation = glm::identity<glm::quat>();
      instance.color = glm::vec4(1,1,1, 1);
      instance.light = false;
      instance.scale = glm::vec3(1,1,1);

      this->_instancedBrickModels.pushAlias(k_alias, instance);
    }

  }


  AbstractBrickModelWeakRef currModelRef = this->_brickModelsManager._brickModelsPool.acquire();
  auto& currBuilder = this->_brickModelsManager._floorBuilder;
  auto& currWBuilder = this->_brickModelsManager._wallBuilder;

  {
    AbstractBrickModel& currModel = *currModelRef;
    currBuilder.setBrickModel(&currModel);
    currWBuilder.setBrickModel(&currModel);
  }

  const AbstractBrickModel& constModel = *currModelRef;

  currBuilder.addFloorFromOrigin(glm::vec3(62.5f,30.0f, 5.0f), glm::vec2(10.0f,10.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(60.0f,50.0f,10.0f), glm::vec2(20.0f,20.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(90.0f,52.5f,15.0f), glm::vec2(10.0f,10.0f));
  currBuilder.addFloorFromOrigin(glm::vec3(90.0f,30.0f,10.0f), glm::vec2(10.0f,10.0f));

  currBuilder.connectFloors(
    constModel.getFloorsManager().getFloorQuads().at(0).getCenter(), 0.5f,
    constModel.getFloorsManager().getFloorQuads().at(1).getCenter(), 0.5f //,
    // FloorBuilder::ConnectOpts(constModel.getFloorsManager().getFloorQuads().at(0).getCenter().x, 6.0f)
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

  // // pre-merge
  // currBuilder.addFloorFromOrigin(glm::vec3(60.0f,80.0f,10.0f), glm::vec2(20.0f,20.0f));
  // currBuilder.removeFloorFromOrigin(glm::vec3(65.0f,85.0f,7.5f), glm::vec3(5.0f,5.0f,5.0f));

  // currBuilder.mergeAllAdjacentQuads();

  // // post-merge
  // currBuilder.addFloorFromOrigin(glm::vec3(85.0f,80.0f,10.0f), glm::vec2(20.0f,20.0f));
  // currBuilder.removeFloorFromOrigin(glm::vec3(90.0f,85.0f,7.5f), glm::vec3(5.0f,5.0f,5.0f));

  // {
  //   currWBuilder.addWallFromOrigin(glm::vec3(50+62,30+28,10+10), glm::vec2(20,20), WallOrientation::posX);
  //   currWBuilder.removeWallFromOrigin(glm::vec3(50+60,30+38,10+20), glm::vec3(5,5,5));
  // }

  D_MYLOG("step1");

  {
    auto& quadA = constModel.getFloorsManager().getFloorQuads().at(2);
    auto& quadB = constModel.getFloorsManager().getFloorQuads().at(3);
    const glm::vec3 searchPos = (quadA.getCenter() + quadB.getCenter()) * 0.5f;

    // std::vector<const FloorQuad*> outQuads;
    // if (currModel.findFloorQuads(searchPos, 0.5f, outQuads)) {
    //   auto& connQuad = *outQuads.at(0);
    //   // currWBuilder.
    //   currWBuilder.makeWallAdjacentToFloor(connQuad, WallOrientation::negX, 5.0f);
    //   currWBuilder.makeWallAdjacentToFloor(connQuad, WallOrientation::posX, 5.0f);
    // }
    currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::negX, 5.0f);
    currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::posX, 5.0f);

    currWBuilder.makeWallAdjacentToFloor(quadA, WallOrientation::posY, 5.0f);
    currWBuilder.makeWallAdjacentToFloor(quadB, WallOrientation::negY, 5.0f);
  }

  D_MYLOG("step2");

  {
    auto& quadA = constModel.getFloorsManager().getFloorQuads().at(1);
    auto& quadB = constModel.getFloorsManager().getFloorQuads().at(2);
    const glm::vec3 searchPos = quadA.getCenter() + (quadB.getCenter() - quadA.getCenter()) * 0.7f;

    // std::vector<const FloorQuad*> outQuads;
    // if (currModel.findFloorQuads(searchPos, 0.5f, outQuads)) {
    //   auto& connQuad = *outQuads.at(0);
    //   currWBuilder.makeWallAdjacentToFloor(connQuad, WallOrientation::negY, 5.0f);
    //   currWBuilder.makeWallAdjacentToFloor(connQuad, WallOrientation::posY, 5.0f);

    //   // const float width1 = quadB.getFloorVertex(FloorQuad::FloorVertexType::posX_posY).y - connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY).y;
    //   // currWBuilder.addWallFromOrigin(
    //   //   connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_posY),
    //   //   glm::vec2(width1, 5.0f),
    //   //   WallOrientation::posX
    //   // );

    //   // const float width2 = connQuad.getFloorVertex(FloorQuad::FloorVertexType::posX_negY).y - quadB.getFloorVertex(FloorQuad::FloorVertexType::posX_negY).y;
    //   // currWBuilder.addWallFromOrigin(
    //   //   quadB.getFloorVertex(FloorQuad::FloorVertexType::negX_negY),
    //   //   glm::vec2(width2, 5.0f),
    //   //   WallOrientation::posX
    //   // );
    // }

    D_MYLOG("step3");

    currWBuilder.makeWallAdjacentToFloor(quadA, WallOrientation::negX, 5.0f);
    currWBuilder.makeWallAdjacentToFloor(quadB, WallOrientation::posX, 5.0f);

    currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::negY, 5.0f);
    currWBuilder.makeWallAdjacentToFloor(searchPos, 0.5f, WallOrientation::posY, 5.0f);

    {
      // currWBuilder.addWallFromOrigin(glm::vec3(62,28,10), glm::vec2(20,20));
      // currWBuilder.removeWallFromOrigin(glm::vec3(quadB.getCenter().x, quadB.getCenter().y, quadB.getCenter().z + 1), glm::vec3(3,100,3));
      // currWBuilder.removeWallFromOrigin(glm::vec3(quadB.getCenter().x, quadB.getCenter().y, quadB.getCenter().z + 1), glm::vec3(100,3,3));


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

  // {
  //   InstancedBrickModels::Vertices vertices;

  //   // glm::vec3 position. glm::vec3 normal. glm::vec3 color
  //   vertices.emplace_back(glm::vec3( 0, 0, 0), glm::vec3(0,0,1), glm::vec3(1,1,1));
  //   vertices.emplace_back(glm::vec3(10, 0, 0), glm::vec3(0,0,1), glm::vec3(1,1,1));
  //   vertices.emplace_back(glm::vec3( 0, 0, 0), glm::vec3(0,0,1), glm::vec3(1,1,1));
  //   vertices.emplace_back(glm::vec3( 0,10, 0), glm::vec3(0,0,1), glm::vec3(1,1,1));
  //   vertices.emplace_back(glm::vec3( 0, 0, 0), glm::vec3(0,0,1), glm::vec3(1,1,1));
  //   vertices.emplace_back(glm::vec3( 0, 0,10), glm::vec3(0,0,1), glm::vec3(1,1,1));

  //   const int32_t k_alias = 1000;

  //   this->_instancedBrickModels.createAlias(k_alias, vertices);

  //   InstancedBrickModels::GeometryInstance instance;

  //   instance.position = glm::vec3(100,1,1);
  //   instance.orientation = glm::identity<glm::quat>();
  //   instance.color = glm::vec4(1,1,1, 1);
  //   instance.light = false;
  //   instance.scale = glm::vec3(1,1,1);

  //   this->_instancedBrickModels.pushAlias(k_alias, instance);
  // }

  // {
  //   // this->_wireFramesStackRenderer

  //   constModel.buildVertices(this->_wireFramesStackRenderer);


  //   const int32_t k_alias = 2000;

  //   this->_instancedBrickModels.createAlias(k_alias, this->_wireFramesStackRenderer._vertices);

  //   InstancedBrickModels::GeometryInstance instance;

  //   instance.position = glm::vec3(100,1,1);
  //   instance.orientation = glm::identity<glm::quat>();
  //   instance.color = glm::vec4(1,1,1, 1);
  //   instance.light = false;
  //   instance.scale = glm::vec3(1,1,1);

  //   this->_instancedBrickModels.pushAlias(k_alias, instance);
  // }

}

void CityArchitect::update(float deltaTimeSec) {

}

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
