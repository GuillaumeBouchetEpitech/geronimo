
#include "ResultQuadRenderer.hpp"

#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/GlContext.hpp"
#include "geronimo/graphics/opengl/ShaderProgramBuilder.hpp"

using namespace gero::graphics::opengl;
using namespace gero::graphics::opengl::GlContext;

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {
namespace depthDeferred {

ResultQuadRenderer::ResultQuadRenderer() {}

ResultQuadRenderer::~ResultQuadRenderer() {}

void ResultQuadRenderer::initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize) {

  gero::graphics::opengl::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::opengl::GeometryBuilder geometryBuilder;

  const std::string k_rootPath = inRootPath + "/resultQuadRenderer/shaders/";

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
    // .addUniform("u_diffuseCoefTexture")
    // .addUniform("u_specularCoefTexture")
    .addUniform("u_diffuseColorTexture")
    .addUniform("u_specularColorTexture")
    .addUniform("u_ambientCoef")
    .addUniform("u_viewPos")
    .addUniform("u_sunLight");

  _quadShader = std::make_shared<gero::graphics::opengl::ShaderProgram>(shaderProgramBuilder.getDefinition());

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

  const std::array<Vertex, 4> vertices = {
    {{{float(inFrameSize.x) * 0.0f, float(inFrameSize.y) * 0.0f, -0.5f}, {0.0f, 0.0f}},
     {{float(inFrameSize.x) * 1.0f, float(inFrameSize.y) * 0.0f, -0.5f}, {1.0f, 0.0f}},
     {{float(inFrameSize.x) * 1.0f, float(inFrameSize.y) * 1.0f, -0.5f}, {1.0f, 1.0f}},
     {{float(inFrameSize.x) * 0.0f, float(inFrameSize.y) * 1.0f, -0.5f}, {0.0f, 1.0f}}}};

  const std::array<uint32_t, 6> indices = {{0, 1, 2, 2, 3, 0}};

  std::vector<Vertex> buffer;
  buffer.reserve(indices.size());

  for (uint32_t index : indices)
    buffer.push_back(vertices[index]);

  _quadGeometry.updateOrAllocateBuffer(0, buffer);
  _quadGeometry.setPrimitiveCount(uint32_t(buffer.size()));
}

void ResultQuadRenderer::render(const glm::vec3& eyePos,
                                const glm::vec4& sunLight,
                                const glm::mat4& composedMatrix,
                                const gero::graphics::opengl::Texture& colorTexture,
                                const gero::graphics::opengl::Texture& positionTexture,
                                const gero::graphics::opengl::Texture& normalTexture,
                                const gero::graphics::opengl::Texture& depthTexture,
                                // const gero::graphics::opengl::Texture& diffuseCoefTexture,
                                // const gero::graphics::opengl::Texture& specularCoefTexture,
                                const gero::graphics::opengl::Texture& diffuseColorTexture,
                                const gero::graphics::opengl::Texture& specularColorTexture,
                                float ambientLightCoef) {

  _quadShader->preBind([&](IBoundShaderProgram& bound) {
    bound.setUniform("u_composedMatrix", composedMatrix);
    bound.setUniform("u_ambientCoef", ambientLightCoef);
    bound.setUniform("u_viewPos", eyePos);
    bound.setUniform("u_sunLight", sunLight);

    struct TextureData {
      std::string_view name;
      const gero::graphics::opengl::Texture& texture;
    };

    const std::array<TextureData, 6> allTextures = {{
      {"u_colorTexture", colorTexture},
      {"u_positionTexture", positionTexture},
      {"u_normalTexture", normalTexture},
      {"u_depthTexture", depthTexture},
      // {"u_diffuseCoefTexture", diffuseCoefTexture},
      // {"u_specularCoefTexture", specularCoefTexture},
      {"u_diffuseColorTexture", diffuseColorTexture},
      {"u_specularColorTexture", specularColorTexture},
    }};

    for (std::size_t index = 0; index < allTextures.size(); ++index) {
      const TextureData& currData = allTextures.at(index);
      GlContext::Texture::active(uint32_t(index));
      auto location = bound.getUniform(currData.name.data());
      bound.setUniform(location, int32_t(index));
      currData.texture.bind();
    }

    _quadGeometry.render();

    GlContext::Texture::active(0);
  });
}

} // namespace depthDeferred
} // namespace opengl
} // namespace advanced
} // namespace graphics
} // namespace gero
