
#include "StackRenderers.hpp"

#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/ShaderProgramBuilder.hpp"

#include "geronimo/system/asValue.hpp"

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {

void StackRenderers::initialize(std::shared_ptr<gero::graphics::opengl::IUnboundShaderProgram> inShader,
                                const gero::graphics::opengl::Geometry::Definition& inGeoDef) {
  _shader = inShader;

  //
  //
  //

  gero::graphics::opengl::Geometry::Definition tmpGeoDef = inGeoDef;

  tmpGeoDef.primitiveType = gero::graphics::opengl::Geometry::PrimitiveType::lines;
  _wireFramesStackRenderer.initialize(*_shader, tmpGeoDef);

  tmpGeoDef.primitiveType = gero::graphics::opengl::Geometry::PrimitiveType::triangles;
  _trianglesStackRenderer.initialize(*_shader, tmpGeoDef);
}

void StackRenderers::initialize(const gero::graphics::opengl::ShaderProgram::Definition& inShaderDef,
                                const gero::graphics::opengl::Geometry::Definition& inGeoDef) {

  auto newShader = gero::graphics::opengl::ShaderProgram::buildUnbound(inShaderDef);

  initialize(newShader, inGeoDef);
}

void StackRenderers::initialize(const std::string& inRootPath) {

  const std::string k_rootPath = inRootPath + "/shaders/";

  gero::graphics::opengl::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "stackRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "stackRenderer.glsl.frag")
    .addAttribute("a_position")
    .addAttribute("a_color")
    .addUniform("u_composedMatrix");

  gero::graphics::opengl::GeometryBuilder geometryBuilder;
  geometryBuilder
    .reset()
    // .setShader(*_shader)
    // .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::lines)
    .addVbo()
    .addVboAttribute("a_position", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_color", gero::graphics::opengl::Geometry::AttrType::Vec4f)
    .setVboAsDynamic();

  StackRenderers::initialize(shaderProgramBuilder.getDefinition(), geometryBuilder.getDefinition(false));
}

void StackRenderers::setMatricesData(const gero::graphics::camera::ICamera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void StackRenderers::flush() {
  if (!_trianglesStackRenderer.canRender() && !_wireFramesStackRenderer.canRender())
    return;

  if (!_shader)
    D_THROW(std::runtime_error, "shader not setup");

  _shader->preBind([this](opengl::IBoundShaderProgram& bound) {
    bound.setUniform("u_composedMatrix", _matricesData.composed);

    _trianglesStackRenderer.flush();
    _wireFramesStackRenderer.flush();
  });
}

void StackRenderers::safeMode(const std::function<void()>& callback) {
  if (!callback)
    return;

  if (!_shader)
    D_THROW(std::runtime_error, "shader not setup");

  _shader->preBind([&](opengl::IBoundShaderProgram& bound) {
    bound.setUniform("u_composedMatrix", _matricesData.composed);

    _trianglesStackRenderer.startSafeMode();
    _wireFramesStackRenderer.startSafeMode();

    callback();

    _trianglesStackRenderer.stopSafeMode();
    _wireFramesStackRenderer.stopSafeMode();

    _trianglesStackRenderer.flush();
    _wireFramesStackRenderer.flush();
  });
}

stackRenderers::ITrianglesStackRenderer& StackRenderers::getTrianglesStack() { return _trianglesStackRenderer; }

stackRenderers::IWireFramesStackRenderer& StackRenderers::getWireFramesStack() { return _wireFramesStackRenderer; }

} // namespace advanced
} // namespace opengl
} // namespace graphics
} // namespace gero
