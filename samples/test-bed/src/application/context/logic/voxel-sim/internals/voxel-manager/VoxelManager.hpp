
#pragma once

#include "./internals/TrimeshVertex.hpp"
#include "./internals/VoxelShape.hpp"
#include "./internals/VoxelModelMatrix.hpp"
#include "./internals/VoxelModelGeometry.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include <unordered_map>

class VoxelManager {

public:
  VoxelManager() = default;
  ~VoxelManager() = default;

public:
  void loadJsonFile(const std::string_view inFilepath);

public:
  // void update(float elapsedTime);
  void render();

public:
  struct ShapesData {
    std::vector<std::shared_ptr<VoxelShape>> _allVoxelShapes;
    std::unordered_map<std::string, std::shared_ptr<VoxelShape>> _voxelShapesMap;
    std::unordered_map<uint32_t, std::shared_ptr<VoxelShape>> _voxelShapesAliasMap;
  }
  shapesData;

  struct ModelsData {
    std::vector<std::shared_ptr<VoxelModelMatrix>> allVoxelMatrices;
    std::unordered_map<std::string, std::shared_ptr<VoxelModelMatrix>> voxelMatricesMap;

    std::vector<std::shared_ptr<VoxelModelGeometry>> allVoxelGeometries;
  }
  modelsData;

};
