
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

  // if (!this->modelsData.allVoxelMatrices.empty()) {
  //   this->modelsData.allVoxelMatrices.at(0)->render(*this);
  // }

  if (!this->modelsData.allVoxelGeometries.empty()) {
    const glm::vec3 k_origin = glm::vec3(35,35,35);
    this->modelsData.allVoxelGeometries.at(0)->render(k_origin);
  }

  stackRenderer.flush();
}

void VoxelShape::render(const glm::vec3& inPosition) const
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
      // angular -> yellow
      tmpColor = glm::vec4(0.5f,0.5f,0.0f, 1);
    }
    else if (dotX > 0.95f) {
      // X axis aligned -> red
      tmpColor = glm::vec4(0.5f,0.0f,0.0f, 1);
    }
    else if (dotY > 0.95f) {
      // Y axis aligned -> green
      tmpColor = glm::vec4(0.0f,0.5f,0.0f, 1);
    }
    else if (dotZ > 0.95f) {
      // Z axis aligned -> blue
      tmpColor = glm::vec4(0.0f,0.0f,0.5f, 1);
    }

    trianglesStack.pushTriangle(inPosition + v0, inPosition + v1, inPosition + v2, tmpColor);
  }

}

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

void VoxelModelMatrix::render(const VoxelSimImpl& voxelSimImpl) const
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

// MARK: build
void VoxelModelGeometry::build(const VoxelSimImpl& inVoxelSimImpl, const VoxelModelMatrix& inMatrix)
{
  this->vertices.clear();
  this->vertices.reserve(1024 * 2);

  std::vector<TrimeshVertex> tmpVertices;
  std::vector<TrimeshVertex> tmpSideVertices;
  tmpVertices.reserve(128);
  tmpSideVertices.reserve(128);

  const std::array<glm::ivec3, 6> sideOffsets = {{
    glm::ivec3(-1,0,0),
    glm::ivec3(+1,0,0),
    glm::ivec3(0,-1,0),
    glm::ivec3(0,+1,0),
    glm::ivec3(0,0,-1),
    glm::ivec3(0,0,+1),
  }};

  for (int32_t zz = 0; zz < inMatrix.gridSize.z; ++zz)
  for (int32_t yy = 0; yy < inMatrix.gridSize.y; ++yy)
  for (int32_t xx = 0; xx < inMatrix.gridSize.x; ++xx)
  {
    const glm::ivec3 currCursor = glm::ivec3(xx, yy, zz);
    const uint32_t currVal = inMatrix.getValue(currCursor);
    if (currVal == 0) {
      continue;
    }

    auto currIt = inVoxelSimImpl.shapesData._voxelShapesAliasMap.find(currVal);
    if (currIt == inVoxelSimImpl.shapesData._voxelShapesAliasMap.end()) {
      continue;
    }

    // copy the current shape's vertices
    tmpVertices.clear();
    for (const auto& currVertex : currIt->second->vertices)
    {
      TrimeshVertex newVertex;
      newVertex.pos = currVertex.pos + glm::vec3(currCursor);
      newVertex.norm = currVertex.norm;
      tmpVertices.push_back(newVertex);
    }

#if 1

    // TODO: optimize here?
    // -> find the shapes around this one
    // ---> count the shared vertices
    // ---> find the related triangles
    // ---> if 4 vertex match
    // -----> don't push the related triangles
    // ---> else if 3 vertex match
    // -----> check for triangle with crossing hypotenuses
    // --------> and re-arrange the triangle if needed
    // -----> don't push the related triangles

    // find the shape around this one
    for (const glm::ivec3 sideOffset : sideOffsets)
    {
      const glm::ivec3 sideCursor = currCursor + sideOffset;
      const uint32_t sideVal = inMatrix.getValue(sideCursor);
      if (sideVal == 0) {
        continue;
      }

      auto sideIt = inVoxelSimImpl.shapesData._voxelShapesAliasMap.find(sideVal);
      if (sideIt == inVoxelSimImpl.shapesData._voxelShapesAliasMap.end()) {
        continue;
      }

      // copy the side shape's vertices
      tmpSideVertices.clear();
      for (const auto& sideVertex : sideIt->second->vertices)
      {
        TrimeshVertex newVertex;
        newVertex.pos = sideVertex.pos + glm::vec3(sideCursor);
        newVertex.norm = sideVertex.norm;
        tmpSideVertices.push_back(newVertex);
      }

      // // count the shared vertices
      // uint32_t commonVertices = 0;
      tests_vec3::hash_set commonVerticesSet;
      for (auto currVertex : tmpVertices)
      for (auto sideVertex : tmpSideVertices)
      {
        if (glm::distance(currVertex.pos, sideVertex.pos) < 0.1f) {
          commonVerticesSet.insert(currVertex.pos);
        }
      }

      if (commonVerticesSet.size() >= 3 && commonVerticesSet.size() <= 4) {
        // one common quad -> 2 triangles
        // one common triangle
        // -> must check and handle if the triangles have crossing hypotenuses

        // find the directly matching triangles
        std::vector<std::size_t> toErase;
        // std::set<std::size_t> nearbyCurrTriangles;
        // std::set<std::size_t> nearbySideTriangles;
        toErase.reserve(16);
        for (std::size_t ii = 0; ii < tmpVertices.size(); ii += 3) {
          const glm::vec3 curr_v0 = tmpVertices.at(ii + 0).pos;
          const glm::vec3 curr_v1 = tmpVertices.at(ii + 1).pos;
          const glm::vec3 curr_v2 = tmpVertices.at(ii + 2).pos;
          for (std::size_t jj = 0; jj < tmpSideVertices.size(); jj += 3) {
            const glm::vec3 side_v0 = tmpSideVertices.at(jj + 0).pos;
            const glm::vec3 side_v1 = tmpSideVertices.at(jj + 1).pos;
            const glm::vec3 side_v2 = tmpSideVertices.at(jj + 2).pos;

            std::array<const glm::vec3, 3> curr_array = {{ curr_v0, curr_v1, curr_v2 }};
            std::array<const glm::vec3, 3> side_array = {{ side_v0, side_v1, side_v2 }};

            uint32_t totalNearby = 0;
            // uint32_t totalAligned = 0;
            for (std::size_t iii = 0; iii < 3; ++iii)
            for (std::size_t jjj = 0; jjj < 3; ++jjj) {
              const glm::vec3& curr_pos = curr_array.at(iii);
              const glm::vec3& side_pos = side_array.at(jjj);

              if (glm::distance(curr_pos, side_pos) < 0.001f) {
                totalNearby += 1;
              }
            }

            if (totalNearby == 3) {
              toErase.push_back(ii + 0);
              toErase.push_back(ii + 1);
              toErase.push_back(ii + 2);
            // } else if (totalNearby == 2) {
            //   nearbyCurrTriangles.insert(ii);
            //   nearbySideTriangles.insert(jj);
            }

          }
        }

        if (toErase.empty() == false) {

          // std::sort(toErase.begin(), toErase.begin(), [](std::size_t a, std::size_t b) { return a < b; });

          for (auto it = toErase.rbegin(); it != toErase.rend(); ++it) {
            tmpVertices.erase(tmpVertices.begin() + *it);
          }
        }
        else /*if (nearbyCurrTriangles.size() == 2)*/ {

          // we're likely facing a case of "crossing triangles hypotenuses"

          glm::vec3 aabb_min = glm::vec3(+999999999.0f,+999999999.0f,+999999999.0f);
          glm::vec3 aabb_max = glm::vec3(-999999999.0f,-999999999.0f,-999999999.0f);
          for (auto& currVertex : commonVerticesSet)
          {
            aabb_min = glm::min(aabb_min, currVertex);
            aabb_max = glm::max(aabb_max, currVertex);
          }
          aabb_min -= 0.1f;
          aabb_max += 0.1f;

          auto _aabb_inside = [&aabb_min, &aabb_max](const glm::vec3& currVec3) -> bool {
            return (!(
              currVec3.x < aabb_min.x ||
              currVec3.y < aabb_min.y ||
              currVec3.z < aabb_min.z ||
              currVec3.x > aabb_max.x ||
              currVec3.y > aabb_max.y ||
              currVec3.z > aabb_max.z
            ));
          };

          // commonVerticesSet
          std::vector<std::size_t> triangleIndices;
          triangleIndices.reserve(16);
          for (std::size_t ii = 0; ii < tmpVertices.size(); ii += 3) {
            const glm::vec3 curr_v0 = tmpVertices.at(ii + 0).pos;
            const glm::vec3 curr_v1 = tmpVertices.at(ii + 1).pos;
            const glm::vec3 curr_v2 = tmpVertices.at(ii + 2).pos;

            if (
              !_aabb_inside(curr_v0) ||
              !_aabb_inside(curr_v1) ||
              !_aabb_inside(curr_v2)
            ) {
              // outside the aabb -> skip
              continue;
            }

            triangleIndices.push_back(ii);
          }

          // find the "current" triangles and see if they can be "swapped"
          // -> if only one "current" triangle is found -> skip it -> the side shape must handle it

          if (triangleIndices.size() == 2) {
            // -> 2 triangles, now we must "rotate" the vertices

            const std::size_t indexA = triangleIndices.at(0);
            const std::size_t indexB = triangleIndices.at(1);

            const glm::vec3 normal = tmpVertices.at(indexA + 0).norm;

            const glm::quat tmpQuat = glm::angleAxis(gero::math::hpi, normal);

              // newVertex.pos = currVertex.pos + glm::vec3(currCursor);

            tmpVertices.at(indexA + 0).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexA + 0).pos - glm::vec3(currCursor));
            tmpVertices.at(indexA + 1).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexA + 1).pos - glm::vec3(currCursor));
            tmpVertices.at(indexA + 2).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexA + 2).pos - glm::vec3(currCursor));
            tmpVertices.at(indexB + 0).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexB + 0).pos - glm::vec3(currCursor));
            tmpVertices.at(indexB + 1).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexB + 1).pos - glm::vec3(currCursor));
            tmpVertices.at(indexB + 2).pos = glm::vec3(currCursor) + tmpQuat * (tmpVertices.at(indexB + 2).pos - glm::vec3(currCursor));


            for (std::size_t ii = 0; ii < tmpVertices.size(); ii += 3) {
              const glm::vec3 curr_v0 = tmpVertices.at(ii + 0).pos;
              const glm::vec3 curr_v1 = tmpVertices.at(ii + 1).pos;
              const glm::vec3 curr_v2 = tmpVertices.at(ii + 2).pos;
              for (std::size_t jj = 0; jj < tmpSideVertices.size(); jj += 3) {
                const glm::vec3 side_v0 = tmpSideVertices.at(jj + 0).pos;
                const glm::vec3 side_v1 = tmpSideVertices.at(jj + 1).pos;
                const glm::vec3 side_v2 = tmpSideVertices.at(jj + 2).pos;

                std::array<const glm::vec3, 3> curr_array = {{ curr_v0, curr_v1, curr_v2 }};
                std::array<const glm::vec3, 3> side_array = {{ side_v0, side_v1, side_v2 }};

                uint32_t totalNearby = 0;
                // uint32_t totalAligned = 0;
                for (std::size_t iii = 0; iii < 3; ++iii)
                for (std::size_t jjj = 0; jjj < 3; ++jjj) {
                  const glm::vec3& curr_pos = curr_array.at(iii);
                  const glm::vec3& side_pos = side_array.at(jjj);

                  if (glm::distance(curr_pos, side_pos) < 0.001f) {
                    totalNearby += 1;
                  }
                }

                if (totalNearby == 3) {
                  toErase.push_back(ii + 0);
                  toErase.push_back(ii + 1);
                  toErase.push_back(ii + 2);
                }

              }
            }

            for (auto it = toErase.rbegin(); it != toErase.rend(); ++it) {
              tmpVertices.erase(tmpVertices.begin() + *it);
            }

          }
          else {

            triangleIndices.clear();
            for (std::size_t ii = 0; ii < tmpSideVertices.size(); ii += 3) {
              const glm::vec3 curr_v0 = tmpSideVertices.at(ii + 0).pos;
              const glm::vec3 curr_v1 = tmpSideVertices.at(ii + 1).pos;
              const glm::vec3 curr_v2 = tmpSideVertices.at(ii + 2).pos;

              if (
                !_aabb_inside(curr_v0) ||
                !_aabb_inside(curr_v1) ||
                !_aabb_inside(curr_v2)
              ) {
                // outside the aabb -> skip
                continue;
              }

              triangleIndices.push_back(ii);
            }

            if (triangleIndices.size() == 2) {

              const std::size_t indexA = triangleIndices.at(0);
              const std::size_t indexB = triangleIndices.at(1);

              const glm::vec3 normal = tmpSideVertices.at(indexA + 0).norm;

              const glm::quat tmpQuat = glm::angleAxis(gero::math::hpi, normal);

                // newVertex.pos = currVertex.pos + glm::vec3(currCursor);

              tmpSideVertices.at(indexA + 0).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexA + 0).pos - glm::vec3(currCursor));
              tmpSideVertices.at(indexA + 1).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexA + 1).pos - glm::vec3(currCursor));
              tmpSideVertices.at(indexA + 2).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexA + 2).pos - glm::vec3(currCursor));
              tmpSideVertices.at(indexB + 0).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexB + 0).pos - glm::vec3(currCursor));
              tmpSideVertices.at(indexB + 1).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexB + 1).pos - glm::vec3(currCursor));
              tmpSideVertices.at(indexB + 2).pos = glm::vec3(currCursor) + tmpQuat * (tmpSideVertices.at(indexB + 2).pos - glm::vec3(currCursor));


              for (std::size_t ii = 0; ii < tmpVertices.size(); ii += 3) {
                const glm::vec3 curr_v0 = tmpVertices.at(ii + 0).pos;
                const glm::vec3 curr_v1 = tmpVertices.at(ii + 1).pos;
                const glm::vec3 curr_v2 = tmpVertices.at(ii + 2).pos;
                for (std::size_t jj = 0; jj < tmpSideVertices.size(); jj += 3) {
                  const glm::vec3 side_v0 = tmpSideVertices.at(jj + 0).pos;
                  const glm::vec3 side_v1 = tmpSideVertices.at(jj + 1).pos;
                  const glm::vec3 side_v2 = tmpSideVertices.at(jj + 2).pos;

                  std::array<const glm::vec3, 3> curr_array = {{ curr_v0, curr_v1, curr_v2 }};
                  std::array<const glm::vec3, 3> side_array = {{ side_v0, side_v1, side_v2 }};

                  uint32_t totalNearby = 0;
                  // uint32_t totalAligned = 0;
                  for (std::size_t iii = 0; iii < 3; ++iii)
                  for (std::size_t jjj = 0; jjj < 3; ++jjj) {
                    const glm::vec3& curr_pos = curr_array.at(iii);
                    const glm::vec3& side_pos = side_array.at(jjj);

                    if (glm::distance(curr_pos, side_pos) < 0.001f) {
                      totalNearby += 1;
                    }
                  }

                  if (totalNearby == 3) {
                    toErase.push_back(ii + 0);
                    toErase.push_back(ii + 1);
                    toErase.push_back(ii + 2);
                  }

                }
              }

              for (auto it = toErase.rbegin(); it != toErase.rend(); ++it) {
                tmpVertices.erase(tmpVertices.begin() + *it);
              }

            }

          }

        }


        //

      }

    }

#endif

    // accumulate the vertices kept after the optimisation pass
    for (const auto& currVertex : tmpVertices) {
      this->vertices.push_back(currVertex);
    }
  }

}

// MARK: render
void VoxelModelGeometry::render(const glm::vec3& inPosition) const
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
      // angular -> yellow
      tmpColor = glm::vec4(0.5f,0.5f,0.0f, 1);
    }
    else if (dotX > 0.95f) {
      // X axis aligned -> red
      tmpColor = glm::vec4(0.5f,0.0f,0.0f, 1);
    }
    else if (dotY > 0.95f) {
      // Y axis aligned -> green
      tmpColor = glm::vec4(0.0f,0.5f,0.0f, 1);
    }
    else if (dotZ > 0.95f) {
      // Z axis aligned -> blue
      tmpColor = glm::vec4(0.2f,0.2f,0.5f, 1);
    }

    wireFramesStack.pushLine(inPosition + v0, inPosition + v1, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v1, inPosition + v2, glm::vec3(1,1,0));
    wireFramesStack.pushLine(inPosition + v2, inPosition + v0, glm::vec3(1,1,0));
    // wireFramesStack.pushLine(inPosition + v0, inPosition + v1, tmpColor * 2.0f);
    // wireFramesStack.pushLine(inPosition + v1, inPosition + v2, tmpColor * 2.0f);
    // wireFramesStack.pushLine(inPosition + v2, inPosition + v0, tmpColor * 2.0f);

    // normal
    wireFramesStack.pushLine(inPosition + center, inPosition + center + tmpNorm, glm::vec3(1,1,1));

    // triangle
    trianglesStack.pushTriangle(inPosition + v0, inPosition + v1, inPosition + v2, tmpColor);
  }

}
