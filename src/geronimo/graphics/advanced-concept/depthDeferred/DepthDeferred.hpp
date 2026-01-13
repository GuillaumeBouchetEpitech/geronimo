
#pragma once

#include "internals/LightStackRenderer.hpp"
#include "internals/ResultQuadRenderer.hpp"
#include "internals/ScreenRecorder.hpp"
#include "internals/FrameMerger.hpp"

#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

namespace gero {
namespace graphics {

class DepthDeferred {
public:
  DepthDeferred() = default;
  ~DepthDeferred() = default;

public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize, float inResolutionScaling = 1.0f);

public:
  void resize(const glm::ivec2& inFrameSize, float inResolutionScaling = 1.0f);

  void setEyePosition(const glm::vec3& inEyePos);
  void setSunLightDirection(const glm::vec3& inSunLightDirection);
  void setSunLightPosition(const glm::vec3& inSunLightPosition);
  void setAmbientLightRatio(float ambientLightRatio);

  void startRecording();
  void stopRecording();
  void startRecordingTransparency();
  void stopRecordingTransparency();

  void pushSpotLight(const glm::vec3& inPos, const glm::vec3& inColor, float inRadius);

  // void applySpotLights(const gero::graphics::ICamera& inSceneCamera, bool resetLightStack = true);

  void renderHudQuad(const gero::graphics::ICamera& inSceneCamera, const gero::graphics::ICamera& inHudCamera);

public:
  float getResolutionScaling() const;

private:
  depthDeferred::ScreenRecorder _screenRecorder;
  depthDeferred::ScreenRecorder _screenRecorder_transparency;

  depthDeferred::LightStackRenderer _lightStackRenderer;

  depthDeferred::ResultQuadRenderer _resultQuadRenderer;

  depthDeferred::FrameMerger _frameMerger;

  glm::vec3 _eyePos;
  glm::vec4 _sunLight = {-1.0f, -1.0f, -2.0f, 0.0f}; // w=0 -> means directional light
  float _ambientLightRatio = 0.1f;
  float _resolutionScaling = 1.0f;
};

} // namespace graphics
} // namespace gero
