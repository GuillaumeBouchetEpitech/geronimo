
#include "SlowerDeferred.hpp"

namespace gero {
namespace graphics {

void SlowerDeferred::initialize(const std::string& inRootPath,
                                const glm::ivec2& inFrameSize,
                                float inResolutionScaling /*= 1.0f*/) {
  _sunLightDirection = glm::normalize(_sunLightDirection);

  _resultQuadRenderer.initialize(inRootPath, inFrameSize);
  _lightStackRenderer.initialize(inRootPath, inFrameSize);

  resize(inFrameSize, inResolutionScaling);
}

void SlowerDeferred::resize(const glm::ivec2& inFrameSize, float inResolutionScaling /*= 1.0f*/) {

  _resolutionScaling = inResolutionScaling;

  const glm::ivec2 downScaledFrame = glm::ivec2(glm::vec2(inFrameSize) * _resolutionScaling);

  _screenRecorder.resize(downScaledFrame);
  _lightStackRenderer.resize(downScaledFrame);
  _resultQuadRenderer.resize(inFrameSize);
}

void SlowerDeferred::setEyePosition(const glm::vec3& inEyePos) { _eyePos = inEyePos; }

void SlowerDeferred::setSunLightDirection(const glm::vec3& inSunLightDirection) {
  _sunLightDirection = glm::normalize(inSunLightDirection);
}

void SlowerDeferred::setAmbientLightRatio(float ambientLightRatio) { _ambientLightRatio = ambientLightRatio; }

void SlowerDeferred::startRecording() { _screenRecorder.startRecording(); }

void SlowerDeferred::stopRecording() { _screenRecorder.stopRecording(); }

void SlowerDeferred::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  _lightStackRenderer.pushSpotLight(inPosition, inRadius);
}

void SlowerDeferred::applySpotLights(const gero::graphics::ICamera& inSceneCamera) {
  _lightStackRenderer.flush(_eyePos,
                            inSceneCamera.getMatricesData().composed,
                            _screenRecorder.getPositionTexture(),
                            _screenRecorder.getNormalTexture());
}

void SlowerDeferred::renderHudQuad(const gero::graphics::ICamera& inHudCamera) {
  _resultQuadRenderer.render(_eyePos,
                             _sunLightDirection,
                             inHudCamera.getMatricesData().composed,
                             _screenRecorder.getColorTexture(),
                             _screenRecorder.getPositionTexture(),
                             _screenRecorder.getNormalTexture(),
                             _screenRecorder.getDepthTexture(),
                             _lightStackRenderer.getDiffuseCoefTexture(),
                             _lightStackRenderer.getSpecularCoefTexture(),
                             _ambientLightRatio);
}

float SlowerDeferred::getResolutionScaling() const { return _resolutionScaling; }

} // namespace graphics
} // namespace gero
