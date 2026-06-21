
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
#include <string>

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
  void createAlias(const std::string& alias, const Vertices& vertices);
  void deleteAlias(const std::string& alias);
  bool hasAlias(const std::string& alias) const;
  void clearAlias(const std::string& alias);
  void pushAlias(const std::string& alias, const GeometryInstance& newInstance);
  void sortAlias(const std::string& alias, const gero::graphics::camera::ICamera& sceneCamera);
  void preAllocateAlias(const std::string& alias, std::size_t newSize);

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

  std::unordered_map<std::string, std::shared_ptr<AliasedGeometry>> _aliasedGeometriesMap;

  bool _strictMode = true;
};
