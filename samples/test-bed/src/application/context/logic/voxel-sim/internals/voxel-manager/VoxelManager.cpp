
#include "VoxelManager.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/math/constants.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <array>
#include <algorithm>

void VoxelManager::initialize()
{
  this->voxelGeometriesStackRenderer.initialize();
}

void VoxelManager::render() {

  auto& context = Context::get();
  auto& scene = context.graphic.renderer.getSceneRenderer();
  auto& stackRenderer = scene.getStackRenderers();

  auto& sceneCamera = scene.getCamera();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  const glm::vec3 k_origin = glm::vec3(30,30,30);

  stackRenderer.flush();

  wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(100,0,0), glm::vec3(1,0,0));
  wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(0,100,0), glm::vec3(0,1,0));
  wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(0,0,100), glm::vec3(0,0,1));

  trianglesStack.pushQuad(glm::vec2(35,35), glm::vec2(50,50), glm::vec4(0.3f,0.3f,0.3f, 1), 27.0f);

  // D_MYERR("vertices.size() -> " << vertices.size());

  for (std::size_t ii = 0; ii < this->shapesData._allVoxelShapes.size(); ++ii)
  {
    const auto& currShape = this->shapesData._allVoxelShapes.at(ii);

    const float currX = float(currShape->alias / 10U);
    const float currY = float(currShape->alias % 10U);

    const auto& vertices = currShape->vertices;

    glm::vec3 tmpOrigin = k_origin;
    // tmpOrigin += glm::vec3(0.1f);
    tmpOrigin.x += currX * 2.0f;
    tmpOrigin.y += currY * 2.0f;
    // tmpOrigin.y += float(ii) * 2.0f;

    currShape->debugRender(tmpOrigin);
  }

  // // debug
  // if (!this->modelsData.allVoxelGeometries.empty()) {
  //   this->modelsData.allVoxelGeometries.at(0)->render(k_origin + glm::vec3(5,5,5));

  //   const glm::vec3 from = sceneCamera.getEye();
  //   const glm::vec3 to = sceneCamera.getEye() + sceneCamera.getForwardAxis() * 50.0f;
  //   gero::math::RayCastResult outRayCastResult;
  //   this->modelsData.allVoxelGeometries.at(0)->intersect(from, to, outRayCastResult);

  //   // D_MYERR("outRayCastResult.distance: " << outRayCastResult.distance);

  //   if (outRayCastResult.distance >= 0.0f) {
  //     wireFramesStack.pushCross(from + (to - from) * outRayCastResult.distance, glm::vec3(1,0,1), 0.5f);
  //   }
  // }

  if (this->geometriesData.allVoxelGeometryInstances.empty() == false) {
    this->geometriesData.allVoxelGeometryInstances.at(0)->debugRender();


    const glm::vec3 from = sceneCamera.getEye();
    const glm::vec3 to = sceneCamera.getEye() + sceneCamera.getForwardAxis() * 50.0f;
    gero::math::RayCastResult outRayCastResult;
    this->geometriesData.allVoxelGeometryInstances.at(0)->intersect(from, to, outRayCastResult);

    // D_MYERR("outRayCastResult.distance: " << outRayCastResult.distance);

    if (outRayCastResult.distance >= 0.0f) {
      const glm::vec3 impactPos = from + (to - from) * outRayCastResult.distance;
      const glm::vec3 impactNorm = impactPos - outRayCastResult.normal;
      wireFramesStack.pushCross(impactPos, glm::vec3(0,1,0), 0.5f);

      wireFramesStack.pushLine(impactPos, impactNorm, glm::vec3(1,1,1));
    }


    {
      VoxelGeometriesStackRenderer::GeometryInstance newInstance;
      newInstance.position = glm::vec3(10,10,10);
      newInstance.orientation = glm::identity<glm::quat>();
      newInstance.light = true;
      newInstance.scale = glm::vec3(1,1,1);
      newInstance.color = glm::vec4(1,1,1,1);
      this->voxelGeometriesStackRenderer.clearAlias(1);
      this->voxelGeometriesStackRenderer.pushAlias(1, newInstance);
      this->voxelGeometriesStackRenderer.setMatricesData(sceneCamera.getMatricesData());
      this->voxelGeometriesStackRenderer.renderAll();
    }


  }

  stackRenderer.flush();
}
