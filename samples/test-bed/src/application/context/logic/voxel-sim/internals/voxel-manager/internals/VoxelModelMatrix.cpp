
#include "VoxelModelMatrix.hpp"

#include "../VoxelManager.hpp"

#include "application/context/Context.hpp"

#include <array>
#include <algorithm>

uint32_t VoxelModelMatrix::getValue(int32_t x, int32_t y, int32_t z) const
{
  if (
    z < 0 || z >= this->gridSize.z ||
    y < 0 || y >= this->gridSize.y ||
    x < 0 || x >= this->gridSize.x
  ) {
    // out of range -> return empty shape's alias value
    return 0;
  }

  // in range -> return stored shape's alias value
  return this->values.at(z * this->gridSize.x * this->gridSize.y + y * this->gridSize.x + x);
}

uint32_t VoxelModelMatrix::getValue(const glm::ivec3& cursor) const
{
  return this->getValue(cursor.x, cursor.y, cursor.z);
}

void VoxelModelMatrix::render(const VoxelManager& inVoxelManager) const
{
  const glm::vec3 k_origin = glm::vec3(35,35,35);
  {
    // render "debug cage"
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
    const uint32_t currVal = this->getValue(xx, yy, zz);

    auto it = inVoxelManager.shapesData._voxelShapesAliasMap.find(currVal);
    if (it == inVoxelManager.shapesData._voxelShapesAliasMap.end()) {
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
