
#pragma once

#include "Camera.hpp"

#include <vector>

namespace gero {
namespace graphics {


class PerspectiveClustering {

public:
  struct Def {
    uint32_t clusterSliceX = 15;
    uint32_t clusterSliceY = 15;
    uint32_t clusterSliceZ = 15;
    uint32_t maxLightsPerCluster = 90;
  };

public:

  struct SpotLight {
    glm::vec3 position;
    float radius;
  };

public:
  PerspectiveClustering() = default;
  ~PerspectiveClustering() = default;

public:
  void initialize(const Def& inDef);

public:
  void resetClustersCount();
  void computeCluster(const Camera& inCamera, const std::vector<SpotLight>& inLights);

public:
  const std::vector<float>& getDataBuffer() const;

  int32_t getClusterSize(int32_t inX, int32_t inY, int32_t inZ) const;

  float getZStride() const;

  const Def& getDefinition() const;

private:
  std::size_t _getClusterId(int32_t inX, int32_t inY, int32_t inZ) const;

private:
  Def _def;
  std::vector<float> _dataBuffer;

  float _zStride = 0.0f;

};


}
}

