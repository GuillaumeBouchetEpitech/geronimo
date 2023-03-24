
#pragma once

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <memory>

class ResultQuadRenderer {

public:
  ResultQuadRenderer();
  ~ResultQuadRenderer();

public:
  void initialize(const glm::ivec2& inFrameSize);

public:
  void resize(const glm::ivec2& inFrameSize);

  void render(const glm::vec3& eyePos,
              const glm::vec3& sunLightDirection,
              const glm::mat4& composedMatrix,
              const gero::graphics::Texture& colorTexture,
              const gero::graphics::Texture& positionTexture,
              const gero::graphics::Texture& normalTexture,
              const gero::graphics::Texture& diffuseCoefTexture,
              const gero::graphics::Texture& specularCoefTexture,
              float ambiantLightCoef);

private:
  glm::ivec2 _frameSize = {0, 0};

  std::shared_ptr<gero::graphics::ShaderProgram> _quadShader;
  gero::graphics::Geometry _quadGeometry;
};
