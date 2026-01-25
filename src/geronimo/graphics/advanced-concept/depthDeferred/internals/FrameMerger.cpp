


#include "FrameMerger.hpp"


#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"


namespace gero {
namespace graphics {
namespace depthDeferred {

using namespace gero::graphics;
using TexQuality = gero::graphics::Texture::Quality;
using TexPattern = gero::graphics::Texture::Pattern;
using DepthFormat = gero::graphics::Texture::DepthFormat;
using DepthType = gero::graphics::Texture::DepthType;
using namespace gero::graphics::GlContext;


void FrameMerger::initialize(const std::string& inRootPath, const glm::ivec2& inFrameSize)
{
  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::GeometryBuilder geometryBuilder;

  const std::string k_rootPath = inRootPath + "/fame-merger/shaders/";

  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "frameMerger.glsl.vert")
    .setFragmentFilename(k_rootPath + "frameMerger.glsl.frag")
    .addAttribute("a_vertex_position")
    .addAttribute("a_vertex_uv")
    .addUniform("u_composedMatrix")
    //
    .addUniform("u_depthTexture_opaque")
    .addUniform("u_depthTexture_transparency")
    //
    .addUniform("u_colorTexture_opaque")
    .addUniform("u_colorTexture_transparency");

  quadShader = std::make_shared<gero::graphics::ShaderProgram>(shaderProgramBuilder.getDefinition());

  geometryBuilder.reset()
    .setShader(*quadShader)
    .setPrimitiveType(Geometry::PrimitiveType::triangles)
    .addVbo()
    .addVboAttribute("a_vertex_position", Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertex_uv", Geometry::AttrType::Vec2f);

  quadGeometry.initialize(*quadShader, geometryBuilder.getDefinition());

  resize(inFrameSize);
}



void FrameMerger::resize(const glm::ivec2& inFrameSize)
{
  if (frameSize == inFrameSize) {
    return;
  }

  frameSize = inFrameSize;

  {
    struct Vertex {
      glm::vec3 position;
      glm::vec2 uv;
    };

    const std::array<Vertex, 4> vertices = {{{{float(inFrameSize.x) * 0.0f, float(inFrameSize.y) * 0.0f, -0.5f}, {0.0f, 0.0f}},
                                            {{float(inFrameSize.x) * 1.0f, float(inFrameSize.y) * 0.0f, -0.5f}, {1.0f, 0.0f}},
                                            {{float(inFrameSize.x) * 1.0f, float(inFrameSize.y) * 1.0f, -0.5f}, {1.0f, 1.0f}},
                                            {{float(inFrameSize.x) * 0.0f, float(inFrameSize.y) * 1.0f, -0.5f}, {0.0f, 1.0f}}}};

    const std::array<uint32_t, 6> indices = {{0, 1, 2, 2, 3, 0}};

    std::vector<Vertex> buffer;
    buffer.reserve(indices.size());

    for (uint32_t index : indices)
      buffer.push_back(vertices[index]);

    quadGeometry.updateOrAllocateBuffer(0, buffer);
    quadGeometry.setPrimitiveCount(uint32_t(buffer.size()));
  }

  {
    depthTexture_opaque.allocateDepth(inFrameSize, DepthFormat::depth16, DepthType::unsignedShort);
    colorTexture_opaque.allocateBlankRgbaUBytes(inFrameSize, TexQuality::pixelated, TexPattern::clamped);

    gero::graphics::FrameBuffer::Definition def;
    def.depthTexture = &depthTexture_opaque;
    def.colorTextures.push_back({0, &colorTexture_opaque});
    frameBuffer_opaque.initialize(def);
  }

  {
    depthTexture_transparency.allocateDepth(inFrameSize, DepthFormat::depth16, DepthType::unsignedShort);
    colorTexture_transparency.allocateBlankRgbaUBytes(inFrameSize, TexQuality::pixelated, TexPattern::clamped);

    gero::graphics::FrameBuffer::Definition def;
    def.depthTexture = &depthTexture_transparency;
    def.colorTextures.push_back({0, &colorTexture_transparency});
    frameBuffer_transparency.initialize(def);
  }
}

//
//
//

//
//
//

void FrameMerger::startRecordingOpaque() {
  frameBuffer_opaque.bind();

  GlContext::clearColor(0.0f, 0.0f, 0.0f, 0.0f);
  GlContext::clearDepth(1.0f);
  GlContext::clears(Buffers::color, Buffers::depth);

  // GlContext::enable(States::depthTest);
  // GlContext::setDepthFunc(DepthFuncs::less);

  // // GlContext::enable(States::blend);
  // // GlContext::setBlendFunc(BlendFuncs::srcAlpha, BlendFuncs::oneMinusSrcAlpha);
  // GlContext::disable(States::blend);

  // GlContext::enable(States::cullFace);

}

void FrameMerger::stopRecordingOpaque() {
  FrameBuffer::unbind();
}

//
//
//

void FrameMerger::startRecordingTransparency() {
  frameBuffer_transparency.bind();

  GlContext::clearColor(0.0f, 0.0f, 0.0f, 0.0f);
  GlContext::clearDepth(1.0f);
  GlContext::clears(Buffers::color, Buffers::depth);

  // GlContext::disable(States::depthTest);
  // GlContext::enable(States::depthTest);
  // GlContext::setDepthFunc(DepthFuncs::less);

  // GlContext::enable(States::blend);
  // GlContext::setBlendFunc(BlendFuncs::srcAlpha, BlendFuncs::oneMinusSrcAlpha);

}

void FrameMerger::stopRecordingTransparency() {
  FrameBuffer::unbind();
  // GlContext::disable(States::blend);
  // GlContext::enable(States::depthTest);
}

//
//
//

//
//
//

void FrameMerger::render(const glm::mat4& composedMatrix)
{
  quadShader->preBind([&](IBoundShaderProgram& bound) {
    bound.setUniform("u_composedMatrix", composedMatrix);

    struct TextureData {
      std::string_view name;
      const gero::graphics::Texture& texture;
    };

    const std::array<TextureData, 4> allTextures = {{
      {"u_colorTexture_opaque", colorTexture_opaque},
      {"u_depthTexture_opaque", depthTexture_opaque},
      {"u_colorTexture_transparency", colorTexture_transparency},
      {"u_depthTexture_transparency", depthTexture_transparency},
    }};

    for (std::size_t index = 0; index < allTextures.size(); ++index) {
      const TextureData& currData = allTextures.at(index);
      GlContext::Texture::active(uint32_t(index));
      auto location = bound.getUniform(currData.name.data());
      bound.setUniform(location, int32_t(index));
      currData.texture.bind();
    }

    quadGeometry.render();

    GlContext::Texture::active(0);
  });
}


} // namespace depthDeferred
} // namespace graphics
} // namespace gero
