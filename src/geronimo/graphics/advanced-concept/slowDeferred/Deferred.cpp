
#include "Deferred.hpp"

namespace gero {
namespace graphics {

void Deferred::initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize, float inResolutionScaling /*= 1.0f*/) {
  _sunLightDirection = glm::normalize(_sunLightDirection);

  _resultQuadRenderer.initialize(inRootPath, inFrameSize);
  _lightStackRenderer.initialize(inRootPath, inFrameSize);

  resize(inFrameSize, inResolutionScaling);
}

void Deferred::resize(const glm::ivec2& inFrameSize, float inResolutionScaling /*= 1.0f*/) {

  _resolutionScaling = inResolutionScaling;

  const glm::ivec2 downScaledFrame = glm::ivec2(glm::vec2(inFrameSize) * _resolutionScaling);

  _screenRecorder.resize(downScaledFrame);
  _lightStackRenderer.resize(downScaledFrame);
  _resultQuadRenderer.resize(inFrameSize);
}

void Deferred::setEyePosition(const glm::vec3& inEyePos) { _eyePos = inEyePos; }

void Deferred::setSunLightDirection(const glm::vec3& inSunLightDirection) {
  _sunLightDirection = glm::normalize(inSunLightDirection);
}

void Deferred::setAmbiantLightCoef(float ambiantLightCoef) { _ambiantLightCoef = ambiantLightCoef; }

void Deferred::startRecording() { _screenRecorder.startRecording(); }

void Deferred::stopRecording() { _screenRecorder.stopRecording(); }

void Deferred::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  _lightStackRenderer.pushSpotLight(inPosition, inRadius);
}

void Deferred::applySpotLights(const glm::mat4& composedMatrix) {
  _lightStackRenderer.flush(
    _eyePos, composedMatrix, _screenRecorder.getPositionTexture(), _screenRecorder.getNormalTexture());
}

void Deferred::renderHudQuad(const glm::mat4& composedMatrix) {
  _resultQuadRenderer.render(_eyePos,
                             _sunLightDirection,
                             composedMatrix,
                             _screenRecorder.getColorTexture(),
                             _screenRecorder.getPositionTexture(),
                             _screenRecorder.getNormalTexture(),
                             _screenRecorder.getDepthTexture(),
                             _lightStackRenderer.getDiffuseCoefTexture(),
                             _lightStackRenderer.getSpecularCoefTexture(),
                             _ambiantLightCoef);
}

float Deferred::getResolutionScaling() const {
  return _resolutionScaling;
}

} // namespace graphics
} // namespace gero
