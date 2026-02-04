
#include "DepthDeferred.hpp"

namespace gero {
namespace graphics {

void DepthDeferred::initialize(const std::string& inRootPath,
                               const glm::ivec2& inFrameSize,
                               float inResolutionScaling /*= 1.0f*/) {

  _sunLight = glm::vec4(glm::normalize(glm::vec3(_sunLight)), 0.0f);

  _lightStackRenderer.initialize(inRootPath, inFrameSize);

  _resultQuadRenderer.initialize(inRootPath, inFrameSize);

  _frameMerger.initialize(inRootPath, inFrameSize);

  resize(inFrameSize, inResolutionScaling);
}

void DepthDeferred::resize(const glm::ivec2& inFrameSize, float inResolutionScaling /*= 1.0f*/) {

  _resolutionScaling = inResolutionScaling;

  const glm::ivec2 downScaledFrame = glm::ivec2(glm::vec2(inFrameSize) * _resolutionScaling);

  _screenRecorder.resize(downScaledFrame);
  _screenRecorder_transparency.resize(downScaledFrame);
  _lightStackRenderer.resize(downScaledFrame);
  _resultQuadRenderer.resize(inFrameSize);
  _frameMerger.resize(inFrameSize);
}

void DepthDeferred::setEyePosition(const glm::vec3& inEyePos) { _eyePos = inEyePos; }

void DepthDeferred::setSunLightDirection(const glm::vec3& inSunLightDirection) {
  _sunLight = glm::vec4(glm::normalize(glm::vec3(inSunLightDirection)), 0.0f);
}
void DepthDeferred::setSunLightPosition(const glm::vec3& inSunLightPosition) {
  _sunLight = glm::vec4(inSunLightPosition, 1.0f);
}

void DepthDeferred::setAmbientLightRatio(float ambientLightRatio) { _ambientLightRatio = ambientLightRatio; }

void DepthDeferred::startRecording() { _screenRecorder.startRecording(); }
void DepthDeferred::stopRecording() { _screenRecorder.stopRecording(); }

void DepthDeferred::startRecordingTransparency() { _screenRecorder_transparency.startRecording(); }
void DepthDeferred::stopRecordingTransparency() { _screenRecorder_transparency.stopRecording(); }

void DepthDeferred::pushSpotLight(const glm::vec3& inPosition, const glm::vec3& inColor, float inRadius) {
  _lightStackRenderer.pushSpotLight(inPosition, inColor, inRadius);
}

// void DepthDeferred::applySpotLights(const gero::graphics::ICamera& inSceneCamera, bool resetLightStack /*= true*/) {
//   _lightStackRenderer.flush(_eyePos,
//                             inSceneCamera.getMatricesData().composed,
//                             _screenRecorder.getPositionTexture(),
//                             _screenRecorder.getNormalTexture());

//   if (resetLightStack) {
//     _lightStackRenderer.reset();
//   }
// }

void DepthDeferred::renderHudQuad(const gero::graphics::ICamera& inSceneCamera,
                                  const gero::graphics::ICamera& inHudCamera) {

  _lightStackRenderer.flush(_eyePos,
                            inSceneCamera.getMatricesData().composed,
                            _screenRecorder.getPositionTexture(),
                            _screenRecorder.getNormalTexture());

  // TODO: capture this in a texture (color+depth)
  _frameMerger.startRecordingOpaque();
  _resultQuadRenderer.render(_eyePos,
                             _sunLight,
                             inHudCamera.getMatricesData().composed,
                             _screenRecorder.getColorTexture(),
                             _screenRecorder.getPositionTexture(),
                             _screenRecorder.getNormalTexture(),
                             _screenRecorder.getDepthTexture(),
                             _lightStackRenderer.getDiffuseColorTexture(),
                             _lightStackRenderer.getSpecularColorTexture(),
                             _ambientLightRatio);
  _frameMerger.stopRecordingOpaque();

  _lightStackRenderer.flush(_eyePos,
                            inSceneCamera.getMatricesData().composed,
                            _screenRecorder_transparency.getPositionTexture(),
                            _screenRecorder_transparency.getNormalTexture());

  // // TODO: capture this in a texture (color+depth)
  _frameMerger.startRecordingTransparency();
  _resultQuadRenderer.render(_eyePos,
                             _sunLight,
                             inHudCamera.getMatricesData().composed,
                             _screenRecorder_transparency.getColorTexture(),
                             _screenRecorder_transparency.getPositionTexture(),
                             _screenRecorder_transparency.getNormalTexture(),
                             _screenRecorder_transparency.getDepthTexture(),
                             _lightStackRenderer.getDiffuseColorTexture(),
                             _lightStackRenderer.getSpecularColorTexture(),
                             _ambientLightRatio);
  _frameMerger.stopRecordingTransparency();

  // TODO: merge the 2 captured textures -> color/depth x2 (opaque/transparent)
  _frameMerger.render(inHudCamera.getMatricesData().composed);

  _lightStackRenderer.reset();
}

float DepthDeferred::getResolutionScaling() const { return _resolutionScaling; }

} // namespace graphics
} // namespace gero
