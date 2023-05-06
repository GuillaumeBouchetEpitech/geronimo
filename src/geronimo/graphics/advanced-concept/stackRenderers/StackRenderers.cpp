
#include "StackRenderers.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

#include "geronimo/system/asValue.hpp"

namespace gero {
namespace graphics {

void StackRenderers::initialize(std::shared_ptr<gero::graphics::ShaderProgram> inShader,
                                const gero::graphics::Geometry::Definition& inGeoDef) {
  _shader = inShader;

  //
  //
  //

  gero::graphics::Geometry::Definition tmpGeoDef = inGeoDef;

  tmpGeoDef.primitiveType = gero::graphics::Geometry::PrimitiveType::lines;
  _wireFramesStackRenderer.initialize(*_shader, tmpGeoDef);

  tmpGeoDef.primitiveType = gero::graphics::Geometry::PrimitiveType::triangles;
  _trianglesStackRenderer.initialize(*_shader, tmpGeoDef);
}

void StackRenderers::initialize(const gero::graphics::ShaderProgram::Definition& inShaderDef,
                                const gero::graphics::Geometry::Definition& inGeoDef) {

  auto newShader = std::make_shared<gero::graphics::ShaderProgram>(inShaderDef);

  initialize(newShader, inGeoDef);
}

void StackRenderers::initialize(const std::string& inRootPath) {

  std::string currSrcFolder = __FILE__;
  currSrcFolder = currSrcFolder.substr(0, currSrcFolder.rfind("src/") + 4);
  const std::string k_rootPath = inRootPath + "/geronimo/graphics/advanced-concept/stackRenderers/shaders/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "stackRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "stackRenderer.glsl.frag")
    .addAttribute("a_position")
    .addAttribute("a_color")
    .addUniform("u_composedMatrix");

  gero::graphics::GeometryBuilder geometryBuilder;
  geometryBuilder
    .reset()
    // .setShader(*_shader)
    // .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::lines)
    .addVbo()
    .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_color", gero::graphics::Geometry::AttrType::Vec4f)
    .setVboAsDynamic()
    ;

  StackRenderers::initialize(shaderProgramBuilder.getDefinition(), geometryBuilder.getDefinition(false));
}

void StackRenderers::setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void StackRenderers::flush() {
  if (!_trianglesStackRenderer.canRender() && !_wireFramesStackRenderer.canRender())
    return;

  if (!_shader)
    D_THROW(std::runtime_error, "shader not setup");

  _shader->bind();
  _shader->setUniform("u_composedMatrix", _matricesData.composed);

  _trianglesStackRenderer.flush();
  _wireFramesStackRenderer.flush();
}

void StackRenderers::safeMode(const std::function<void()>& callback) {
  if (!callback)
    return;

  if (!_shader)
    D_THROW(std::runtime_error, "shader not setup");

  _shader->bind();
  _shader->setUniform("u_composedMatrix", _matricesData.composed);

  _trianglesStackRenderer.startSafeMode();
  _wireFramesStackRenderer.startSafeMode();

  callback();

  _trianglesStackRenderer.stopSafeMode();
  _wireFramesStackRenderer.stopSafeMode();

  _trianglesStackRenderer.flush();
  _wireFramesStackRenderer.flush();
}

ITrianglesStackRenderer& StackRenderers::getTrianglesStack() { return _trianglesStackRenderer; }

IWireFramesStackRenderer& StackRenderers::getWireFramesStack() { return _wireFramesStackRenderer; }

} // namespace graphics
} // namespace gero
