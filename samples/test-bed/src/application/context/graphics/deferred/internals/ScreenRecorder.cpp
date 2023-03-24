
#include "ScreenRecorder.hpp"

#include "geronimo/graphics/GlContext.hpp"

using namespace gero::graphics;
using TexQuality = gero::graphics::Texture::Quality;
using TexPattern = gero::graphics::Texture::Pattern;
using DepthFormat = gero::graphics::Texture::DepthFormat;
using DepthType = gero::graphics::Texture::DepthType;
using namespace gero::graphics::GlContext;

void ScreenRecorder::resize(const glm::ivec2& inFrameSize) {
  if (_frameSize == inFrameSize)
    return;

  _frameSize = inFrameSize;

  _colorTexture.allocateBlank(_frameSize, TexQuality::pixelated, TexPattern::clamped);
  _positionTexture.allocateFloatBlank(_frameSize, TexQuality::pixelated, TexPattern::clamped);
  _normalTexture.allocateBlank(_frameSize, TexQuality::pixelated, TexPattern::clamped);

  _depthTexture.allocateDepth(_frameSize, DepthFormat::depth16, DepthType::unsignedShort);
  // _depthTexture.allocateCompatibleDepth(_frameSize);

  gero::graphics::FrameBuffer::Definition def;
  def.colorTextures.push_back({0, &_colorTexture});
  def.colorTextures.push_back({1, &_positionTexture});
  def.colorTextures.push_back({2, &_normalTexture});
  def.depthTexture = &_depthTexture;
  _frameBufferGeometries.initialize(def);
}

void ScreenRecorder::startRecording() {
  _frameBufferGeometries.bind();

  GlContext::clearColor(0.3f, 0.3f, 0.3f, 1.0f);
  GlContext::clearDepth(1.0f);
  GlContext::clears(Buffers::color, Buffers::depth);
}

void ScreenRecorder::stopRecording() { FrameBuffer::unbind(); }

const gero::graphics::Texture& ScreenRecorder::getColorTexture() const { return _colorTexture; }

const gero::graphics::Texture& ScreenRecorder::getPositionTexture() const { return _positionTexture; }

const gero::graphics::Texture& ScreenRecorder::getNormalTexture() const { return _normalTexture; }

const gero::graphics::Texture& ScreenRecorder::getDepthTexture() const { return _depthTexture; }