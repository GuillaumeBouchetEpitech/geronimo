
#pragma once

#include "geronimo/graphics/opengl/FrameBuffer.hpp"
#include "geronimo/graphics/opengl/Geometry.hpp"
#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/ShaderProgram.hpp"
#include "geronimo/graphics/opengl/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {
namespace depthDeferredNoSpecular {

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
  gero::graphics::opengl::Texture depthTexture_opaque;
  gero::graphics::opengl::Texture colorTexture_opaque;
  gero::graphics::opengl::FrameBuffer frameBuffer_opaque;

  // capture transparent pass
  gero::graphics::opengl::Texture depthTexture_transparency;
  gero::graphics::opengl::Texture colorTexture_transparency;
  gero::graphics::opengl::FrameBuffer frameBuffer_transparency;

  // merge transparent texture into opaque texture
  std::shared_ptr<gero::graphics::opengl::ShaderProgram> quadShader;
  gero::graphics::opengl::Geometry quadGeometry;
};

} // namespace depthDeferredNoSpecular
} // namespace opengl
} // namespace advanced
} // namespace graphics
} // namespace gero
