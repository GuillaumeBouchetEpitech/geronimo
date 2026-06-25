
#include "InstancedBrickModels.hpp"

#include "geronimo/graphics/opengl/GeometryBuilder.hpp"
#include "geronimo/graphics/opengl/ShaderProgramBuilder.hpp"
#include "geronimo/graphics/camera/sceneToScreen.hpp"

#include <algorithm> // std::sort

void InstancedBrickModels::initialize() {

  const std::string basePath = "./assets/graphics/shaders/scene/instanced-brick-models/shaders/";

  gero::graphics::opengl::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(basePath + "instanced-brick-models.glsl.vert")
    .setFragmentFilename(basePath + "instanced-brick-models.glsl.frag")

    .addAttribute("a_vertexPosition")
    .addAttribute("a_vertexNormal")
    .addAttribute("a_vertexColor")

    .addAttribute("a_offsetPosition")
    .addAttribute("a_offsetOrientation")
    .addAttribute("a_offsetScale")
    .addAttribute("a_offsetColor")
    .addAttribute("a_offsetLight")

    .addUniform("u_composedMatrix");

  auto shaderDef = shaderProgramBuilder.getDefinition();

  _shader = std::make_shared<gero::graphics::opengl::ShaderProgram>(shaderDef);

  gero::graphics::opengl::GeometryBuilder geometryBuilder;
  geometryBuilder.reset()
    .setShader(*_shader)
    // .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::triangles)
    .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::lines)
    .addVbo()
    .addVboAttribute("a_vertexPosition", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertexNormal", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_vertexColor", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVbo()
    .setVboAsInstanced()
    .setVboAsDynamic()
    .addVboAttribute("a_offsetPosition", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_offsetOrientation", gero::graphics::opengl::Geometry::AttrType::Vec4f)
    .addVboAttribute("a_offsetScale", gero::graphics::opengl::Geometry::AttrType::Vec3f)
    .addVboAttribute("a_offsetColor", gero::graphics::opengl::Geometry::AttrType::Vec4f)
    .addVboAttribute("a_offsetLight", gero::graphics::opengl::Geometry::AttrType::Float);

  _geomDef_wireframes = geometryBuilder.getDefinition();

  // geometryBuilder.setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::triangles);
  // _geomDef_triangles = geometryBuilder.getDefinition();

  {
    geometryBuilder.reset()
      .setShader(*_shader)
      .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::triangles)
      // .setPrimitiveType(gero::graphics::opengl::Geometry::PrimitiveType::lines)
      .addVbo()
      .addVboAttribute("a_vertexPosition", gero::graphics::opengl::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_vertexNormal", gero::graphics::opengl::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_vertexColor", gero::graphics::opengl::Geometry::AttrType::Vec3f)
      .addVbo()
      .setVboAsInstanced()
      .setVboAsDynamic()
      .addVboAttribute("a_offsetPosition", gero::graphics::opengl::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetOrientation", gero::graphics::opengl::Geometry::AttrType::Vec4f)
      .addVboAttribute("a_offsetScale", gero::graphics::opengl::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetColor", gero::graphics::opengl::Geometry::AttrType::Vec4f)
      .addVboAttribute("a_offsetLight", gero::graphics::opengl::Geometry::AttrType::Float);

    _geomDef_triangles = geometryBuilder.getDefinition();
  }
}

void InstancedBrickModels::setMatricesData(const gero::graphics::camera::Camera::MatricesData& matricesData) {
  _matricesData = matricesData;
}

void InstancedBrickModels::createAlias(const std::string& alias, const Vertices& vertices_wireframes, const Vertices& vertices_triangles) {

  if (_aliasedGeometriesMap.count(alias) > 0) {
    D_THROW(std::runtime_error, "alias already exist, alias: " << alias);
  }

  auto newAlias = std::make_shared<AliasedGeometry>();

  newAlias->instanceVertices_wireframes.reserve(32); // pre-allocate
  newAlias->instanceVertices_triangles.reserve(32); // pre-allocate

  if (!vertices_wireframes.empty()) {
    newAlias->geometry_wireframes.initialize(*_shader, _geomDef_wireframes);
    newAlias->geometry_wireframes.allocateBuffer(0, vertices_wireframes);
    newAlias->geometry_wireframes.preAllocateBufferFromCapacity(1, newAlias->instanceVertices_wireframes);
    newAlias->geometry_wireframes.setPrimitiveStart(0);
    newAlias->geometry_wireframes.setPrimitiveCount(uint32_t(vertices_wireframes.size()));
  }

  if (!vertices_triangles.empty()) {
    newAlias->geometry_triangles.initialize(*_shader, _geomDef_triangles);
    newAlias->geometry_triangles.allocateBuffer(0, vertices_triangles);
    newAlias->geometry_triangles.preAllocateBufferFromCapacity(1, newAlias->instanceVertices_triangles);
    newAlias->geometry_triangles.setPrimitiveStart(0);
    newAlias->geometry_triangles.setPrimitiveCount(uint32_t(vertices_triangles.size()));
  }

  _aliasedGeometriesMap[alias] = newAlias;
}

void InstancedBrickModels::deleteAlias(const std::string& alias) { _aliasedGeometriesMap.erase(alias); }
void InstancedBrickModels::clearAlias(const std::string& alias) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  it->second->instanceVertices_wireframes.clear();
  it->second->instanceVertices_triangles.clear();
}

bool InstancedBrickModels::hasAlias(const std::string& alias) const {
  return this->_aliasedGeometriesMap.count(alias) > 0;
}

void InstancedBrickModels::pushAlias(const std::string& alias, const GeometryInstance& newInstance) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  auto& tmpData = *it->second;

  if (_strictMode && tmpData.instanceVertices_wireframes.size() + 1 < tmpData.instanceVertices_wireframes.capacity()) {
    GeometryInstance copiedInstance = newInstance;
    copiedInstance.light = false;
    tmpData.instanceVertices_wireframes.push_back(copiedInstance);
  }

  if (_strictMode && tmpData.instanceVertices_triangles.size() + 1 < tmpData.instanceVertices_triangles.capacity()) {
    tmpData.instanceVertices_triangles.push_back(newInstance);
  }
}

// void InstancedBrickModels::sortAlias(const std::string& alias, const gero::graphics::camera::ICamera& sceneCamera) {
//   auto it = _aliasedGeometriesMap.find(alias);
//   if (it == _aliasedGeometriesMap.end()) {
//     D_THROW(std::runtime_error, "alias not found, alias: " << alias);
//   }

//   auto& tmpData = *it->second;

//   std::sort(tmpData.instanceVertices.begin(),
//             tmpData.instanceVertices.end(),
//             [&sceneCamera](const GeometryInstance& left,
//                            const GeometryInstance& right) {
//               // sort by screen depth

//               auto& matricesData = sceneCamera.getMatricesData();

//               glm::vec3 leftScreenCoord = glm::vec3(0, 0, 1);
//               glm::vec3 rightScreenCoord = glm::vec3(0, 0, 1);

//               gero::graphics::camera::sceneToScreen(left.position,
//                                             matricesData.view,
//                                             matricesData.projection,
//                                             glm::vec2(0, 0),
//                                             sceneCamera.getSize(),
//                                             leftScreenCoord);

//               gero::graphics::camera::sceneToScreen(right.position,
//                                             matricesData.view,
//                                             matricesData.projection,
//                                             glm::vec2(0, 0),
//                                             sceneCamera.getSize(),
//                                             rightScreenCoord);

//               return leftScreenCoord.z > rightScreenCoord.z;
//             });
// }

void InstancedBrickModels::preAllocateAlias(const std::string& alias, std::size_t newSize) {
  auto it = _aliasedGeometriesMap.find(alias);
  if (it == _aliasedGeometriesMap.end()) {
    D_THROW(std::runtime_error, "alias not found, alias: " << alias);
  }

  auto& tmpData = *it->second;

  tmpData.instanceVertices_wireframes.reserve(newSize); // pre-allocate
  tmpData.instanceVertices_triangles.reserve(newSize); // pre-allocate

  if (tmpData.geometry_wireframes.getPrimitiveCount() > 0) {
    tmpData.geometry_wireframes.preAllocateBufferFromCapacity(1, tmpData.instanceVertices_wireframes);
  }

  if (tmpData.geometry_triangles.getPrimitiveCount() > 0) {
    tmpData.geometry_triangles.preAllocateBufferFromCapacity(1, tmpData.instanceVertices_triangles);
  }
}

void InstancedBrickModels::clearAll() { _aliasedGeometriesMap.clear(); }

void InstancedBrickModels::renderAll(bool inClearAll /*= true*/) {
  if (!_shader) {
    D_THROW(std::runtime_error, "shader not setup");
  }

  if (_aliasedGeometriesMap.empty()) {
    return;
  }

  _shader->preBind([this, inClearAll](gero::graphics::opengl::IBoundShaderProgram& boundShader) {
    boundShader.setUniform("u_composedMatrix", _matricesData.composed);

    for (const auto& pair : _aliasedGeometriesMap) {
      auto& tmpData = *pair.second;
      // auto& instanceVertices = tmpData.instanceVertices_wireframes;
      // if (instanceVertices.empty()) {
      //   continue;
      // }

      // {
      //   auto& geometry = tmpData.geometry_wireframes;
      //   if (geometry.getPrimitiveCount() > 0) {
      //     if (_strictMode) {
      //       geometry.updateBuffer(1, instanceVertices);
      //     } else {
      //       geometry.updateOrAllocateBuffer(1, instanceVertices);
      //     }

      //     geometry.setInstancedCount(uint32_t(instanceVertices.size()));
      //     geometry.render();
      //   }
      // }

      if (!tmpData.instanceVertices_wireframes.empty()) {
        auto& geometry = tmpData.geometry_wireframes;
        if (geometry.getPrimitiveCount() > 0) {
          if (_strictMode) {
            geometry.updateBuffer(1, tmpData.instanceVertices_wireframes);
          } else {
            geometry.updateOrAllocateBuffer(1, tmpData.instanceVertices_wireframes);
          }

          geometry.setInstancedCount(uint32_t(tmpData.instanceVertices_wireframes.size()));
          geometry.render();
        }
      }

      if (!tmpData.instanceVertices_triangles.empty()) {
        auto& geometry = tmpData.geometry_triangles;
        if (geometry.getPrimitiveCount() > 0) {
          if (_strictMode) {
            geometry.updateBuffer(1, tmpData.instanceVertices_triangles);
          } else {
            geometry.updateOrAllocateBuffer(1, tmpData.instanceVertices_triangles);
          }

          geometry.setInstancedCount(uint32_t(tmpData.instanceVertices_triangles.size()));
          geometry.render();
        }
      }

      if (inClearAll) {
        tmpData.instanceVertices_wireframes.clear();
        tmpData.instanceVertices_triangles.clear();
      }
    }
  });
}
