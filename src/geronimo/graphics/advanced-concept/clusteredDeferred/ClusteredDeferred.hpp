
#pragma once

#include "internals/LightManager.hpp"
#include "internals/ResultQuadRenderer.hpp"
#include "internals/ScreenRecorder.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

namespace gero {
namespace graphics {

class IClusteredDeferred {
public:
  virtual ~IClusteredDeferred() = default;

public:
  virtual void resize(const glm::ivec2& inFrameSize) = 0;

  virtual void setEyePosition(const glm::vec3& inEyePos) = 0;
  virtual void setSunLightDirection(const glm::vec3& inSunLightDirection) = 0;
  virtual void setAmbiantLightCoef(float ambiantLightCoef) = 0;

  virtual void startRecording() = 0;
  virtual void stopRecording() = 0;

  virtual void pushSpotLight(const glm::vec3& inPos, float inRadius) = 0;
  virtual void applySpotLights(const gero::graphics::ICamera& inSceneCamera) = 0;

  virtual void renderHudQuad(const gero::graphics::ICamera& inSceneCamera, const gero::graphics::ICamera& inHudCamera) = 0;
};


class ClusteredDeferred : public IClusteredDeferred, public gero::NonCopyable {
public:
  ClusteredDeferred() = default;
  ~ClusteredDeferred() = default;

public:
  void initialize(const std::string& inRootPath,
                  const gero::graphics::PerspectiveClustering::Def& inClusterDef,
                  const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize) override;

  void setEyePosition(const glm::vec3& inEyePos) override;
  void setSunLightDirection(const glm::vec3& inSunLightDirection) override;
  void setAmbiantLightCoef(float ambiantLightCoef) override;

  void startRecording() override;
  void stopRecording() override;

  void pushSpotLight(const glm::vec3& inPos, float inRadius) override;
  void applySpotLights(const gero::graphics::ICamera& inSceneCamera) override;

  void renderHudQuad(const gero::graphics::ICamera& inSceneCamera, const gero::graphics::ICamera& inHudCamera) override;

private:
  gero::graphics::PerspectiveClustering::Def _clusterDef;

  ScreenRecorder _screenRecorder;
  LightManager _lightManager;
  ResultQuadRenderer _resultQuadRenderer;

  glm::vec3 _eyePos;
  glm::vec3 _sunLightDirection;
  float _ambiantLightCoef = 0.1f;
};

} // namespace graphics
} // namespace gero
