
#include "SceneStackRenderers.hpp"

#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/graphics/GeometryBuilder.hpp"

void SceneStackRenderers::initialize() {

  const std::string basePath = "./assets/graphics/shaders/scene/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(basePath + "stackRenderer.glsl.vert")
    .setFragmentFilename(basePath + "stackRenderer.glsl.frag")
    .addAttribute("a_position")
    .addAttribute("a_color")
    .addUniform("u_composedMatrix")
    ;

  gero::graphics::GeometryBuilder geometryBuilder;
  geometryBuilder.reset()
    // .setShader(*_shader)
    // .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::lines)
    .addVbo()
    .setVboAsDynamic()
    .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_color", gero::graphics::Geometry::AttrType::Vec4f);

  StackRenderers::initialize(shaderProgramBuilder.getDefinition(), geometryBuilder.getDefinition(false));
}
