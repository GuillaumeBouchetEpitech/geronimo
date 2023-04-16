
#pragma once

#include "internals/LightManager.hpp"
#include "internals/ResultQuadRenderer.hpp"
#include "internals/ScreenRecorder.hpp"

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace graphics {

class ClusteredDeferred {
public:
  ClusteredDeferred() = default;
  ~ClusteredDeferred() = default;

public:
  void initialize(const std::string& inRootPath, const gero::graphics::PerspectiveClustering::Def& inClusterDef, const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

  void setEyePosition(const glm::vec3& inEyePos);
  void setSunLightDirection(const glm::vec3& inSunLightDirection);
  void setAmbiantLightCoef(float ambiantLightCoef);

  void startRecording();
  void stopRecording();

  void pushSpotLight(const glm::vec3& inPos, float inRadius);
  void applySpotLights(const gero::graphics::Camera& inSceneCamera);

  void renderHudQuad(const gero::graphics::Camera& inSceneCamera, const gero::graphics::Camera& inHudCamera);

private:

  gero::graphics::PerspectiveClustering::Def _clusterDef;

  ScreenRecorder _screenRecorder;
  LightManager _lightManager;
  ResultQuadRenderer _resultQuadRenderer;

  glm::vec3 _eyePos;
  glm::vec3 _sunLightDirection;
  float _ambiantLightCoef = 0.1f;
};

}
}
