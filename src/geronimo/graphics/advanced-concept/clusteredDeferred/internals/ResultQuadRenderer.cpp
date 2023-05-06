
#include "ResultQuadRenderer.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/graphics/GlContext.hpp"

namespace gero {
namespace graphics {

using namespace gero::graphics;
using namespace gero::graphics::GlContext;

ResultQuadRenderer::ResultQuadRenderer() {}

ResultQuadRenderer::~ResultQuadRenderer() {}

void ResultQuadRenderer::initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize) {

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::GeometryBuilder geometryBuilder;

  std::string currSrcFolder = __FILE__;
  currSrcFolder = currSrcFolder.substr(0, currSrcFolder.rfind("src/") + 4);
  const std::string k_rootPath =
    inRootPath + "/geronimo/graphics/advanced-concept/clusteredDeferred/internals/shaders/";

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
    .addUniform("u_clusterDataTexture")
    .addUniform("u_lightsDataTexture")

    .addUniform("u_viewMatrix")

    .addUniform("u_ambiantCoef")
    .addUniform("u_viewPos")
    .addUniform("u_sunLightDirection")

    .addUniform("u_screenWidth")
    .addUniform("u_screenHeight")
    .addUniform("u_camNear")
    .addUniform("u_zStride")

    .addUniform("u_numXSlices")
    .addUniform("u_numYSlices")
    .addUniform("u_maxLightsPerCluster")

    ;

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

  _quadGeometry.allocateBuffer(0, buffer);
  _quadGeometry.updateBuffer(0, buffer);
  _quadGeometry.setPrimitiveCount(uint32_t(buffer.size()));
}

void ResultQuadRenderer::render(const glm::vec3& sunLightDirection,
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
                                float ambiantLightCoef) {

  if (inSceneCamera.getProjectionType() != Camera::ProjectionType::perspective)
    D_THROW(std::runtime_error, "scene camera is not set with a perspective projection");
  if (inHudCamera.getProjectionType() != Camera::ProjectionType::orthographic)
    D_THROW(std::runtime_error, "hud camera is not set with a orthographic projection");

  _quadShader->bind();

  _quadShader->setUniform("u_composedMatrix", inHudCamera.getMatricesData().composed);
  _quadShader->setUniform("u_ambiantCoef", ambiantLightCoef);
  _quadShader->setUniform("u_viewPos", inSceneCamera.getEye());
  _quadShader->setUniform("u_sunLightDirection", sunLightDirection);

  _quadShader->setUniform("u_viewMatrix", inSceneCamera.getMatricesData().view);

  _quadShader->setUniform("u_screenWidth", 800.0f);
  _quadShader->setUniform("u_screenHeight", 600.0f);
  _quadShader->setUniform("u_camNear", inSceneCamera.getProjectionData().perspective.near);
  _quadShader->setUniform("u_zStride", inZStride);

  _quadShader->setUniform("u_numXSlices", int32_t(clusterDef.clusterSliceX));
  _quadShader->setUniform("u_numYSlices", int32_t(clusterDef.clusterSliceY));
  _quadShader->setUniform("u_maxLightsPerCluster", int32_t(clusterDef.maxLightsPerCluster));

  struct TextureBinding {
    std::string_view name;
    const gero::graphics::Texture& texture;
  };

  struct DataTextureBinding {
    std::string_view name;
    const gero::graphics::DataTexture& texture;
  };

  const std::array<TextureBinding, 6> allTextures = {{
    {"u_colorTexture", colorTexture},
    {"u_positionTexture", positionTexture},
    {"u_normalTexture", normalTexture},
    {"u_depthTexture", depthTexture},
    {"u_clusterDataTexture", clusterDataTexture.getTexture()},
    {"u_lightsDataTexture", lightsDataTexture.getTexture()},
  }};

  for (std::size_t index = 0; index < allTextures.size(); ++index) {
    const TextureBinding& currData = allTextures.at(index);
    GlContext::Texture::active(uint32_t(index));
    auto location = _quadShader->getUniform(currData.name.data());
    _quadShader->setUniform(location, int32_t(index));
    currData.texture.bind();
  }

  _quadGeometry.render();

  GlContext::Texture::active(0);
}

} // namespace graphics
} // namespace gero
