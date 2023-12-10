
#pragma once

#include "internals/LightStackRenderer.hpp"
#include "internals/ResultQuadRenderer.hpp"
#include "internals/ScreenRecorder.hpp"

#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

namespace gero {
namespace graphics {

class SlowerDeferred {
public:
  SlowerDeferred() = default;
  ~SlowerDeferred() = default;

public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize, float inResolutionScaling = 1.0f);

public:
  void resize(const glm::ivec2& inFrameSize, float inResolutionScaling = 1.0f);

  void setEyePosition(const glm::vec3& inEyePos);
  void setSunLightDirection(const glm::vec3& inSunLightDirection);
  void setAmbientLightRatio(float ambientLightRatio);

  void startRecording();
  void stopRecording();

  void pushSpotLight(const glm::vec3& inPos, float inRadius);
  void applySpotLights(const gero::graphics::ICamera& inSceneCamera);

  void renderHudQuad(const gero::graphics::ICamera& inHudCamera);

public:
  float getResolutionScaling() const;

private:
  slowDeferred::ScreenRecorder _screenRecorder;
  slowDeferred::LightStackRenderer _lightStackRenderer;
  slowDeferred::ResultQuadRenderer _resultQuadRenderer;

  glm::vec3 _eyePos;
  glm::vec3 _sunLightDirection = {-1.0f, -1.0f, -2.0f};
  float _ambientLightRatio = 0.1f;
  float _resolutionScaling = 1.0f;
};

} // namespace graphics
} // namespace gero
