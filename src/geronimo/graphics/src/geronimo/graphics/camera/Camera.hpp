
#pragma once

#include "FrustumCulling.hpp"

#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace graphics {

class ICamera {
public:
  struct MatricesData {
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 composed;
    glm::mat4 invComposed;
  };

public:
  enum class ProjectionType {
    perspective,
    orthographic,
  };

  struct Perspective {
    float fovy;
    float near;
    float far;
  };

  struct Orthographic {
    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;
  };

  union ProjectionData {
    Perspective perspective;
    Orthographic orthographic;
  };

public:
  virtual bool sceneToHudCoord(const glm::vec3& scenePos, glm::vec3& hudPos) const = 0;
  virtual void hudToSceneCoord(const glm::vec2& pos2d, glm::vec3& from, glm::vec3& to) const = 0;

public:
  virtual const IFrustumCulling& getFrustumCulling() const = 0;

public:
  virtual const glm::vec3& getEye() const = 0;
  virtual const glm::vec3& getTarget() const = 0;
  virtual const glm::vec3& getUp() const = 0;
  virtual const glm::vec3& getForwardAxis() const = 0;

  virtual const glm::vec2& getSize() const = 0;

  virtual ProjectionType getProjectionType() const = 0;
  virtual const ProjectionData& getProjectionData() const = 0;

public:
  virtual const MatricesData& getMatricesData() const = 0;
};

class Camera : public ICamera {
public:
  Camera() = default;
  ~Camera() = default;

public:
  void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
  void computeMatrices();

  const IFrustumCulling& getFrustumCulling() const override;

  bool sceneToHudCoord(const glm::vec3& scenePos, glm::vec3& hudPos) const override;
  void hudToSceneCoord(const glm::vec2& pos2d, glm::vec3& from, glm::vec3& to) const override;

public:
  const glm::vec3& getEye() const override;
  const glm::vec3& getTarget() const override;
  const glm::vec3& getUp() const override;
  const glm::vec3& getForwardAxis() const override;

  void setSize(uint32_t width, uint32_t height);
  const glm::vec2& getSize() const override;

  void setPerspective(float fovy, float near, float far);
  void setOrthographic(float left, float right, float bottom, float top, float near, float far);
  ProjectionType getProjectionType() const override;
  const ProjectionData& getProjectionData() const override;

public:
  const MatricesData& getMatricesData() const override;

private:
  glm::vec2 _viewportSize = {800.0f, 600.0f};

  glm::vec3 _eye = {0.0f, 0.0f, 0.0f};
  glm::vec3 _target = {1.0f, 0.0f, 0.0f};
  glm::vec3 _up = {0.0f, 0.0f, 1.0f};

  glm::vec3 _forwardAxis = {1.0f, 0.0f, 0.0f};

  bool _dirtyProjectionMatrices = true;

  ICamera::MatricesData _matricesData;

  ICamera::ProjectionType _projectionType;
  ICamera::ProjectionData _projectionData;

  FrustumCulling _frustumCulling;
};

} // namespace graphics
} // namespace gero
