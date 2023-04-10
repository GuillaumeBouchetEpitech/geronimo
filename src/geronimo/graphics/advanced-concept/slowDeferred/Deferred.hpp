
#pragma once

#include "internals/LightStackRenderer.hpp"
#include "internals/ResultQuadRenderer.hpp"
#include "internals/ScreenRecorder.hpp"

#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

class Deferred {
public:
  Deferred() = default;
  ~Deferred() = default;

public:
  void initialize(const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

  void setEyePosition(const glm::vec3& inEyePos);
  void setSunLightDirection(const glm::vec3& inSunLightDirection);
  void setAmbiantLightCoef(float ambiantLightCoef);

  void startRecording();
  void stopRecording();

  void pushSpotLight(const glm::vec3& inPos, float inRadius);
  void applySpotLights(const glm::mat4& composedMatrix);

  void renderHudQuad(const glm::mat4& composedMatrix);

private:
  ScreenRecorder _screenRecorder;
  ResultQuadRenderer _resultQuadRenderer;
  LightStackRenderer _lightStackRenderer;

  glm::vec3 _eyePos;
  glm::vec3 _sunLightDirection = {-1.0f, -1.0f, -2.0f};
  float _ambiantLightCoef = 0.1f;
};
