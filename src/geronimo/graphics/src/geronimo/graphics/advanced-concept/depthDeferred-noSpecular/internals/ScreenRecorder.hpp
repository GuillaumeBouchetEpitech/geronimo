
#pragma once

#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

namespace gero {
namespace graphics {

namespace depthDeferredNoSpecular {

class ScreenRecorder {
public:
  ScreenRecorder() = default;
  ~ScreenRecorder() = default;

public:
  void resize(const glm::ivec2& inFrameSize);

  void startRecording();
  void stopRecording();

public:
  const gero::graphics::Texture& getColorTexture() const;
  const gero::graphics::Texture& getPositionTexture() const;
  const gero::graphics::Texture& getNormalTexture() const;
  const gero::graphics::Texture& getDepthTexture() const;

private:
  glm::ivec2 _frameSize = {0, 0};

  gero::graphics::Texture _colorTexture;
  gero::graphics::Texture _positionTexture;
  gero::graphics::Texture _normalTexture;
  gero::graphics::Texture _depthTexture;
  gero::graphics::FrameBuffer _frameBufferGeometries;
};

} // namespace depthDeferredNoSpecular

} // namespace graphics
} // namespace gero
