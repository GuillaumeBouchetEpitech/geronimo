
#include "VoxelModelGeometry.hpp"

#include "../VoxelManager.hpp"

// #include "application/context/Context.hpp"

#include "geronimo/system/math/constants.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <array>

namespace /*anonymous*/ {

  // MARK: MyAABB
  struct MyAABB {
    glm::vec3 _min;
    glm::vec3 _max;

    MyAABB() {
      this->reset();
    }
    void reset() {
      this->_min = glm::vec3(+999999999.0f,+999999999.0f,+999999999.0f);
      this->_max = glm::vec3(-999999999.0f,-999999999.0f,-999999999.0f);
    }
    void expand(const glm::vec3& inValue) {
      this->_min = glm::min(this->_min, inValue);
      this->_max = glm::max(this->_max, inValue);
    }
    void grow(float inValue) {
      this->_min -= inValue;
      this->_max += inValue;
    }
    bool contains(const glm::vec3& inValue) const
    {
      return (!(
        inValue.x < this->_min.x ||
        inValue.y < this->_min.y ||
        inValue.z < this->_min.z ||
        inValue.x > this->_max.x ||
        inValue.y > this->_max.y ||
        inValue.z > this->_max.z
      ));
    }
  };

  // MARK: _removeDuplicatedTriangles
  void _removeDuplicatedTriangles(
    std::vector<TrimeshVertex>& tmpVertices,
    const std::vector<TrimeshVertex>& tmpSideVertices,
    std::vector<std::size_t>& toErase
  ) {

    // find the directly matching triangles
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

  // MARK: _handleDuplicatedTriangles
  bool _handleDuplicatedTriangles(
    bool reverse,
    std::vector<TrimeshVertex>& tmpVertices,
    std::vector<TrimeshVertex>& tmpSideVertices,
    const MyAABB& myAABB,
    const glm::ivec3& currCursor,
    std::vector<std::size_t>& triangleIndices,
    std::vector<std::size_t>& toErase
  ) {

    std::vector<TrimeshVertex>& currVertices = reverse ? tmpSideVertices : tmpVertices;

    triangleIndices.clear();
    for (std::size_t ii = 0; ii < currVertices.size(); ii += 3) {
      const glm::vec3 curr_v0 = currVertices.at(ii + 0).pos;
      const glm::vec3 curr_v1 = currVertices.at(ii + 1).pos;
      const glm::vec3 curr_v2 = currVertices.at(ii + 2).pos;

      if (
        !myAABB.contains(curr_v0) ||
        !myAABB.contains(curr_v1) ||
        !myAABB.contains(curr_v2)
      ) {
        // outside the aabb -> skip
        continue;
      }

      triangleIndices.push_back(ii);
    }

    if (triangleIndices.size() != 2) {
      return false;
    }

    const std::size_t indexA = triangleIndices.at(0);
    const std::size_t indexB = triangleIndices.at(1);

    const glm::vec3 normal = currVertices.at(indexA + 0).norm;

    const glm::quat tmpQuat = glm::angleAxis(gero::math::hpi, normal);

    currVertices.at(indexA + 0).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexA + 0).pos - glm::vec3(currCursor));
    currVertices.at(indexA + 1).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexA + 1).pos - glm::vec3(currCursor));
    currVertices.at(indexA + 2).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexA + 2).pos - glm::vec3(currCursor));
    currVertices.at(indexB + 0).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexB + 0).pos - glm::vec3(currCursor));
    currVertices.at(indexB + 1).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexB + 1).pos - glm::vec3(currCursor));
    currVertices.at(indexB + 2).pos = glm::vec3(currCursor) + tmpQuat * (currVertices.at(indexB + 2).pos - glm::vec3(currCursor));

    _removeDuplicatedTriangles(tmpVertices, tmpSideVertices, toErase);

    return true;
  }

};

// MARK: build
void VoxelModelGeometry::build(const VoxelManager& inVoxelManager, const VoxelModelMatrix& inMatrix)
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

  tests_vec3::hash_set commonVerticesSet;
  std::vector<std::size_t> triangleIndices;
  triangleIndices.reserve(16);
  std::vector<std::size_t> toErase;
  toErase.reserve(16);
  MyAABB myAABB;

  for (int32_t zz = 0; zz < inMatrix.gridSize.z; ++zz)
  for (int32_t yy = 0; yy < inMatrix.gridSize.y; ++yy)
  for (int32_t xx = 0; xx < inMatrix.gridSize.x; ++xx)
  {
    const glm::ivec3 currCursor = glm::ivec3(xx, yy, zz);
    const VoxelModelMatrixCell& currVal = inMatrix.getValue(currCursor);
    if (currVal.shapeAlias == 0) {
      continue;
    }

    auto currItShape = inVoxelManager.shapesData._voxelShapesAliasMap.find(currVal.shapeAlias);
    if (currItShape == inVoxelManager.shapesData._voxelShapesAliasMap.end()) {
      continue;
    }

    auto currItMaterial = inVoxelManager.materialsData.voxelMaterialsAliasMap.find(currVal.colorAlias);
    if (currItMaterial == inVoxelManager.materialsData.voxelMaterialsAliasMap.end()) {
      continue;
    }

    const glm::vec3& materialColor = currItMaterial->second->color;

    // copy the current shape's vertices
    tmpVertices.clear();
    for (const auto& currVertex : currItShape->second->vertices)
    {
      TrimeshVertex newVertex;
      newVertex.pos = currVertex.pos + glm::vec3(currCursor);
      newVertex.color = materialColor;
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
      const VoxelModelMatrixCell& sideValShape = inMatrix.getValue(sideCursor);
      if (sideValShape.shapeAlias == 0) {
        continue;
      }

      auto sideIt = inVoxelManager.shapesData._voxelShapesAliasMap.find(sideValShape.shapeAlias);
      if (sideIt == inVoxelManager.shapesData._voxelShapesAliasMap.end()) {
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

      // count the shared vertices
      commonVerticesSet.clear();
      for (auto currVertex : tmpVertices)
      for (auto sideVertex : tmpSideVertices)
      {
        if (glm::distance(currVertex.pos, sideVertex.pos) < 0.1f) {
          commonVerticesSet.insert(currVertex.pos);
        }
      }

      if (commonVerticesSet.size() < 3 && commonVerticesSet.size() > 4) {
        continue;
      }

      // one common quad -> 2 triangles
      // one common triangle
      // -> must check and handle if the triangles have crossing hypotenuses

      toErase.clear();
      _removeDuplicatedTriangles(tmpVertices, tmpSideVertices, toErase);

      if (toErase.empty() == false) {
        continue;
      }

      // we're likely facing a case of "crossing triangles hypotenuses"

      myAABB.reset();
      for (auto& currVertex : commonVerticesSet)
        myAABB.expand(currVertex);
      myAABB.grow(0.1f);

      // rotate the "current quad" 90 degree on its normal
      // -> this will potentially make the triangles match
      const bool workDone = _handleDuplicatedTriangles(
        false, // reverse
        tmpVertices,
        tmpSideVertices,
        myAABB,
        currCursor,
        triangleIndices,
        toErase
      );

      // not matching triangles found -> rotate the "side quad" instead
      if (workDone == false) {

        // rotate the "side quad" 90 degree on its normal
        // -> this will potentially make the triangles match
        _handleDuplicatedTriangles(
          true, // reverse
          tmpVertices,
          tmpSideVertices,
          myAABB,
          currCursor,
          triangleIndices,
          toErase
        );

      }

      //

    }

#endif

    // accumulate the vertices kept after the optimisation pass
    for (const auto& currVertex : tmpVertices) {
      this->vertices.push_back(currVertex);
    }
  }

}
