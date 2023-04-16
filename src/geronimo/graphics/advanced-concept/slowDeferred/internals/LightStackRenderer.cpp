
#include "LightStackRenderer.hpp"

#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"

namespace gero {
namespace graphics {

using namespace gero::graphics;
using TexQuality = gero::graphics::Texture::Quality;
using TexPattern = gero::graphics::Texture::Pattern;
using DepthFormat = gero::graphics::Texture::DepthFormat;
using DepthType = gero::graphics::Texture::DepthType;
using namespace gero::graphics::GlContext;

void PostProcessData::initialize(const glm::ivec2& inFrameSize,
                                 gero::graphics::ShaderProgram::Definition inShaderDef,
                                 gero::graphics::GeometryBuilder& geometryBuilder) {

  resize(inFrameSize);

  shader = std::make_shared<gero::graphics::ShaderProgram>(inShaderDef);

  geometryBuilder.setShader(*shader);

  geometry.initialize(*shader, geometryBuilder.getDefinition());
}

void PostProcessData::resize(const glm::ivec2& inFrameSize) {

  // depthTexture.allocateDepth(inFrameSize, DepthFormat::depth32, DepthType::float32);
  depthTexture.allocateDepth(inFrameSize, DepthFormat::depth16, DepthType::unsignedShort);
  // depthTexture.allocateCompatibleDepth(inFrameSize);

  gero::graphics::FrameBuffer::Definition def;
  def.depthTexture = &depthTexture;
  frameBuffer.initialize(def);
}

LightStackRenderer::LightStackRenderer() {}

LightStackRenderer::~LightStackRenderer() {}

void LightStackRenderer::initialize(const glm::ivec2& inFrameSize) {

  _frameSize = inFrameSize;

  gero::graphics::GeometryBuilder geometryBuilder;

  geometryBuilder.reset()
    .setPrimitiveType(Geometry::PrimitiveType::triangles)
    .addVbo()
    .addVboAttribute("a_vertex_position", Geometry::AttrType::Vec3f)
    .addIgnoredVboAttribute("a_vertex_normal", Geometry::AttrType::Vec3f)
    .addVbo()
    .setVboAsInstanced()
    .addVboAttribute("a_offset_lightPosition", Geometry::AttrType::Vec3f)
    .addVboAttribute("a_offset_lightRadius", Geometry::AttrType::Float);

  //
  //
  //

  const std::string k_rootPath = "assets/graphics/shaders/deferred/lightStackRenderer/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;

  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "lightStackRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "lightStackRenderer-diffuse.glsl.frag")
    .addAttribute("a_vertex_position")
    .addAttribute("a_offset_lightPosition")
    .addAttribute("a_offset_lightRadius")
    .addUniform("u_composedMatrix")
    .addUniform("u_positionTexture")
    .addUniform("u_normalTexture")
    // .addUniform("u_depthTexture")
    .addUniform("u_resolution");

  _diffuse.initialize(_frameSize, shaderProgramBuilder.getDefinition(), geometryBuilder);

  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "lightStackRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "lightStackRenderer-specular.glsl.frag")
    .addAttribute("a_vertex_position")
    .addAttribute("a_offset_lightPosition")
    .addAttribute("a_offset_lightRadius")
    .addUniform("u_composedMatrix")
    // .addUniform("u_colorTexture")
    .addUniform("u_positionTexture")
    .addUniform("u_normalTexture")
    .addUniform("u_diffuseCoefTexture")
    .addUniform("u_resolution")
    .addUniform("u_viewPos");

  _specular.initialize(_frameSize, shaderProgramBuilder.getDefinition(), geometryBuilder);

  // const vertices = generateSphereVertices(2, 1);

  gero::graphics::MakeGeometries::Vertices vertices;
  gero::graphics::MakeGeometries::makeSphere(vertices, 2, 1.0f);

  _diffuse.geometry.updateBuffer(0, vertices, false);
  _diffuse.geometry.setPrimitiveCount(uint32_t(vertices.size()));

  _specular.geometry.updateBuffer(0, vertices, false);
  _specular.geometry.setPrimitiveCount(uint32_t(vertices.size()));

  _vertices.reserve(1024);
}

void LightStackRenderer::resize(const glm::ivec2& inFrameSize) {
  if (_frameSize == inFrameSize)
    return;

  _frameSize = inFrameSize;

  //
  //

  _diffuse.resize(_frameSize);
  _specular.resize(_frameSize);
}

void LightStackRenderer::pushSpotLight(const glm::vec3& inPosition, float inRadius) {
  _vertices.push_back({inPosition, inRadius});
}

void LightStackRenderer::flush(const glm::vec3& eyePos,
                               const glm::mat4& composedMatrix,
                               const gero::graphics::Texture& positionTexture,
                               const gero::graphics::Texture& normalTexture) {

  _diffuse.frameBuffer.bind();

  // GlContext::setBackFaceCullingDirection(GlContext::BackFaceCullingDirection::counterClockWise);
  GlContext::setBackFaceCullingDirection(GlContext::BackFaceCullingDirection::clockWise);
  GlContext::clearDepth(0.0f);
  GlContext::setDepthFunc(GlContext::DepthFuncs::greater);
  GlContext::setColorMask(false, false, false, false);

  GlContext::clears(GlContext::Buffers::depth);

  _flushDiffuse(composedMatrix, positionTexture, normalTexture);

  _specular.frameBuffer.bind();

  GlContext::clears(GlContext::Buffers::depth);

  _flushSpecular(eyePos, composedMatrix, positionTexture, normalTexture, _diffuse.depthTexture);

  FrameBuffer::unbind();

  // GlContext::setBackFaceCullingDirection(GlContext::BackFaceCullingDirection::clockWise);
  GlContext::setBackFaceCullingDirection(GlContext::BackFaceCullingDirection::counterClockWise);
  GlContext::clearDepth(1.0f);
  GlContext::setDepthFunc(GlContext::DepthFuncs::less);
  GlContext::setColorMask(true, true, true, true);

  GlContext::Texture::active(0);

  reset();
}

void LightStackRenderer::_flushDiffuse(const glm::mat4& composedMatrix,
                                       const gero::graphics::Texture& positionTexture,
                                       const gero::graphics::Texture& normalTexture) {

  _diffuse.shader->bind();
  _diffuse.shader->setUniform("u_composedMatrix", composedMatrix);
  _diffuse.shader->setUniform("u_resolution", float(_frameSize.x), float(_frameSize.y));

  struct TextureData {
    std::string_view name;
    const gero::graphics::Texture& texture;
  };

  const std::array<TextureData, 2> allTextures = {{
    {"u_positionTexture", positionTexture},
    {"u_normalTexture", normalTexture},
  }};

  for (std::size_t index = 0; index < allTextures.size(); ++index) {
    const TextureData& currData = allTextures.at(index);
    GlContext::Texture::active(uint32_t(index));
    auto location = _diffuse.shader->getUniform(currData.name.data());
    _diffuse.shader->setUniform(location, int32_t(index));
    currData.texture.bind();
  }

  _diffuse.geometry.updateBuffer(1, _vertices, true);
  _diffuse.geometry.setInstancedCount(uint32_t(_vertices.size()));
  _diffuse.geometry.render();
}

void LightStackRenderer::_flushSpecular(const glm::vec3& eyePos,
                                        const glm::mat4& composedMatrix,
                                        const gero::graphics::Texture& positionTexture,
                                        const gero::graphics::Texture& normalTexture,
                                        const gero::graphics::Texture& diffuseCoefTexture) {

  _specular.shader->bind();
  _specular.shader->setUniform("u_composedMatrix", composedMatrix);
  _specular.shader->setUniform("u_resolution", float(_frameSize.x), float(_frameSize.y));
  _specular.shader->setUniform("u_viewPos", eyePos.x, eyePos.y, eyePos.z);

  struct TextureData {
    std::string_view name;
    const gero::graphics::Texture& texture;
  };

  const std::array<TextureData, 3> allTextures = {{
    {"u_positionTexture", positionTexture},
    {"u_normalTexture", normalTexture},
    {"u_diffuseCoefTexture", diffuseCoefTexture},
  }};

  for (std::size_t index = 0; index < allTextures.size(); ++index) {
    GlContext::Texture::active(uint32_t(index));
    auto location = _specular.shader->getUniform(allTextures.at(index).name.data());
    _specular.shader->setUniform(location, int32_t(index));
    allTextures.at(index).texture.bind();
  }

  _specular.geometry.updateBuffer(1, _vertices, true);
  _specular.geometry.setInstancedCount(uint32_t(_vertices.size()));
  _specular.geometry.render();
}

void LightStackRenderer::reset() { _vertices.clear(); }

const gero::graphics::Texture& LightStackRenderer::getDiffuseCoefTexture() { return _diffuse.depthTexture; }

const gero::graphics::Texture& LightStackRenderer::getSpecularCoefTexture() { return _specular.depthTexture; }

}
}
