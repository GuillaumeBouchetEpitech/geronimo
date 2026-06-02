
#pragma once

#include "geronimo/graphics/opengl/Geometry.hpp"
#include "geronimo/graphics/opengl/GlContext.hpp"
#include "geronimo/graphics/opengl/ShaderProgram.hpp"
#include "geronimo/graphics/opengl/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {

namespace depthDeferredNoSpecular {

class ResultQuadRenderer {

public:
  ResultQuadRenderer();
  ~ResultQuadRenderer();

public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

  void render( // const glm::vec3& eyePos,
    const glm::vec4& sunLightDirection,
    const glm::mat4& composedMatrix,
    const gero::graphics::opengl::Texture& colorTexture,
    const gero::graphics::opengl::Texture& positionTexture,
    const gero::graphics::opengl::Texture& normalTexture,
    const gero::graphics::opengl::Texture& depthTexture,
    const gero::graphics::opengl::Texture& diffuseCoefTexture,
    // const gero::graphics::opengl::Texture& specularCoefTexture,
    float ambientLightCoef);

private:
  glm::ivec2 _frameSize = {0, 0};

  std::shared_ptr<gero::graphics::opengl::ShaderProgram> _quadShader;
  gero::graphics::opengl::Geometry _quadGeometry;
};

} // namespace depthDeferredNoSpecular
} // namespace advanced
} // namespace opengl
} // namespace graphics
} // namespace gero
