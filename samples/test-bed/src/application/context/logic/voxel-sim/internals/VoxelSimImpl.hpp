
#include "../AbstractVoxelSim.hpp"

// #include "geronimo/system/trees/bvh/BvhTree.hpp"
// #include "geronimo/system/trees/kd/SpatialIndexer.hpp"


#include "./voxel-manager/VoxelManager.hpp"

// #include "./TrimeshVertex.hpp"
// #include "./VoxelShape.hpp"
// #include "./VoxelModelMatrix.hpp"
// #include "./VoxelModelGeometry.hpp"

// #include "geronimo/helpers/GLMath.hpp"

// #include <string>
// #include <memory>
// #include <string_view>
// #include <vector>
// #include <unordered_map>

class VoxelSimImpl : public AbstractVoxelSim {
public:
  VoxelSimImpl();
  ~VoxelSimImpl() = default;

// private:
//   void _loadJsonFile(const std::string_view inFilepath);

public:
  void update(float elapsedTime) override;
  void render() override;

public:

  VoxelManager voxelManager;

  // struct ShapesData {
  //   std::vector<std::shared_ptr<VoxelShape>> _allVoxelShapes;
  //   std::unordered_map<std::string, std::shared_ptr<VoxelShape>> _voxelShapesMap;
  //   std::unordered_map<uint32_t, std::shared_ptr<VoxelShape>> _voxelShapesAliasMap;
  // }
  // shapesData;

  // struct ModelsData {
  //   std::vector<std::shared_ptr<VoxelModelMatrix>> allVoxelMatrices;
  //   std::unordered_map<std::string, std::shared_ptr<VoxelModelMatrix>> voxelMatricesMap;

  //   std::vector<std::shared_ptr<VoxelModelGeometry>> allVoxelGeometries;
  // }
  // modelsData;

};
