
#pragma once

#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace gero {
namespace graphics {
namespace depthDeferred {

class FrameMerger {
public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize);

  void resize(const glm::ivec2& inFrameSize);

  void startRecordingOpaque();
  void stopRecordingOpaque();
  void startRecordingTransparency();
  void stopRecordingTransparency();

  void render(const glm::mat4& composedMatrix);

public:
  glm::ivec2 frameSize = {0, 0};

  // capture opaque pass
  gero::graphics::Texture depthTexture_opaque;
  gero::graphics::Texture colorTexture_opaque;
  gero::graphics::FrameBuffer frameBuffer_opaque;

  // capture transparent pass
  gero::graphics::Texture depthTexture_transparency;
  gero::graphics::Texture colorTexture_transparency;
  gero::graphics::FrameBuffer frameBuffer_transparency;

  // merge transparent texture into opaque texture
  std::shared_ptr<gero::graphics::ShaderProgram> quadShader;
  gero::graphics::Geometry quadGeometry;
};

} // namespace depthDeferred
} // namespace graphics
} // namespace gero
