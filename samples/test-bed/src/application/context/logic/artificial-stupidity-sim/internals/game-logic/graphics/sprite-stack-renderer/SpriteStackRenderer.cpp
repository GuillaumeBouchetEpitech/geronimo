
#include "SpriteStackRenderer.hpp"


#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/ShaderProgramBuilder.hpp"


SpriteStackRenderer::SpriteStackRenderer()
{
}

SpriteStackRenderer::~SpriteStackRenderer()
{
}


void SpriteStackRenderer::initialize() {

  _stack.reserve(512);

  // const std::string basePath = "./assets/graphics/shaders/scene/";
  // const std::string basePath = "./src/application/context/graphics/renderers/scene/geometries-stack-renderer/shaders/";
  // const std::string basePath = "./src/application/context/graphics/renderers/scene/sprite-stack-renderer/shaders/";
  const std::string basePath = "./assets/graphics/scene/sprite-stack-renderer/shaders/";

  gero::graphics::opengl::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(basePath + "sprite.glsl.vert")
    .setFragmentFilename(basePath + "sprite.glsl.frag")
    .addAttribute("a_vertexPosition")
    .addAttribute("a_vertexTexCoord")
    .addAttribute("a_vertexColor")
    .addUniform("u_composedMatrix")
    .addUniform("u_texture")
    ;

  auto shaderDef = shaderProgramBuilder.getDefinition();

  _shader = std::make_shared<gero::graphics::opengl::ShaderProgram>(shaderDef);

  gero::graphics::opengl::GeometryBuilder geometryBuilder;
  geometryBuilder.reset()
    .setShader(*_shader)
    .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::triangles)
    .addVbo()
    .setVboAsDynamic()
    .addVboAttribute("a_vertexPosition", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertexTexCoord", gero::graphics::opengl::Geometry::AttrType::Vec2f)
    .addVboAttribute("a_vertexColor", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    ;

  _geometry.initialize(*_shader, geometryBuilder.getDefinition());
  _geometry.preAllocateBufferFromCapacity(0, _stack);
  _geometry.setPrimitiveStart(0);
  _geometry.setPrimitiveCount(0);
}

void SpriteStackRenderer::setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void SpriteStackRenderer::push(const StackElement& newElem)
{
  _stack.push_back(newElem);
}

void SpriteStackRenderer::pushFrame(const FrameElement& newFrame)
{
  StackElement bottomLeft;
  bottomLeft.position = glm::vec3(newFrame.pos.bottomLeft.x,newFrame.pos.bottomLeft.y,newFrame.depth);
  bottomLeft.texCoord = glm::vec2(newFrame.texcoord.bottomLeft.x,newFrame.texcoord.topRight.y);
  bottomLeft.color = newFrame.color;

  StackElement bottomRight;
  bottomRight.position = glm::vec3(newFrame.pos.topRight.x,newFrame.pos.bottomLeft.y,newFrame.depth);
  bottomRight.texCoord = glm::vec2(newFrame.texcoord.topRight.x,newFrame.texcoord.topRight.y);
  bottomRight.color = newFrame.color;

  StackElement topLeft;
  topLeft.position = glm::vec3(newFrame.pos.bottomLeft.x,newFrame.pos.topRight.y,newFrame.depth);
  topLeft.texCoord = glm::vec2(newFrame.texcoord.bottomLeft.x,newFrame.texcoord.bottomLeft.y);
  topLeft.color = newFrame.color;

  StackElement topRight;
  topRight.position = glm::vec3(newFrame.pos.topRight.x,newFrame.pos.topRight.y,newFrame.depth);
  topRight.texCoord = glm::vec2(newFrame.texcoord.topRight.x,newFrame.texcoord.bottomLeft.y);
  topRight.color = newFrame.color;

  _stack.push_back(bottomLeft);
  _stack.push_back(bottomRight);
  _stack.push_back(topLeft);

  _stack.push_back(bottomRight);
  _stack.push_back(topRight);
  _stack.push_back(topLeft);
}

void SpriteStackRenderer::render()
{
  if (!_shader) {
    D_THROW(std::runtime_error, "shader not setup");
  }

  if (_stack.empty()) {
    return;
  }

  _shader->preBind([this](gero::graphics::opengl::IBoundShaderProgram& boundShader) {
    boundShader.setUniform("u_texture", 0);
    boundShader.setUniform("u_composedMatrix", _matricesData.composed);

    _geometry.updateOrAllocateBuffer(0, _stack);
    _geometry.setPrimitiveCount(uint32_t(_stack.size()));
    _geometry.render();

  });

  clear();
}

void SpriteStackRenderer::clear()
{
  _stack.clear();
}

