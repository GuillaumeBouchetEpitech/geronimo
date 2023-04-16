
#include "ClusteredDeferred.hpp"

namespace gero {
namespace graphics {

void ClusteredDeferred::initialize(const std::string& inRootPath, const gero::graphics::PerspectiveClustering::Def& inClusterDef, const glm::ivec2& inFrameSize) {

  _clusterDef = inClusterDef;

  _lightManager.initialize(_clusterDef);
  _sunLightDirection = glm::normalize(_sunLightDirection);
  _resultQuadRenderer.initialize(inRootPath, inFrameSize);

  resize(inFrameSize);

  setSunLightDirection(glm::vec3(-1.0f, -1.0f, -2.0f));
}

void ClusteredDeferred::resize(const glm::ivec2& inFrameSize) {
  _screenRecorder.resize(inFrameSize);
  _resultQuadRenderer.resize(inFrameSize);
}

void ClusteredDeferred::setEyePosition(const glm::vec3& inEyePos) { _eyePos = inEyePos; }

void ClusteredDeferred::setSunLightDirection(const glm::vec3& inSunLightDirection) {
  _sunLightDirection = glm::normalize(inSunLightDirection);
}

void ClusteredDeferred::setAmbiantLightCoef(float ambiantLightCoef) { _ambiantLightCoef = ambiantLightCoef; }

void ClusteredDeferred::startRecording() { _screenRecorder.startRecording(); }

void ClusteredDeferred::stopRecording() { _screenRecorder.stopRecording(); }

void ClusteredDeferred::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  _lightManager.pushSpotLight(inPosition, inRadius);
}

void ClusteredDeferred::applySpotLights(const gero::graphics::Camera& inSceneCamera)
{
  _lightManager.computeDataTextures(inSceneCamera);
  _lightManager.reset();
}

void ClusteredDeferred::renderHudQuad(const gero::graphics::Camera& inSceneCamera, const gero::graphics::Camera& inHudCamera) {

  _resultQuadRenderer.render(_sunLightDirection,
                             inSceneCamera,
                             inHudCamera,
                             _clusterDef,
                             _screenRecorder.getColorTexture(),
                             _screenRecorder.getPositionTexture(),
                             _screenRecorder.getNormalTexture(),
                             _screenRecorder.getDepthTexture(),
                             _lightManager.getClusterDataTexture(),
                             _lightManager.getLightsDataTexture(),
                             _lightManager.getZStride(),
                             _ambiantLightCoef);

}

}
}
