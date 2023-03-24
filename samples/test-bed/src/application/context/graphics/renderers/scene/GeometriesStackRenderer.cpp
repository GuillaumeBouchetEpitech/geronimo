
#include "GeometriesStackRenderer.hpp"

#include "application/context/Context.hpp"
#include "application/context/graphics/graphicAliases.hpp"

void GeometriesStackRenderer::initialize() {
  auto& resourceManager = Context::get().graphic.resourceManager;

  _shader = resourceManager.getShader(gero::asValue(ShaderAliases::geometriesStackRenderer));
}

void GeometriesStackRenderer::setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void GeometriesStackRenderer::createAlias(int32_t alias, const gero::graphics::MakeGeometries::Vertices& vertices) {
  auto& resourceManager = Context::get().graphic.resourceManager;

  auto newAlias = std::make_shared<AliasedGeometry>();

  auto geoDef = resourceManager.getGeometryDefinition(gero::asValue(GeometryIds::geometriesStackRenderer));
  newAlias->geometry.initialize(*_shader, geoDef);
  newAlias->geometry.updateBuffer(0, vertices, false);
  newAlias->geometry.setPrimitiveStart(0);
  newAlias->geometry.setPrimitiveCount(uint32_t(vertices.size()));

  newAlias->instanceVertices.reserve(256); // pre-allocate

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

  _shader->bind();
  _shader->setUniform("u_composedMatrix", _matricesData.composed);
  // _shader->setUniform("u_ambiantCoef", 0.2f);

  // const glm::vec3& lightPos = Context::get().graphic.scene.lightPos;
  // _shader->setUniform("u_lightPos", lightPos.x, lightPos.y, lightPos.z);

  for (const auto& pair : _aliasedGeometriesMap) {
    auto& vertices = pair.second->instanceVertices;
    if (vertices.empty())
      continue;

    auto& geometry = pair.second->geometry;

    geometry.updateBuffer(1, vertices, true);
    geometry.setInstancedCount(uint32_t(vertices.size()));
    geometry.render();

    vertices.clear();
  }
}
