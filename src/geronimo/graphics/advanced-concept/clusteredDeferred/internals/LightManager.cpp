
#include "LightManager.hpp"

#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/system/math/constants.hpp"

namespace {
  constexpr std::size_t k_maxSpotLights = 512;
}

void LightManager::initialize(const gero::graphics::PerspectiveClustering::Def& clusterDef)
{
  _cluster.initialize(clusterDef);

  _allSpotLights.reserve(k_maxSpotLights);
  _spotLightsBuffer.reserve(k_maxSpotLights);

  const uint32_t k_clusterDataSize = clusterDef.clusterSliceX * clusterDef.clusterSliceY * clusterDef.clusterSliceZ;

  {
    const float* dataPointer = nullptr;
    _clusterDataTexture.allocate(dataPointer, 0, { k_clusterDataSize, 1U + clusterDef.maxLightsPerCluster });
  }

  {
    const glm::vec4* dataPointer = nullptr;
    _spotLightsTexture.allocate(dataPointer, 0, { uint32_t(k_maxSpotLights), 1 });
  }
}

void LightManager::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  if (_allSpotLights.size() < k_maxSpotLights)
    _allSpotLights.push_back({inPosition, inRadius});
}

void LightManager::computeDataTextures(const gero::graphics::Camera& inSceneCamera) {

  {
    _cluster.computeCluster(inSceneCamera, _allSpotLights);

    const auto& def = _cluster.getDefinition();
    const uint32_t k_clusterDataSize = def.clusterSliceX * def.clusterSliceY * def.clusterSliceZ;

    _clusterDataTexture.update(_cluster.getDataBuffer(), {0, 0}, { k_clusterDataSize, 1U + def.maxLightsPerCluster });
  }

  {
    _spotLightsBuffer.clear();

    if (!_allSpotLights.empty())
    {

      for (const auto& currSpotLight : _allSpotLights)
      {
        _spotLightsBuffer.push_back({
          currSpotLight.position.x,
          currSpotLight.position.y,
          currSpotLight.position.z,
          currSpotLight.radius
        });
      }

      _spotLightsTexture.update(_spotLightsBuffer, {0, 0}, { uint32_t(_spotLightsBuffer.size()), 1U });

    }

  }

}

void LightManager::reset() { _allSpotLights.clear(); }

const gero::graphics::DataTexture& LightManager::getClusterDataTexture() const
{
  return _clusterDataTexture;
}

const gero::graphics::DataTexture& LightManager::getLightsDataTexture() const
{
  return _spotLightsTexture;
}

float LightManager::getZStride() const
{
  return _cluster.getZStride();
}

