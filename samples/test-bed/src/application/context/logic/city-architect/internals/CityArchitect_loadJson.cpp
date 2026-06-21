
#include "CityArchitect.hpp"

// #include "application/context/Context.hpp"

// #include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
// #include "geronimo/system/asValue.hpp"
// #include "geronimo/system/easing/easingFunctions.hpp"
// #include "geronimo/system/math/angles.hpp"
// #include "geronimo/system/math/clamp.hpp"
// #include "geronimo/system/math/constants.hpp"
// #include "geronimo/system/math/lerp.hpp"
// #include "geronimo/system/math/safe-normalize.hpp"
// // #include "geronimo/system/rng/RandomNumberGenerator.hpp"
// #include "geronimo/system/rng/DeterministicRng.hpp"

#include "geronimo/system/parser-utils/jsonUtils.hpp"
#include "geronimo/helpers/Json.hpp"

#include <fstream> // <- std::ifstream

namespace /*anonymous*/ {

// MARK: _processTransform
glm::mat4 _processTransform(const json& inJsonData)
{
  glm::mat4 tmpMat = glm::identity<glm::mat4>();

  if (!inJsonData.is_array()) {
    D_THROW(std::invalid_argument, "transforms not an array");
  }

  for (auto transformVal : inJsonData) {

    if (!transformVal.is_object()) {
      D_THROW(std::invalid_argument, "transforms value not an object");
    }

    std::string transformTypeVal;
    jsonUtils::str::get(transformVal, "type", transformTypeVal);

    if (transformTypeVal == "translate") {
      glm::vec3 pos;
      jsonUtils::fvec3::get(transformVal, "pos", pos);
      // D_MYLOG(" => pos -> " << pos);
      tmpMat = glm::translate(tmpMat, pos);
    }
    else if (transformTypeVal == "rotate") {
      glm::vec3 axis;
      float angle;
      jsonUtils::fvec3::get(transformVal, "axis", axis);
      jsonUtils::float32::get(transformVal, "angle", angle);
      // D_MYLOG(" => axis -> " << axis);
      // D_MYLOG(" => angle -> " << angle);
      tmpMat = glm::rotate(tmpMat, glm::radians(angle), axis);
    }
    else {
      D_MYERR("unknown transform type -> " << transformTypeVal);
    }
  }

  return tmpMat;
}

};

void CityArchitect::loadJson(std::string_view inFilepath) {

  // parse json file
  std::ifstream filestream(inFilepath.data());

  constexpr bool allow_exceptions = true;
  constexpr bool ignore_comments = true;
  const json rootData = json::parse(filestream, nullptr, allow_exceptions, ignore_comments);

  {
    //MARK: bricks

    jsonUtils::common::propertyExist(rootData, "bricks");
    const auto bricksVal = rootData["bricks"];

    // std::unordered_map<std::string, json> _allBricksJsonByName;
    // for (auto pair : bricksVal.items()) {
    //   const std::string brickKey = pair.key();
    //   const auto brickVal = pair.value();

    //   _allBricksJsonByName[brickKey] = brickVal;

    //   jsonUtils::common::propertyExist(brickVal, "operations");
    //   const auto operationsVal = brickVal["operations"];
    // }

    // uint32_t currentindex = 0;

    for (auto currBrickVal : bricksVal) {

      // const std::string brickKey = pair.key();
      // const auto brickVal = pair.value();

      std::string brickNameVal;
      jsonUtils::str::get(currBrickVal, "name", brickNameVal);

      D_MYLOG(" -> name: " << brickNameVal);

      //

      ExpectBrickRef brickResult = this->_brickModelsManager.createNewBrick(brickNameVal);
      if (!brickResult) {
        D_MYERR("===> could not create a brick");
        switch (brickResult.error()) {
        case BrickCreateError::duplicated_name: D_MYERR(" -> duplicated_name"); break;
        default: D_MYERR(" -> unknown error?"); break;
        }
        continue;
      }
      AbstractBrickModelWeakRef currModelRef = *brickResult;

      auto& currBuilder = this->_brickModelsManager._floorBuilder;
      auto& currWBuilder = this->_brickModelsManager._wallBuilder;
      auto& currIBuilder = this->_brickModelsManager._brickInstanceBuilder;

      {
        AbstractBrickModel& currModel = *currModelRef;
        currBuilder.setBrickModel(&currModel);
        currWBuilder.setBrickModel(&currModel);
        currIBuilder.setBrickModel(&currModel);
      }

      const AbstractBrickModel& constModel = *currModelRef;

      //

      jsonUtils::common::propertyExist(currBrickVal, "operations");
      const auto operationsVal = currBrickVal["operations"];

      for (auto currOpval : operationsVal)
      {

        std::string opType;
        jsonUtils::str::get(currOpval, "type", opType);

        D_MYLOG(" ---> opType: " << opType);

        if (opType == "add-floor") {
          glm::vec3 floor_origin;
          glm::vec2 floor_size;
          jsonUtils::fvec3::get(currOpval, "origin", floor_origin);
          jsonUtils::fvec2::get(currOpval, "size", floor_size);

          FloorBuilder::ExpectGenericQuadRef result = currBuilder.addFloorFromOrigin(floor_origin, floor_size);
          if (!result) {
            D_MYERR("===> could not create a floor");
            switch (result.error()) {
            case FloorBuilder::QuadCreateError::is_blocked: D_MYERR(" -> is_blocked"); break;
            case FloorBuilder::QuadCreateError::not_aligned: D_MYERR(" -> not_aligned"); break;
            case FloorBuilder::QuadCreateError::out_of_range: D_MYERR(" -> out_of_range"); break;
            default: D_MYERR(" -> unknown error?"); break;
            }
          }
        }
        else if (opType == "connect-floors") {

          jsonUtils::common::propertyExist(currOpval, "searchA");
          const auto searchA_val = currOpval["searchA"];
          glm::vec3 searchA_center;
          float searchA_radius;
          jsonUtils::fvec3::get(searchA_val, "center", searchA_center);
          jsonUtils::float32::get(searchA_val, "radius", searchA_radius);

          jsonUtils::common::propertyExist(currOpval, "searchB");
          const auto searchB_val = currOpval["searchB"];
          glm::vec3 searchB_center;
          float searchB_radius;
          jsonUtils::fvec3::get(searchB_val, "center", searchB_center);
          jsonUtils::float32::get(searchB_val, "radius", searchB_radius);

          if (currOpval.contains("options")) {
            const auto opts_val = currOpval["options"];

            float axisVal;
            float widthVal;
            jsonUtils::float32::get(opts_val, "axis", axisVal);
            jsonUtils::float32::get(opts_val, "width", widthVal);

            bool result = currBuilder.connectFloors(
              searchA_center, searchA_radius,
              searchB_center, searchB_radius,
              FloorBuilder::ConnectOpts(axisVal, widthVal)
            );
            if (!result) {
              D_MYERR("===> could not connect floors (with options)");
            }
          }
          else {
            bool result = currBuilder.connectFloors(
              searchA_center, searchA_radius,
              searchB_center, searchB_radius
            );
            if (!result) {
              D_MYERR("===> could not connect floors (without options)");
            }
          }
        }
        else if (opType == "add-instance") {

          std::string targetVal;
          jsonUtils::str::get(currOpval, "target", targetVal);
          D_MYLOG("-> targetVal " << targetVal);

          auto targetResult = this->_brickModelsManager.getBrickByName(targetVal);
          if (!targetResult) {
            D_MYERR("===> could not find brick named \"" << targetVal << "\"");
            continue;
          }

          // glm::vec3 instance_origin;
          // jsonUtils::fvec3::get(currOpval, "origin", instance_origin);
          // D_MYLOG("-> instance_origin " << instance_origin);

          glm::vec3 instancePos = glm::vec3(0,0,0);
          glm::quat instanceQuat = glm::identity<glm::quat>();

          if (currOpval.contains("origin")) {
            jsonUtils::fvec3::get(currOpval, "origin", instancePos);
            D_MYLOG("-> origin " << instancePos);
          }

          if (currOpval.contains("transforms")) {
            const glm::mat4 tmpMat = _processTransform(currOpval["transforms"]);
            instancePos = tmpMat * glm::vec4(instancePos, 1.0f);
            instanceQuat = glm::toQuat(tmpMat);
          }

          auto instanceResult = currIBuilder.addInstance(
            instancePos,
            instanceQuat,
            targetResult.value()
          );
          if (!instanceResult) {
            D_MYERR("===> could not create a brick instance");
            switch (instanceResult.error()) {
            case BrickInstanceError::unknown: D_MYERR(" -> unknown"); break;
            default: D_MYERR(" -> unknown error?"); break;
            }
            continue;
          }

        }
        else {
          D_MYERR(" -> unknown operation?");
        }

      }


      // {
      //   this->_wireFramesStackRenderer._vertices.clear();
      //   constModel.buildVertices(this->_wireFramesStackRenderer);

      //   this->_instancedBrickModels.createAlias(brickNameVal, this->_wireFramesStackRenderer._vertices);

      //   InstancedBrickModels::GeometryInstance instance;

      //   instance.position = glm::vec3(110 + float(currentindex) * 50.0f, -70, 10);
      //   instance.orientation = glm::identity<glm::quat>();
      //   instance.color = glm::vec4(1,1,1, 1);
      //   instance.light = false;
      //   instance.scale = glm::vec3(1,1,1);

      //   this->_instancedBrickModels.pushAlias(brickNameVal, instance);

      //   // D_MYERR("STEP?");
      //   constModel.buildInstances(instance.position, instance.orientation, this->_instancedBrickModels);
      // }

      // currentindex += 1;

    }

  }

  {
    //MARK: scene

    jsonUtils::common::propertyExist(rootData, "scene");
    const auto sceneVal = rootData["scene"];

    for (auto currSceneVal : sceneVal) {

      std::string targetVal;
      jsonUtils::str::get(currSceneVal, "target", targetVal);
      D_MYLOG("-> targetVal " << targetVal);

      auto targetResult = this->_brickModelsManager.getBrickByName(targetVal);
      if (!targetResult) {
        D_MYERR("===> could not find brick named \"" << targetVal << "\"");
        continue;
      }

      glm::vec3 instancePos = glm::vec3(0,0,0);
      glm::quat instanceQuat = glm::identity<glm::quat>();

      if (currSceneVal.contains("origin")) {
        jsonUtils::fvec3::get(currSceneVal, "origin", instancePos);
        D_MYLOG("-> origin " << instancePos);
      }

      if (currSceneVal.contains("transforms")) {
        const glm::mat4 tmpMat = _processTransform(currSceneVal["transforms"]);
        instancePos = tmpMat * glm::vec4(instancePos, 1.0f);
        instanceQuat = glm::toQuat(tmpMat);
      }

      {
        if (!this->_instancedBrickModels.hasAlias(targetVal)) {

          this->_wireFramesStackRenderer._vertices.clear();
          targetResult.value()->buildVertices(this->_wireFramesStackRenderer);

          this->_instancedBrickModels.createAlias(targetVal, this->_wireFramesStackRenderer._vertices);
        }

        InstancedBrickModels::GeometryInstance instance;

        instance.position = instancePos;
        instance.orientation = instanceQuat;
        instance.color = glm::vec4(1,1,1, 1);
        instance.light = false;
        instance.scale = glm::vec3(1,1,1);

        this->_instancedBrickModels.pushAlias(targetVal, instance);

        targetResult.value()->buildInstances(instance.position, instance.orientation, this->_instancedBrickModels);
      }

    }

  }


}
