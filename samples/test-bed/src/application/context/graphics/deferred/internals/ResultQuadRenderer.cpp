
#include "ResultQuadRenderer.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

using namespace gero::graphics;
using namespace gero::graphics::GlContext;

ResultQuadRenderer::ResultQuadRenderer() {}

ResultQuadRenderer::~ResultQuadRenderer() {}

void ResultQuadRenderer::initialize(const glm::ivec2& inFrameSize) {

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::GeometryBuilder geometryBuilder;

  const std::string k_rootPath = "assets/graphics/shaders/deferred/resultQuadRenderer/";

  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "resultQuadRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "resultQuadRenderer.glsl.frag")
    .addAttribute("a_vertex_position")
    .addAttribute("a_vertex_uv")
    .addUniform("u_composedMatrix")
    .addUniform("u_colorTexture")
    .addUniform("u_positionTexture")
    .addUniform("u_normalTexture")
    .addUniform("u_depthTexture")
    .addUniform("u_diffuseCoefTexture")
    .addUniform("u_specularCoefTexture")
    .addUniform("u_ambiantCoef")
    .addUniform("u_viewPos")
    .addUniform("u_sunLightDirection");

  _quadShader = std::make_shared<gero::graphics::ShaderProgram>(shaderProgramBuilder.getDefinition());

  geometryBuilder.reset()
    .setShader(*_quadShader)
    .setPrimitiveType(Geometry::PrimitiveType::triangles)
    .addVbo()
    .addVboAttribute("a_vertex_position", Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertex_uv", Geometry::AttrType::Vec2f);

  _quadGeometry.initialize(*_quadShader, geometryBuilder.getDefinition());

  resize(inFrameSize);
}

void ResultQuadRenderer::resize(const glm::ivec2& inFrameSize) {

  if (_frameSize == inFrameSize)
    return;

  _frameSize = inFrameSize;

  //
  //

  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
  };

  const std::array<Vertex, 4> vertices = {{{{inFrameSize.x * 0.0f, inFrameSize.y * 0.0f, -0.5f}, {0.0f, 0.0f}},
                                           {{inFrameSize.x * 1.0f, inFrameSize.y * 0.0f, -0.5f}, {1.0f, 0.0f}},
                                           {{inFrameSize.x * 1.0f, inFrameSize.y * 1.0f, -0.5f}, {1.0f, 1.0f}},
                                           {{inFrameSize.x * 0.0f, inFrameSize.y * 1.0f, -0.5f}, {0.0f, 1.0f}}}};

  const std::array<uint32_t, 6> indices = {{0, 1, 2, 2, 3, 0}};

  std::vector<Vertex> buffer;
  buffer.reserve(indices.size());

  for (uint32_t index : indices)
    buffer.push_back(vertices[index]);

  _quadGeometry.updateBuffer(0, buffer);
  _quadGeometry.setPrimitiveCount(uint32_t(buffer.size()));
}

void ResultQuadRenderer::render(const glm::vec3& eyePos,
                                const glm::vec3& sunLightDirection,
                                const glm::mat4& composedMatrix,
                                const gero::graphics::Texture& colorTexture,
                                const gero::graphics::Texture& positionTexture,
                                const gero::graphics::Texture& normalTexture,
                                const gero::graphics::Texture& depthTexture,
                                const gero::graphics::Texture& diffuseCoefTexture,
                                const gero::graphics::Texture& specularCoefTexture,
                                float ambiantLightCoef) {

  _quadShader->bind();
  _quadShader->setUniform("u_composedMatrix", composedMatrix);
  _quadShader->setUniform("u_ambiantCoef", ambiantLightCoef);
  _quadShader->setUniform("u_viewPos", eyePos);
  _quadShader->setUniform("u_sunLightDirection", sunLightDirection);

  struct TextureData {
    std::string_view name;
    const gero::graphics::Texture& texture;
  };

  const std::array<TextureData, 6> allTextures = {{
    {"u_colorTexture", colorTexture},
    {"u_positionTexture", positionTexture},
    {"u_normalTexture", normalTexture},
    {"u_depthTexture", depthTexture},
    {"u_diffuseCoefTexture", diffuseCoefTexture},
    {"u_specularCoefTexture", specularCoefTexture},
  }};

  for (std::size_t index = 0; index < allTextures.size(); ++index) {
    const TextureData& currData = allTextures.at(index);
    GlContext::Texture::active(uint32_t(index));
    auto location = _quadShader->getUniform(currData.name.data());
    _quadShader->setUniform(location, int32_t(index));
    currData.texture.bind();
  }

  _quadGeometry.render();

  GlContext::Texture::active(0);
}
