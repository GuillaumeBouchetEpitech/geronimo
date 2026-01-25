
#include "LightManager.hpp"

#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/system/math/constants.hpp"

namespace gero {
namespace graphics {

namespace {
constexpr std::size_t k_maxSpotLights = 512;
}

void LightManager::initialize(const gero::graphics::PerspectiveClustering::Def& clusterDef) {
  _cluster.initialize(clusterDef);

  _allSpotLights.reserve(k_maxSpotLights);
  _spotLightsBuffer.reserve(k_maxSpotLights);

  // const uint32_t k_clusterDataSize = clusterDef.clusterSliceX * clusterDef.clusterSliceY * clusterDef.clusterSliceZ;

  {
    const float* dataPointer = nullptr;
    // _clusterDataTexture.allocate(dataPointer, 0, { k_clusterDataSize, 1U + clusterDef.maxLightsPerCluster });

    const uint32_t k_clusterDataWidth = clusterDef.clusterSliceX * clusterDef.clusterSliceY;
    const uint32_t k_clusterDataHeight = clusterDef.clusterSliceZ * (1U + clusterDef.maxLightsPerCluster);

    _clusterDataTexture.allocate(dataPointer, 0, {k_clusterDataWidth, k_clusterDataHeight});
  }

  {
    const glm::vec4* dataPointer = nullptr;
    _spotLightsTexture.allocate(dataPointer, 0, {uint32_t(k_maxSpotLights), 1});
  }
}

void LightManager::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  if (_allSpotLights.size() < k_maxSpotLights)
    _allSpotLights.push_back({inPosition, inRadius});
}

void LightManager::computeDataTextures(const gero::graphics::ICamera& inSceneCamera) {

  if (_allSpotLights.empty())
    return;

  {
    _cluster.computeCluster(inSceneCamera, _allSpotLights);

    const auto& def = _cluster.getDefinition();

    // const uint32_t k_clusterDataSize = def.clusterSliceX * def.clusterSliceY * def.clusterSliceZ;

    // _clusterDataTexture.update(_cluster.getDataBuffer(), {0, 0}, { k_clusterDataSize, 1U + def.maxLightsPerCluster
    // });

    const uint32_t k_clusterDataWidth = def.clusterSliceX * def.clusterSliceY;
    // const uint32_t k_clusterDataHeight = def.clusterSliceZ * (1U + def.maxLightsPerCluster);

    const float* pDataPtr = _cluster.getDataBuffer().data();

    const auto& clusterDef = _cluster.getDefinition();
    for (int32_t zz = 0; zz < int32_t(clusterDef.clusterSliceZ); ++zz) {

      int32_t maxLightsCount = 0;
      for (int32_t yy = 0; yy < int32_t(clusterDef.clusterSliceY); ++yy)
        for (int32_t xx = 0; xx < int32_t(clusterDef.clusterSliceX); ++xx) {
          maxLightsCount = std::max(maxLightsCount, _cluster.getClusterSize(xx, yy, zz));
        }

      const float* pSubDataPtr = &(pDataPtr[uint32_t(zz) * k_clusterDataWidth * (1U + def.maxLightsPerCluster)]);
      const uint32_t dataLength = k_clusterDataWidth * uint32_t(1 + maxLightsCount);

      _clusterDataTexture.update(pSubDataPtr,
                                 dataLength,
                                 {0, uint32_t(zz) * (1U + def.maxLightsPerCluster)},
                                 {k_clusterDataWidth, uint32_t(1 + maxLightsCount)});
    }

    // _clusterDataTexture.update(_cluster.getDataBuffer(), {0, 0}, { k_clusterDataSize, k_clusterDataSize2 });
  }

  {
    _spotLightsBuffer.clear();

    for (const auto& currSpotLight : _allSpotLights) {
      _spotLightsBuffer.push_back(
        {currSpotLight.position.x, currSpotLight.position.y, currSpotLight.position.z, currSpotLight.radius});
    }

    _spotLightsTexture.update(_spotLightsBuffer, {0, 0}, {uint32_t(_spotLightsBuffer.size()), 1U});
  }
}

void LightManager::reset() { _allSpotLights.clear(); }

const gero::graphics::DataTexture& LightManager::getClusterDataTexture() const { return _clusterDataTexture; }

const gero::graphics::DataTexture& LightManager::getLightsDataTexture() const { return _spotLightsTexture; }

float LightManager::getZStride() const { return _cluster.getZStride(); }

} // namespace graphics
} // namespace gero
