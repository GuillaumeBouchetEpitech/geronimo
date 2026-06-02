
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
namespace depthDeferred {

struct PostProcessData {

  gero::graphics::opengl::Texture depthTexture;
  gero::graphics::opengl::Texture colorTexture;
  gero::graphics::opengl::FrameBuffer frameBuffer;
  std::shared_ptr<gero::graphics::opengl::ShaderProgram> shader; // TODO: unecessary shader instance duplication
  gero::graphics::opengl::Geometry geometry;

  void initialize(const glm::ivec2& inFrameSize,
                  gero::graphics::opengl::ShaderProgram::Definition inShaderDef,
                  gero::graphics::opengl::GeometryBuilder& geometryBuilder);

  void resize(const glm::ivec2& inFrameSize);
};

class LightStackRenderer {
public:
  LightStackRenderer();
  ~LightStackRenderer();

public:
  void initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

public:
  void pushSpotLight(const glm::vec3& inPosition, const glm::vec3& inColor, float inRadius);

public:
  void flush(const glm::vec3& eyePos,
             const glm::mat4& composedMatrix,
             const gero::graphics::opengl::Texture& positionTexture,
             const gero::graphics::opengl::Texture& normalTexture);

  void reset();

private:
  void _flushDiffuse(const glm::mat4& composedMatrix,
                     const gero::graphics::opengl::Texture& positionTexture,
                     const gero::graphics::opengl::Texture& normalTexture);

  void _flushSpecular(const glm::vec3& eyePos,
                      const glm::mat4& composedMatrix,
                      const gero::graphics::opengl::Texture& positionTexture,
                      const gero::graphics::opengl::Texture& normalTexture);

public:
  const gero::graphics::opengl::Texture& getDiffuseCoefTexture();
  const gero::graphics::opengl::Texture& getSpecularCoefTexture();

  const gero::graphics::opengl::Texture& getDiffuseColorTexture();
  const gero::graphics::opengl::Texture& getSpecularColorTexture();

private:
  glm::ivec2 _frameSize = {0, 0};

  PostProcessData _diffuse;
  PostProcessData _specular;

  struct Instance {
    glm::vec3 position;
    glm::vec3 color;
    float radius;
  };

  std::vector<Instance> _vertices;
};

} // namespace depthDeferred
} // namespace opengl
} // namespace advanced
} // namespace graphics
} // namespace gero
