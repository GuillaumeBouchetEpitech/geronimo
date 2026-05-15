
#include "VoxelSimImpl.hpp"

// #include "application/context/Context.hpp"

#include "geronimo/system/parser-utils/jsonUtils.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

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

#include "geronimo/helpers/Json.hpp"

#include <fstream> // <- std::ifstream
#include <algorithm> // <- std::sort



namespace /*anonymous*/ {


// MARK: _loadShapes
void _loadShapes(VoxelSimImpl& voxelSim, const json& inJsonData)
{

  jsonUtils::common::propertyExist(inJsonData, "shapes");
  const auto shapes = inJsonData["shapes"];

  voxelSim.shapesData._allVoxelShapes.reserve(64);

  // MARK: has-no-ref
  for (auto pair : shapes.items()) {

    const std::string shapeKey = pair.key();
    D_MYLOG(" -> shapeKey => " << shapeKey);

    const auto shapeVal = pair.value();

    if (shapeVal.contains("vertices")) {

      jsonUtils::common::propertyExist(shapeVal, "alias");
      jsonUtils::common::propertyExist(shapeVal, "vertices");
      // jsonUtils::common::propertyExist(shapeVal, "normals");
      jsonUtils::common::propertyExist(shapeVal, "indices");

      if (!shapeVal["vertices"].is_array()) {
        D_THROW(std::invalid_argument, "vertices not an array");
      }
      if (!shapeVal["indices"].is_array()) {
        D_THROW(std::invalid_argument, "indices not an array");
      }

      std::vector<glm::vec3> rawVertices;
      rawVertices.reserve(512);

      // std::vector<glm::vec3> normals;
      // normals.reserve(512);

      for (auto vertexVal : shapeVal["vertices"]) {
        glm::vec3 pos;
        jsonUtils::fvec3::get(vertexVal, "pos", pos);
        D_MYLOG("pos -> " << pos);

        rawVertices.push_back(pos);
      }

      // for (auto vertexVal : shapeVal["normals"]) {
      //   glm::vec3 norm;
      //   jsonUtils::fvec3::get(vertexVal, "norm", norm);
      //   D_MYLOG("norm -> " << norm);
      // }

      uint32_t alias;
      jsonUtils::uint32::get(shapeVal, "alias", alias);
      if (voxelSim.shapesData._voxelShapesAliasMap.count(alias)) {
        D_THROW(std::invalid_argument, "duplicated alias -> " << alias);
      }

      auto newShape = std::make_shared<VoxelShape>();
      newShape->name = shapeKey;
      newShape->alias = alias;

      voxelSim.shapesData._allVoxelShapes.push_back(newShape);
      voxelSim.shapesData._voxelShapesMap[newShape->name] = newShape;
      voxelSim.shapesData._voxelShapesAliasMap[newShape->alias] = newShape;

      jsonUtils::ivec3::get(shapeVal, "extrusion-axis", newShape->extrusionAxis);

      for (auto indexVal : shapeVal["indices"]) {
        glm::uvec3 tri;
        jsonUtils::uvec3::get(indexVal, "tri", tri);
        // uint32_t n;
        // jsonUtils::uint32::get(indexVal, "n", n);
        // D_MYLOG("tri -> " << tri << " | n -> " << n);
        D_MYLOG("tri -> " << tri);

        const glm::vec3 v0 = rawVertices.at(tri.x);
        const glm::vec3 v1 = rawVertices.at(tri.y);
        const glm::vec3 v2 = rawVertices.at(tri.z);

        glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
        const float magnitude = glm::length(normal);
        if (magnitude > 0.0f) {
          normal /= magnitude;
        }
        D_MYLOG("normal -> " << normal);

        newShape->vertices.push_back({ v0, normal });
        newShape->vertices.push_back({ v1, normal });
        newShape->vertices.push_back({ v2, normal });
      }
    }
  }

  // MARK: has-ref
  for (auto pair : shapes.items()) {

    const std::string shapeKey = pair.key();
    D_MYLOG(" -> shapeKey => " << shapeKey);

    const auto shapeVal = pair.value();

    if (shapeVal.contains("ref")) {

      jsonUtils::common::propertyExist(shapeVal, "ref");
      jsonUtils::common::propertyExist(shapeVal, "extrusion-axis");
      jsonUtils::common::propertyExist(shapeVal, "transforms");

      std::string refVal;
      jsonUtils::str::get(shapeVal, "ref", refVal);
      auto it = voxelSim.shapesData._voxelShapesMap.find(refVal);
      if (it == voxelSim.shapesData._voxelShapesMap.end()) {
        D_THROW(std::invalid_argument, "ref shape not found -> " << refVal);
      }
      auto refShape = it->second;

      if (!shapeVal["transforms"].is_array()) {
        D_THROW(std::invalid_argument, "transforms not an array");
      }

      uint32_t alias;
      jsonUtils::uint32::get(shapeVal, "alias", alias);
      if (voxelSim.shapesData._voxelShapesAliasMap.count(alias)) {
        D_THROW(std::invalid_argument, "duplicated alias -> " << alias);
      }

      auto newShape = std::make_shared<VoxelShape>();
      newShape->name = shapeKey;
      newShape->alias = alias;
      newShape->vertices = refShape->vertices; // deep copy

      voxelSim.shapesData._allVoxelShapes.push_back(newShape);
      voxelSim.shapesData._voxelShapesMap[newShape->name] = newShape;
      voxelSim.shapesData._voxelShapesAliasMap[newShape->alias] = newShape;

      jsonUtils::ivec3::get(shapeVal, "extrusion-axis", newShape->extrusionAxis);

      glm::mat4 tmpMat = glm::identity<glm::mat4>();

      for (auto transformVal : shapeVal["transforms"]) {

        std::string transformTypeVal;
        jsonUtils::str::get(transformVal, "type", transformTypeVal);

        if (transformTypeVal == "translate") {
          glm::vec3 pos;
          jsonUtils::fvec3::get(transformVal, "pos", pos);
          D_MYLOG(" => pos -> " << pos);
          tmpMat = glm::translate(tmpMat, pos);
        }
        else if (transformTypeVal == "rotate") {
          glm::vec3 axis;
          jsonUtils::fvec3::get(transformVal, "axis", axis);
          D_MYLOG(" => axis -> " << axis);
          float angle;
          jsonUtils::float32::get(transformVal, "angle", angle);
          D_MYLOG(" => angle -> " << angle);
          tmpMat = glm::rotate(tmpMat, glm::radians(angle), axis);
        }
        else if (transformTypeVal == "apply") {
          for (auto& vertex : newShape->vertices) {
            vertex.pos = tmpMat * glm::vec4(vertex.pos, 1.0f);
            vertex.norm = tmpMat * glm::vec4(vertex.norm, 0.0f);
          }
        }
        else if (transformTypeVal == "reset") {
          tmpMat = glm::identity<glm::mat4>();
        }
        else {
          D_THROW(std::invalid_argument, "unknown transform type -> " << transformTypeVal);
        }
      }
    }
  }

  std::sort(voxelSim.shapesData._allVoxelShapes.begin(), voxelSim.shapesData._allVoxelShapes.end(), [](const std::shared_ptr<VoxelShape> left, const std::shared_ptr<VoxelShape> right)
  {
    return left->alias < right->alias;
  });

}

// MARK: _loadModels
void _loadModels(VoxelSimImpl& voxelSim, const json& inJsonData)
{

  jsonUtils::common::propertyExist(inJsonData, "models");
  const auto modelsVal = inJsonData["models"];

  voxelSim.modelsData.allVoxelMatrices.reserve(64);

  for (auto pair : modelsVal.items()) {

    const std::string modelKey = pair.key();
    D_MYLOG(" -> modelKey => " << modelKey);

    const auto modelVal = pair.value();

    glm::vec3 modelPos;
    jsonUtils::fvec3::get(modelVal, "pos", modelPos);

    glm::quat modelQuat;
    jsonUtils::quat::tryGet(modelVal, "quat", modelQuat);

    jsonUtils::common::propertyExist(modelVal, "data-matrix");

    if (!modelVal["data-matrix"].is_object()) {
      D_THROW(std::invalid_argument, "data-matrix not an object");
    }

    const auto dataMatrixVal = modelVal["data-matrix"];

    glm::uvec3 matrixSize;
    jsonUtils::uvec3::get(dataMatrixVal, "size", matrixSize);

    jsonUtils::common::propertyExist(dataMatrixVal, "operations");
    auto operationsVal = dataMatrixVal["operations"];
    if (!operationsVal.is_array()) {
      D_THROW(std::invalid_argument, "operations not an array");
    }

    auto newMatrix = std::make_shared<VoxelModelMatrix>();
    voxelSim.modelsData.allVoxelMatrices.push_back(newMatrix);
    voxelSim.modelsData.voxelMatricesMap[modelKey] = newMatrix;

    newMatrix->gridSize = glm::max(newMatrix->gridSize, matrixSize);
    newMatrix->values.resize(newMatrix->gridSize.x * newMatrix->gridSize.y * newMatrix->gridSize.z, 0);


    for (auto currOpval : operationsVal)
    {
      std::string opType;
      jsonUtils::str::get(currOpval, "type", opType);

      if (opType == "set-matrix") {

        uint32_t value;
        jsonUtils::uint32::get(currOpval, "value", value);

        auto itShape = voxelSim.shapesData._voxelShapesAliasMap.find(value);
        if (itShape == voxelSim.shapesData._voxelShapesAliasMap.end()) {
          D_THROW(std::invalid_argument, "unknown data-matrix value -> " << value);
        }

        glm::ivec3 pos;
        jsonUtils::ivec3::get(currOpval, "pos", pos);

        glm::ivec3 scale = glm::ivec3(0,0,0);
        jsonUtils::ivec3::tryGet(currOpval, "scale", scale);

        if (itShape->second->extrusionAxis.x == 0) { scale.x = 0; }
        if (itShape->second->extrusionAxis.y == 0) { scale.y = 0; }
        if (itShape->second->extrusionAxis.z == 0) { scale.z = 0; }

        glm::ivec3 iGridSize = glm::ivec3(newMatrix->gridSize);

        const glm::ivec3 tmpDst = pos + scale;
        const glm::ivec3 tmpMin = glm::clamp(glm::min(pos, tmpDst), glm::ivec3(0,0,0), iGridSize - 1);
        const glm::ivec3 tmpMax = glm::clamp(glm::max(pos, tmpDst), glm::ivec3(0,0,0), iGridSize - 1);

        for (int32_t zz = tmpMin.z; zz <= tmpMax.z; ++zz)
        for (int32_t yy = tmpMin.y; yy <= tmpMax.y; ++yy)
        for (int32_t xx = tmpMin.x; xx <= tmpMax.x; ++xx)
        {
          newMatrix->values.at(zz * iGridSize.x * iGridSize.y + yy * iGridSize.x + xx) = value;
        }
      }
      else {
        D_THROW(std::invalid_argument, "unknown operation type -> " << opType);
      }

    }


    auto newGeometry = std::make_shared<VoxelModelGeometry>();
    newGeometry->build(voxelSim, *newMatrix);
    voxelSim.modelsData.allVoxelGeometries.push_back(newGeometry);

  }
}

}; // namespace /*anonymous*/






// MARK:_loadJsonFile
void VoxelSimImpl::_loadJsonFile(const std::string_view inFilepath)
{
  // parse json file
  std::ifstream filestream(inFilepath.data());

  constexpr bool allow_exceptions = true;
  constexpr bool ignore_comments = true;
  const json data = json::parse(filestream, nullptr, allow_exceptions, ignore_comments);

  D_MYLOG(" -> data[\"hello\"] => " << data["hello"]);

  _loadShapes(*this, data);
  _loadModels(*this, data);

  // generate geometry
  // optimized geometry
}


