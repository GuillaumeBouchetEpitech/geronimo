
#include "Context.hpp"

#include "application/context/graphics/graphicAliases.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/Image.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace {

void initializeHudStructures(gero::graphics::ResourceManager& rManager) {
  const std::string basePath = "./assets/graphics/shaders/hud/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::GeometryBuilder geometryBuilder;

  {
    shaderProgramBuilder.reset()
      .setVertexFilename(basePath + "stackRenderer.glsl.vert")
      .setFragmentFilename(basePath + "stackRenderer.glsl.frag")
      .addAttribute("a_position")
      .addAttribute("a_color")
      .addUniform("u_composedMatrix");

    auto shader =
      rManager.createShader(gero::asValue(ShaderAliases::stackRendererHud), shaderProgramBuilder.getDefinition());

    geometryBuilder.reset()
      .setShader(*shader)
      .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::lines)
      .addVbo()
      .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_color", gero::graphics::Geometry::AttrType::Vec4f);

    rManager.createGeometryDefinition(
      gero::asValue(GeometryIds::stackRendererWireFramesHud), geometryBuilder.getDefinition(), true);

    geometryBuilder.setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles);

    rManager.createGeometryDefinition(
      gero::asValue(GeometryIds::stackRendererTrianglesHud), geometryBuilder.getDefinition(), true);
  }

  {
    shaderProgramBuilder.reset()
      .setVertexFilename(basePath + "textRenderer.glsl.vert")
      .setFragmentFilename(basePath + "textRenderer.glsl.frag")
      .addAttribute("a_position")
      .addAttribute("a_texCoord")
      .addAttribute("a_offsetPosition")
      .addAttribute("a_offsetTexCoord")
      .addAttribute("a_offsetColor")
      .addAttribute("a_offsetScale")
      .addUniform("u_composedMatrix")
      .addUniform("u_texture");

    auto shader =
      rManager.createShader(gero::asValue(ShaderAliases::textRenderer), shaderProgramBuilder.getDefinition());

    geometryBuilder.reset()
      .setShader(*shader)
      .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles)
      .addVbo()
      .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec2f)
      .addVboAttribute("a_texCoord", gero::graphics::Geometry::AttrType::Vec2f)
      .addVbo()
      .setVboAsInstanced()
      .addVboAttribute("a_offsetPosition", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetTexCoord", gero::graphics::Geometry::AttrType::Vec2f)
      .addVboAttribute("a_offsetColor", gero::graphics::Geometry::AttrType::Vec4f)
      .addVboAttribute("a_offsetScale", gero::graphics::Geometry::AttrType::Float);

    rManager.createGeometryDefinition(gero::asValue(GeometryIds::textRenderer), geometryBuilder.getDefinition(), true);
  }
}

void initializeSceneStructures(gero::graphics::ResourceManager& rManager) {
  const std::string basePath = "./assets/graphics/shaders/scene/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  gero::graphics::GeometryBuilder geometryBuilder;

  {

    shaderProgramBuilder.reset()
      .setVertexFilename(basePath + "geometriesStackRenderer.glsl.vert")
      .setFragmentFilename(basePath + "geometriesStackRenderer.glsl.frag")

      .addAttribute("a_vertexPosition")
      .addAttribute("a_vertexNormal")

      .addAttribute("a_offsetPosition")
      .addAttribute("a_offsetOrientation")
      .addAttribute("a_offsetScale")
      .addAttribute("a_offsetColor")

      .addUniform("u_composedMatrix")
      // .addUniform("u_ambiantCoef")
      // .addUniform("u_lightPos")
      ;

    auto shader = rManager.createShader(gero::asValue(ShaderAliases::geometriesStackRenderer),
                                        shaderProgramBuilder.getDefinition());

    geometryBuilder.reset()
      .setShader(*shader)
      .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles)
      .addVbo()
      .addVboAttribute("a_vertexPosition", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_vertexNormal", gero::graphics::Geometry::AttrType::Vec3f)
      .addVbo()
      .setVboAsInstanced()
      .addVboAttribute("a_offsetPosition", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetOrientation", gero::graphics::Geometry::AttrType::Vec4f)
      .addVboAttribute("a_offsetScale", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetColor", gero::graphics::Geometry::AttrType::Vec4f);

    rManager.createGeometryDefinition(
      gero::asValue(GeometryIds::geometriesStackRenderer), geometryBuilder.getDefinition(), true);
  }

  {
    shaderProgramBuilder.reset()
      .setVertexFilename(basePath + "stackRenderer.glsl.vert")
      .setFragmentFilename(basePath + "stackRenderer.glsl.frag")
      .addAttribute("a_position")
      .addAttribute("a_color")
      .addUniform("u_composedMatrix");

    auto shader =
      rManager.createShader(gero::asValue(ShaderAliases::stackRendererScene), shaderProgramBuilder.getDefinition());

    geometryBuilder.reset()
      .setShader(*shader)
      .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::lines)
      .addVbo()
      .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_color", gero::graphics::Geometry::AttrType::Vec4f);

    rManager.createGeometryDefinition(
      gero::asValue(GeometryIds::stackRendererWireFramesScene), geometryBuilder.getDefinition(), true);

    geometryBuilder.setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles);

    rManager.createGeometryDefinition(
      gero::asValue(GeometryIds::stackRendererTrianglesScene), geometryBuilder.getDefinition(), true);
  }
}

} // namespace

void Context::initializeGraphicResources() {

  D_MYLOG("MaxTextureSize: " << gero::graphics::GlContext::getMaxTextureSize());

  {
    const auto& vSize = graphic.camera.viewportSize;

    graphic.camera.scene.setPerspective(70.0f, 1.0f, 150.0f);

    graphic.camera.hud.setOrthographic(0.0f, float(vSize.x), 0.0f, float(vSize.y), -10.0f, +10.0f);

    const glm::vec3 eye = {0.0f, 0.0f, 1.0f};
    const glm::vec3 center = {0.0f, 0.0f, 0.0f};
    const glm::vec3 upAxis = {0.0f, 1.0f, 0.0f};
    graphic.camera.hud.lookAt(eye, center, upAxis);
  }

  // gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  // gero::graphics::GeometryBuilder geometryBuilder;

  auto& rManager = graphic.resourceManager;

  initializeHudStructures(rManager);
  initializeSceneStructures(rManager);

  {

    const std::string basePath = "./assets/graphics/textures/";

    rManager.createTexture(gero::asValue(TextureIds::asciiFont),
                           basePath + "ascii_font.png",
                           gero::graphics::Texture::Quality::pixelated,
                           gero::graphics::Texture::Pattern::clamped);
  }
}
