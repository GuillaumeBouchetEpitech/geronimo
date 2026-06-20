
#pragma once

#include "geronimo/graphics/opengl/Geometry.hpp"
#include "geronimo/graphics/opengl/ShaderProgram.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

class InstancedBrickModels : public gero::NonCopyable {
public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
  };
  using Vertices = std::vector<Vertex>;

  struct GeometryInstance {
    // glm::mat4 transform;
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;
    glm::vec4 color;
    float light;
  };
  using InstanceVertices = std::vector<GeometryInstance>;

public:
  InstancedBrickModels() = default;
  ~InstancedBrickModels() = default;

public:
  void initialize();
  void setMatricesData(const gero::graphics::camera::Camera::MatricesData& matricesData);

public:
  void createAlias(int32_t alias, const Vertices& vertices);
  void deleteAlias(int32_t alias);
  void clearAlias(int32_t alias);
  void pushAlias(int32_t alias, const GeometryInstance& newInstance);
  void sortAlias(int32_t alias, const gero::graphics::camera::ICamera& sceneCamera);
  void preAllocateAlias(int32_t alias, std::size_t newSize);

public:
  void clearAll();
  void renderAll(bool clearAll = true);

private:
  std::shared_ptr<gero::graphics::opengl::ShaderProgram> _shader = nullptr;
  gero::graphics::opengl::Geometry::Definition _geomDef;
  gero::graphics::camera::Camera::MatricesData _matricesData;

private:
  struct AliasedGeometry {
    gero::graphics::opengl::Geometry geometry;
    InstanceVertices instanceVertices;
  };

  std::unordered_map<int32_t, std::shared_ptr<AliasedGeometry>> _aliasedGeometriesMap;

  bool _strictMode = true;
};
