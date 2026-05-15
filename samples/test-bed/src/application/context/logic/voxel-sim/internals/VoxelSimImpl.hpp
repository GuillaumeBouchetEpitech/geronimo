
#include "../AbstractVoxelSim.hpp"

// #include "geronimo/system/trees/bvh/BvhTree.hpp"
// #include "geronimo/system/trees/kd/SpatialIndexer.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include <unordered_map>

// forward declaration
class VoxelSimImpl;

struct TrimeshVertex {
  glm::vec3 pos;
  glm::vec3 norm;
};

struct VoxelShape {
  std::string name;
  uint32_t alias;
  glm::ivec3 extrusionAxis = glm::ivec3(0,0,0);
  glm::uvec3 gridSize = glm::uvec3(1,1,1);

  // TODO: store the +x/-x/+y/-y/+z/-z faces? -> easy discard when modelling? (can rely on the normals)

  std::vector<TrimeshVertex> vertices;

  void render(const glm::vec3& position) const;
};

struct VoxelModelMatrix {
  glm::uvec3 gridSize = glm::uvec3(1,1,1);
  std::vector<uint32_t> values;

  uint32_t getValue(int32_t x, int32_t y, int32_t z) const;
  uint32_t getValue(const glm::ivec3& cursor) const;

  void render(const VoxelSimImpl& voxelSimImpl) const;
};

struct VoxelModelGeometry {

  std::vector<TrimeshVertex> vertices;

  void build(const VoxelSimImpl& inVoxelSimImpl, const VoxelModelMatrix& inMatrix);
  void render(const glm::vec3& position) const;
};

class VoxelSimImpl : public AbstractVoxelSim {
public:
  VoxelSimImpl();
  ~VoxelSimImpl() = default;

private:
  void _loadJsonFile(const std::string_view inFilepath);

public:
  void update(float elapsedTime) override;
  void render() override;

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
