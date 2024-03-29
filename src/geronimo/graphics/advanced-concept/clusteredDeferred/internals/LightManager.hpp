
#pragma once

#include "PerspectiveClustering.hpp"

#include "geronimo/graphics/DataTexture.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace gero {
namespace graphics {

class LightManager {
public:
  LightManager() = default;
  ~LightManager() = default;

public:
  void initialize(const gero::graphics::PerspectiveClustering::Def& clusterDef);

public:
  void pushSpotLight(const glm::vec3& inPosition, float inRadius);

  void computeDataTextures(const gero::graphics::ICamera& inSceneCamera);

public:
  void reset();

  const gero::graphics::DataTexture& getClusterDataTexture() const;
  const gero::graphics::DataTexture& getLightsDataTexture() const;

  float getZStride() const;

private:
  gero::graphics::PerspectiveClustering _cluster;

  std::vector<gero::graphics::PerspectiveClustering::SpotLight> _allSpotLights;

  std::vector<glm::vec4> _spotLightsBuffer;

  gero::graphics::DataTexture _clusterDataTexture;
  gero::graphics::DataTexture _spotLightsTexture;
};

} // namespace graphics
} // namespace gero
