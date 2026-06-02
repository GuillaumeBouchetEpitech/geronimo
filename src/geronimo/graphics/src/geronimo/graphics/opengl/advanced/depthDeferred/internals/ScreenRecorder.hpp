
#pragma once

#include "geronimo/graphics/opengl/FrameBuffer.hpp"
#include "geronimo/graphics/opengl/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {
namespace depthDeferred {

class ScreenRecorder {
public:
  ScreenRecorder() = default;
  ~ScreenRecorder() = default;

public:
  void resize(const glm::ivec2& inFrameSize);

  void startRecording();
  void stopRecording();

public:
  const gero::graphics::opengl::Texture& getColorTexture() const;
  const gero::graphics::opengl::Texture& getPositionTexture() const;
  const gero::graphics::opengl::Texture& getNormalTexture() const;
  const gero::graphics::opengl::Texture& getDepthTexture() const;

private:
  glm::ivec2 _frameSize = {0, 0};

  gero::graphics::opengl::Texture _colorTexture;
  gero::graphics::opengl::Texture _positionTexture;
  gero::graphics::opengl::Texture _normalTexture;
  gero::graphics::opengl::Texture _depthTexture;
  gero::graphics::opengl::FrameBuffer _frameBufferGeometries;
};

} // namespace depthDeferred
} // namespace opengl
} // namespace advanced
} // namespace graphics
} // namespace gero
