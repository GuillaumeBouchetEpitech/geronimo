
#pragma once

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

class IGeometriesStackRenderer {
public:
  struct GeometryInstance {
    // glm::mat4 transform;
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;
    glm::vec4 color;
    float light;
  };

public:
  virtual ~IGeometriesStackRenderer() = default;

public:
  virtual void setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) = 0;

public:
  virtual void createAlias(int32_t alias, const gero::graphics::MakeGeometries::Vertices& vertices) = 0;
  virtual void deleteAlias(int32_t alias) = 0;
  virtual void clearAlias(int32_t alias) = 0;
  virtual void pushAlias(int32_t alias, const GeometryInstance& newInstance) = 0;
  virtual void sortAlias(int32_t alias, const gero::graphics::ICamera& sceneCamera) = 0;
  virtual void preAllocateAlias(int32_t alias, std::size_t newSize) = 0;

public:
  virtual void clearAll() = 0;
  virtual void renderAll(bool clearAll = true) = 0;
};

class GeometriesStackRenderer : public IGeometriesStackRenderer, public gero::NonCopyable {
public:
  using GeometryInstance = IGeometriesStackRenderer::GeometryInstance;
  using InstanceVertices = std::vector<GeometryInstance>;

public:
  GeometriesStackRenderer() = default;
  ~GeometriesStackRenderer() = default;

public:
  void initialize();
  void setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) override;

public:
  void createAlias(int32_t alias, const gero::graphics::MakeGeometries::Vertices& vertices) override;
  void deleteAlias(int32_t alias) override;
  void clearAlias(int32_t alias) override;
  void pushAlias(int32_t alias, const GeometryInstance& newInstance) override;
  void sortAlias(int32_t alias, const gero::graphics::ICamera& sceneCamera) override;
  void preAllocateAlias(int32_t alias, std::size_t newSize) override;

public:
  void clearAll() override;
  void renderAll(bool clearAll = true) override;

private:
  std::shared_ptr<gero::graphics::ShaderProgram> _shader = nullptr;
  gero::graphics::Geometry::Definition _geomDef;
  gero::graphics::Camera::MatricesData _matricesData;

private:
  struct AliasedGeometry {
    gero::graphics::Geometry geometry;
    InstanceVertices instanceVertices;
  };

  std::unordered_map<int32_t, std::shared_ptr<AliasedGeometry>> _aliasedGeometriesMap;

  bool _strictMode = true;
};
