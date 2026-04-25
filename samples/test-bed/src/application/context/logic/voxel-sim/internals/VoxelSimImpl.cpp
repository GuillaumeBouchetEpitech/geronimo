
#include "VoxelSimImpl.hpp"

#include "application/context/Context.hpp"

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
    const std::string_view tmpFilename = "./assets/data/index.jsonc";
    this->_loadJsonFile(tmpFilename);
  }


}

void VoxelSimImpl::update(float elapsedTime) {

}

void VoxelSimImpl::render() {

  auto& context = Context::get();
  auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

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

    currShape->render(tmpOrigin);
  }

  if (!this->modelsData.allVoxelMatrices.empty()) {
    this->modelsData.allVoxelMatrices.at(0)->render(*this);
  }

  stackRenderer.flush();
}

void VoxelShape::render(const glm::vec3& inPosition)
{
  auto& context = Context::get();
  auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  for (std::size_t jj = 0; jj < vertices.size(); jj += 3) {
    const glm::vec3 v0 = vertices.at(jj + 0).pos;
    const glm::vec3 v1 = vertices.at(jj + 1).pos;
    const glm::vec3 v2 = vertices.at(jj + 2).pos;
    const glm::vec3 rawNorm = vertices.at(jj + 0).norm;
    const glm::vec3 tmpNorm = rawNorm * 0.3f;

    const glm::vec3 center = (v0 + v1 + v2) / 3.0f;

    wireFramesStack.pushLine(inPosition + v0, inPosition + v1, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v1, inPosition + v2, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v2, inPosition + v0, glm::vec3(1,1,0));

    wireFramesStack.pushLine(inPosition + center, inPosition + center + tmpNorm, glm::vec3(1,1,1));

    glm::vec4 tmpColor = glm::vec4(0.5f,0.0f,0.5f, 1);
    const float dotX = std::abs(glm::dot(rawNorm, glm::vec3(1,0,0)));
    const float dotY = std::abs(glm::dot(rawNorm, glm::vec3(0,1,0)));
    const float dotZ = std::abs(glm::dot(rawNorm, glm::vec3(0,0,1)));
    // D_MYERR("dotX " << dotX << ", dotY " << dotY << ", dotZ " << dotZ);
    // glm::greaterThan(glm::vec3(1,1,1), glm::vec3(1,1,1));
    uint32_t tmpTotal = 0;
    if (dotX > 0.5f) { tmpTotal += 1; }
    if (dotY > 0.5f) { tmpTotal += 1; }
    if (dotZ > 0.5f) { tmpTotal += 1; }
    if (tmpTotal >= 2) {
      tmpColor = glm::vec4(0.5f,0.5f,0.0f, 1);
    }
    else if (dotX > 0.95f) {
      tmpColor = glm::vec4(0.5f,0.0f,0.0f, 1);
    }
    else if (dotY > 0.95f) {
      tmpColor = glm::vec4(0.0f,0.5f,0.0f, 1);
    }
    else if (dotZ > 0.95f) {
      tmpColor = glm::vec4(0.0f,0.0f,0.5f, 1);
    }

    trianglesStack.pushTriangle(inPosition + v0, inPosition + v1, inPosition + v2, tmpColor);
  }

}

void VoxelMatrix::render(const VoxelSimImpl& voxelSimImpl)
{
  const glm::vec3 k_origin = glm::vec3(35,35,35);

  {
    auto& context = Context::get();
    auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

    auto& wireFramesStack = stackRenderer.getWireFramesStack();
    auto& trianglesStack = stackRenderer.getTrianglesStack();

    std::array<glm::vec3, 8> vertices = {{
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 0,this->gridSize.z * 0,this->gridSize.z * 0),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 1,this->gridSize.z * 0,this->gridSize.z * 0),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 1,this->gridSize.z * 1,this->gridSize.z * 0),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 0,this->gridSize.z * 1,this->gridSize.z * 0),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 0,this->gridSize.z * 0,this->gridSize.z * 1),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 1,this->gridSize.z * 0,this->gridSize.z * 1),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 1,this->gridSize.z * 1,this->gridSize.z * 1),
      k_origin - glm::vec3(0.5f) + glm::vec3(this->gridSize.z * 0,this->gridSize.z * 1,this->gridSize.z * 1),
    }};

    const glm::vec3 tmpColor = glm::vec3(1,1,0);

    wireFramesStack.pushLine(vertices[0], vertices[1], tmpColor);
    wireFramesStack.pushLine(vertices[1], vertices[2], tmpColor);
    wireFramesStack.pushLine(vertices[2], vertices[3], tmpColor);
    wireFramesStack.pushLine(vertices[3], vertices[0], tmpColor);

    wireFramesStack.pushLine(vertices[4+0], vertices[4+1], tmpColor);
    wireFramesStack.pushLine(vertices[4+1], vertices[4+2], tmpColor);
    wireFramesStack.pushLine(vertices[4+2], vertices[4+3], tmpColor);
    wireFramesStack.pushLine(vertices[4+3], vertices[4+0], tmpColor);

    wireFramesStack.pushLine(vertices[0+0], vertices[4+0], tmpColor);
    wireFramesStack.pushLine(vertices[0+1], vertices[4+1], tmpColor);
    wireFramesStack.pushLine(vertices[0+2], vertices[4+2], tmpColor);
    wireFramesStack.pushLine(vertices[0+3], vertices[4+3], tmpColor);
  }


  for (int32_t zz = 0; zz < this->gridSize.z; ++zz)
  for (int32_t yy = 0; yy < this->gridSize.y; ++yy)
  for (int32_t xx = 0; xx < this->gridSize.x; ++xx)
  {
    const uint32_t currVal = this->values.at(zz * this->gridSize.x * this->gridSize.y + yy * this->gridSize.x + xx);

    auto it = voxelSimImpl.shapesData._voxelShapesAliasMap.find(currVal);
    if (it == voxelSimImpl.shapesData._voxelShapesAliasMap.end()) {
      continue;
    }

    glm::vec3 tmpOrigin = k_origin;
    tmpOrigin += glm::vec3(xx, yy, zz);

    it->second->render(tmpOrigin);

    // (currVal)
  }

  // glm::uvec3 gridSize = glm::uvec3(1,1,1);
  // std::vector<uint32_t> values;



}

