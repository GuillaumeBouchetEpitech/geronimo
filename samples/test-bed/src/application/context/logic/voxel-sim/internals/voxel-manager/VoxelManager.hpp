
#pragma once

#include "./internals/TrimeshVertex.hpp"
#include "./internals/VoxelShape.hpp"
#include "./internals/VoxelModelMaterial.hpp"
#include "./internals/VoxelModelMatrix.hpp"
#include "./internals/VoxelModelGeometry.hpp"
#include "./internals/VoxelModelGeometryInstance.hpp"

#include "./internals/graphics/voxel-geometries-stack-renderer/VoxelGeometriesStackRenderer.hpp"

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
  void initialize();

public:
  void loadJsonFile(const std::string_view inFilepath);

public:
  void render();

public:
  struct ShapesData {
    std::vector<std::shared_ptr<VoxelShape>> _allVoxelShapes;
    std::unordered_map<std::string, std::shared_ptr<VoxelShape>> _voxelShapesMap;
    std::unordered_map<uint32_t, std::shared_ptr<VoxelShape>> _voxelShapesAliasMap;
  }
  shapesData;

  struct MaterialsData {
    std::vector<std::shared_ptr<VoxelModelMaterial>> allVoxelMaterials;
    std::unordered_map<std::string, std::shared_ptr<VoxelModelMaterial>> voxelMaterialsMap;
    std::unordered_map<uint32_t, std::shared_ptr<VoxelModelMaterial>> voxelMaterialsAliasMap;
  }
  materialsData;

  struct ModelsData {
    std::vector<std::shared_ptr<VoxelModelMatrix>> allVoxelMatrices;
    std::unordered_map<std::string, std::shared_ptr<VoxelModelMatrix>> voxelMatricesMap;
  }
  modelsData;

  struct GeometriesData {
    std::vector<std::shared_ptr<VoxelModelGeometry>> allVoxelGeometries;
    std::vector<std::shared_ptr<VoxelModelGeometryInstance>> allVoxelGeometryInstances;
  }
  geometriesData;

  VoxelGeometriesStackRenderer voxelGeometriesStackRenderer;

};
