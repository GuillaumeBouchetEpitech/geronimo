
#include "GeometriesStackRenderer.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"

void GeometriesStackRenderer::initialize() {

  // const std::string basePath = "./assets/graphics/shaders/scene/";
  const std::string basePath = "./src/application/context/graphics/renderers/scene/geometries-stack-renderer/shaders/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(basePath + "geometries-stack-renderer.glsl.vert")
    .setFragmentFilename(basePath + "geometries-stack-renderer.glsl.frag")

    .addAttribute("a_vertexPosition")
    .addAttribute("a_vertexNormal")

    .addAttribute("a_offsetPosition")
    .addAttribute("a_offsetOrientation")
    .addAttribute("a_offsetScale")
    .addAttribute("a_offsetColor")
    .addAttribute("a_offsetLight")

    .addUniform("u_composedMatrix")
    // .addUniform("u_ambiantCoef")
    // .addUniform("u_lightPos")
    ;

  auto shaderDef = shaderProgramBuilder.getDefinition();

  _shader = std::make_shared<gero::graphics::ShaderProgram>(shaderDef);

  gero::graphics::GeometryBuilder geometryBuilder;
  geometryBuilder.reset()
    .setShader(*_shader)
    .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles)
    .addVbo()
    .addVboAttribute("a_vertexPosition", gero::graphics::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertexNormal", gero::graphics::Geometry::AttrType::Vec3f)
    .addVbo()
    .setVboAsInstanced()
    .setVboAsDynamic()
    .addVboAttribute("a_offsetPosition", gero::graphics::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_offsetOrientation", gero::graphics::Geometry::AttrType::Vec4f)
    .addVboAttribute("a_offsetScale", gero::graphics::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_offsetColor", gero::graphics::Geometry::AttrType::Vec4f)
    .addVboAttribute("a_offsetLight", gero::graphics::Geometry::AttrType::Float);

  _geomDef = geometryBuilder.getDefinition();
}

void GeometriesStackRenderer::setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void GeometriesStackRenderer::createAlias(int32_t alias, const gero::graphics::MakeGeometries::Vertices& vertices) {

  auto newAlias = std::make_shared<AliasedGeometry>();

  newAlias->instanceVertices.reserve(256); // pre-allocate

  newAlias->geometry.initialize(*_shader, _geomDef);
  newAlias->geometry.allocateBuffer(0, vertices);
  newAlias->geometry.preAllocateBufferFromCapacity(1, newAlias->instanceVertices);
  newAlias->geometry.setPrimitiveStart(0);
  newAlias->geometry.setPrimitiveCount(uint32_t(vertices.size()));

  _aliasedGeometriesMap[alias] = newAlias;
}

void GeometriesStackRenderer::deleteAlias(int32_t alias) { _aliasedGeometriesMap.erase(alias); }
void GeometriesStackRenderer::clearAlias(int32_t alias) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end())
    return;

  it->second->instanceVertices.clear();
}

void GeometriesStackRenderer::pushAlias(int32_t alias, const GeometryInstance& newInstance) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end())
    return;

  it->second->instanceVertices.push_back(newInstance);
}

void GeometriesStackRenderer::clearAll() { _aliasedGeometriesMap.clear(); }

void GeometriesStackRenderer::renderAll() {
  if (!_shader)
    D_THROW(std::runtime_error, "not initialized");

  if (_aliasedGeometriesMap.empty())
    return;

  _shader->preBind([this](gero::graphics::IBoundShaderProgram& bound)
  {
    bound.setUniform("u_composedMatrix", _matricesData.composed);
    // bound.setUniform("u_ambiantCoef", 0.2f);

    for (const auto& pair : _aliasedGeometriesMap) {
      auto& vertices = pair.second->instanceVertices;
      if (vertices.empty())
        continue;

      auto& geometry = pair.second->geometry;

      geometry.updateOrAllocateBuffer(1, vertices);
      geometry.setInstancedCount(uint32_t(vertices.size()));
      geometry.render();

      vertices.clear();
    }

  });
}
