
#pragma once

#include "geronimo/graphics/DataTexture.hpp"
#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/graphics/camera/PerspectiveClustering.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>

namespace gero {
namespace graphics {

class ResultQuadRenderer {

public:
  ResultQuadRenderer();
  ~ResultQuadRenderer();

public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

  void render(const glm::vec3& sunLightDirection,
              const gero::graphics::Camera& inSceneCamera,
              const gero::graphics::Camera& inHudCamera,
              const gero::graphics::PerspectiveClustering::Def& clusterDef,
              const gero::graphics::Texture& colorTexture,
              const gero::graphics::Texture& positionTexture,
              const gero::graphics::Texture& normalTexture,
              const gero::graphics::Texture& depthTexture,
              const gero::graphics::DataTexture& clusterDataTexture,
              const gero::graphics::DataTexture& lightsDataTexture,
              float inZStride,
              float ambiantLightCoef);

private:
  glm::ivec2 _frameSize = {0, 0};

  std::shared_ptr<gero::graphics::ShaderProgram> _quadShader;
  gero::graphics::Geometry _quadGeometry;
};

} // namespace graphics
} // namespace gero
