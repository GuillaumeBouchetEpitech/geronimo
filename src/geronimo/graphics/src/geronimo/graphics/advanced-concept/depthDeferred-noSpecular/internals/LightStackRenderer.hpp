
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
namespace depthDeferredNoSpecular {

struct PostProcessData {

  gero::graphics::Texture depthTexture;
  gero::graphics::Texture colorTexture;
  gero::graphics::FrameBuffer frameBuffer;
  std::shared_ptr<gero::graphics::ShaderProgram> shader; // TODO: unecessary shader instance duplication
  gero::graphics::Geometry geometry;

  void initialize(const glm::ivec2& inFrameSize,
                  gero::graphics::ShaderProgram::Definition inShaderDef,
                  gero::graphics::GeometryBuilder& geometryBuilder);

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
  void flush( // const glm::vec3& eyePos,
    const glm::mat4& composedMatrix,
    const gero::graphics::Texture& positionTexture,
    const gero::graphics::Texture& normalTexture);

  void reset();

private:
  void _flushDiffuse(const glm::mat4& composedMatrix,
                     const gero::graphics::Texture& positionTexture,
                     const gero::graphics::Texture& normalTexture);

  // void _flushSpecular(const glm::vec3& eyePos,
  //                     const glm::mat4& composedMatrix,
  //                     const gero::graphics::Texture& positionTexture,
  //                     const gero::graphics::Texture& normalTexture);

public:
  const gero::graphics::Texture& getDiffuseCoefTexture();
  // const gero::graphics::Texture& getSpecularCoefTexture();

  const gero::graphics::Texture& getDiffuseColorTexture();
  // const gero::graphics::Texture& getSpecularColorTexture();

private:
  glm::ivec2 _frameSize = {0, 0};

  PostProcessData _diffuse;
  // PostProcessData _specular;

  struct Instance {
    glm::vec3 position;
    glm::vec3 color;
    float radius;
  };

  std::vector<Instance> _vertices;
};

} // namespace depthDeferredNoSpecular
} // namespace graphics
} // namespace gero
