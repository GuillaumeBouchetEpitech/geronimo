
#include "SceneStackRenderers.hpp"

#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/ShaderProgramBuilder.hpp"

void SceneStackRenderers::initialize() {

  // const std::string basePath = "./assets/graphics/shaders/scene/";
  const std::string basePath = "./assets/graphics/scene/advanced-concept-sub-assets/stackRenderers/shaders/";

  gero::graphics::opengl::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(basePath + "stackRenderer.glsl.vert")
    .setFragmentFilename(basePath + "stackRenderer.glsl.frag")
    .addAttribute("a_position")
    .addAttribute("a_color")
    .addUniform("u_composedMatrix");

  gero::graphics::opengl::GeometryBuilder geometryBuilder;
  geometryBuilder
    .reset()
    // .setShader(*_shader)
    // .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::lines)
    .addVbo()
    .setVboAsDynamic()
    .addVboAttribute("a_position", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_color", gero::graphics::opengl::Geometry::AttrType::Vec4f);

  StackRenderers::initialize(shaderProgramBuilder.getDefinition(), geometryBuilder.getDefinition(false));
}
