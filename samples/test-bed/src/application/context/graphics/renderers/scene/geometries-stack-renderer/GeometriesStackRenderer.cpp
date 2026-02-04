
#include "GeometriesStackRenderer.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/graphics/camera/sceneToScreen.hpp"

#include <algorithm> // std::sort

void GeometriesStackRenderer::initialize() {

  // const std::string basePath = "./assets/graphics/shaders/scene/";
  const std::string basePath = "./assets/graphics/scene/geometriesStackRenderer/shaders/";

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

    .addUniform("u_composedMatrix");

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

  if (_aliasedGeometriesMap.count(alias) > 0) {
    D_THROW(std::runtime_error, "alias already exist, alias: " << alias);
  }

  auto newAlias = std::make_shared<AliasedGeometry>();

  newAlias->instanceVertices.reserve(1024); // pre-allocate

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
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  it->second->instanceVertices.clear();
}

void GeometriesStackRenderer::pushAlias(int32_t alias, const GeometryInstance& newInstance) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  auto& tmpData = *it->second;

  if (_strictMode && tmpData.instanceVertices.size() + 1 >= tmpData.instanceVertices.capacity()) {
    // D_THROW(std::runtime_error, "alias buffer out of of space, alias: " << alias << ", capacity: " <<
    // tmpData.instanceVertices.capacity());
    return;
  }

  tmpData.instanceVertices.push_back(newInstance);
}

void GeometriesStackRenderer::sortAlias(int32_t alias, const gero::graphics::ICamera& sceneCamera) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  auto& tmpData = *it->second;

  std::sort(tmpData.instanceVertices.begin(),
            tmpData.instanceVertices.end(),
            [&sceneCamera](const IGeometriesStackRenderer::GeometryInstance& left,
                           const IGeometriesStackRenderer::GeometryInstance& right) {
              // sort by screen depth

              auto& matricesData = sceneCamera.getMatricesData();

              glm::vec3 leftScreenCoord = glm::vec3(0, 0, 1);
              glm::vec3 rightScreenCoord = glm::vec3(0, 0, 1);

              gero::graphics::sceneToScreen(left.position,
                                            matricesData.view,
                                            matricesData.projection,
                                            glm::vec2(0, 0),
                                            sceneCamera.getSize(),
                                            leftScreenCoord);

              gero::graphics::sceneToScreen(right.position,
                                            matricesData.view,
                                            matricesData.projection,
                                            glm::vec2(0, 0),
                                            sceneCamera.getSize(),
                                            rightScreenCoord);

              return leftScreenCoord.z > rightScreenCoord.z;
            });
}

void GeometriesStackRenderer::preAllocateAlias(int32_t alias, std::size_t newSize) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  auto& tmpData = *it->second;

  tmpData.instanceVertices.reserve(newSize); // pre-allocate
  tmpData.geometry.preAllocateBufferFromCapacity(1, tmpData.instanceVertices);
}

void GeometriesStackRenderer::clearAll() { _aliasedGeometriesMap.clear(); }

void GeometriesStackRenderer::renderAll(bool inClearAll /*= true*/) {
  if (!_shader) {
    D_THROW(std::runtime_error, "shader not setup");
  }

  if (_aliasedGeometriesMap.empty()) {
    return;
  }

  _shader->preBind([this, inClearAll](gero::graphics::IBoundShaderProgram& boundShader) {
    boundShader.setUniform("u_composedMatrix", _matricesData.composed);

    for (const auto& pair : _aliasedGeometriesMap) {
      auto& tmpData = *pair.second;
      auto& vertices = tmpData.instanceVertices;
      if (vertices.empty()) {
        continue;
      }

      auto& geometry = tmpData.geometry;
      if (_strictMode) {
        geometry.updateBuffer(1, vertices);
      } else {
        geometry.updateOrAllocateBuffer(1, vertices);
      }

      geometry.setInstancedCount(uint32_t(vertices.size()));
      geometry.render();

      if (inClearAll) {
        vertices.clear();
      }
    }
  });
}
