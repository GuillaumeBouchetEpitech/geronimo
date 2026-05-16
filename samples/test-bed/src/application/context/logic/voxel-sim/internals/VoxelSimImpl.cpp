
#include "VoxelSimImpl.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/math/constants.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <array>
#include <algorithm>

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

VoxelSimImpl::VoxelSimImpl() {

  {
    const std::string_view tmpFilename = "./assets/data/voxel-data.jsonc";
    this->voxelManager.loadJsonFile(tmpFilename);
  }


}

void VoxelSimImpl::update(float elapsedTime) {

}

void VoxelSimImpl::render() {

  this->voxelManager.render();

  // auto& context = Context::get();
  // auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

  // auto& wireFramesStack = stackRenderer.getWireFramesStack();
  // auto& trianglesStack = stackRenderer.getTrianglesStack();

  // const glm::vec3 k_origin = glm::vec3(30,30,30);

  // stackRenderer.flush();

  // wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(100,0,0), glm::vec3(1,0,0));
  // wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(0,100,0), glm::vec3(0,1,0));
  // wireFramesStack.pushLine(k_origin + glm::vec3(0,0,0), k_origin + glm::vec3(0,0,100), glm::vec3(0,0,1));

  // trianglesStack.pushQuad(glm::vec2(35,35), glm::vec2(50,50), glm::vec4(0.3f,0.3f,0.3f, 1), 27.0f);

  // // D_MYERR("vertices.size() -> " << vertices.size());

  // for (std::size_t ii = 0; ii < this->voxelManager.shapesData._allVoxelShapes.size(); ++ii)
  // {
  //   const auto& currShape = this->voxelManager.shapesData._allVoxelShapes.at(ii);

  //   const float currX = float(currShape->alias / 10U);
  //   const float currY = float(currShape->alias % 10U);

  //   const auto& vertices = currShape->vertices;

  //   glm::vec3 tmpOrigin = k_origin;
  //   // tmpOrigin += glm::vec3(0.1f);
  //   tmpOrigin.x += currX * 2.0f;
  //   tmpOrigin.y += currY * 2.0f;
  //   // tmpOrigin.y += float(ii) * 2.0f;

  //   currShape->render(tmpOrigin);
  // }

  // // if (!this->voxelManager.modelsData.allVoxelMatrices.empty()) {
  // //   this->voxelManager.modelsData.allVoxelMatrices.at(0)->render(*this);
  // // }

  // if (!this->voxelManager.modelsData.allVoxelGeometries.empty()) {
  //   const glm::vec3 k_origin = glm::vec3(35,35,35);
  //   this->voxelManager.modelsData.allVoxelGeometries.at(0)->render(k_origin);
  // }

  // stackRenderer.flush();
}
