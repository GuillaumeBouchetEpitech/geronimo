
#include "Deferred.hpp"

// #include "geronimo/graphics/GlContext.hpp"

// using namespace gero::graphics;
// using TexQuality = gero::graphics::Texture::Quality;
// using TexPattern = gero::graphics::Texture::Pattern;
// using DepthFormat = gero::graphics::Texture::DepthFormat;
// using DepthType = gero::graphics::Texture::DepthType;
// using namespace gero::graphics::GlContext;





void Deferred::initialize(const glm::ivec2& inFrameSize)
{
  _sunLightDirection = glm::normalize(_sunLightDirection);

  _resultQuadRenderer.initialize(inFrameSize);
  _lightStackRenderer.initialize(inFrameSize);

  resize(inFrameSize);
}

void Deferred::resize(const glm::ivec2& inFrameSize)
{
  _screenRecorder.resize(inFrameSize);
  _resultQuadRenderer.resize(inFrameSize);
  _lightStackRenderer.resize(inFrameSize);
}

void Deferred::setEyePosition(const glm::vec3& inEyePos)
{
  _eyePos = inEyePos;
}

void Deferred::setSunLightDirection(const glm::vec3& inSunLightDirection)
{
  _sunLightDirection = glm::normalize(inSunLightDirection);
}

void Deferred::setAmbiantLightCoef(float ambiantLightCoef)
{
  _ambiantLightCoef = ambiantLightCoef;
}

void Deferred::startRecording()
{
  _screenRecorder.startRecording();
}

void Deferred::stopRecording()
{
  _screenRecorder.stopRecording();
}

void Deferred::pushSpotLight(const glm::vec3& inPosition, float inRadius)
{
  _lightStackRenderer.pushSpotLight(inPosition, inRadius);
}

void Deferred::applySpotLights(const glm::mat4& composedMatrix)
{
  _lightStackRenderer.flush(
    _eyePos,
    composedMatrix,
    _screenRecorder.getPositionTexture(),
    _screenRecorder.getNormalTexture()
  );
}

void Deferred::renderHudQuad(const glm::mat4& composedMatrix)
{
  _resultQuadRenderer.render(
    _eyePos,
    _sunLightDirection,
    composedMatrix,
    _screenRecorder.getColorTexture(),
    _screenRecorder.getPositionTexture(),
    _screenRecorder.getNormalTexture(),
    _screenRecorder.getDepthTexture(),
    _lightStackRenderer.getDiffuseCoefTexture(),
    _lightStackRenderer.getSpecularCoefTexture(),
    _ambiantLightCoef
  );
}

