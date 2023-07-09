
#pragma once

#include "PhysicBodyDef.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/shape/PhysicShape.hpp"
#include "geronimo/system/containers/dynamic_heap_array.hpp"
#include "geronimo/system/containers/weak_ref_data_pool.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace gero {
namespace physics {

class AbstractPhysicBody : public gero::weak_ref_data_pool_base_class {
public:
  AbstractPhysicBody() = default;
  virtual ~AbstractPhysicBody() = default;
  AbstractPhysicBody(AbstractPhysicBody& other) = delete;
  AbstractPhysicBody& operator=(const AbstractPhysicBody& other) = delete;

public:
  virtual bool isDynamic() const = 0;

  virtual glm::vec3 getPosition() const = 0;
  virtual glm::quat getOrientation() const = 0;
  virtual glm::mat4& getTransform(glm::mat4& mat4x4) const = 0;

  virtual void setPosition(const glm::vec3& pos) = 0;
  virtual void setOrientation(const glm::vec4& rot) = 0;
  virtual void setOrientation(const glm::quat& rot) = 0;
  virtual void setTransform(const glm::mat4& mat4x4) = 0;

  virtual glm::vec3 getEulerXYZ() const = 0;
  virtual void setEulerXYZ(float x, float y, float z) = 0;
  virtual void setEulerXYZ(const glm::vec3& euler) = 0;

  virtual void setAngularFactor(float x, float y, float z) = 0;
  virtual void setAngularFactor(const glm::vec3& factor) = 0;

  virtual void clearForces() = 0;

  virtual glm::vec3 getLinearVelocity() const = 0;
  virtual glm::vec3 getAngularVelocity() const = 0;
  virtual void setLinearVelocity(float x, float y, float z) = 0;
  virtual void setLinearVelocity(const glm::vec3& vel) = 0;
  virtual void setAngularVelocity(float x, float y, float z) = 0;
  virtual void setAngularVelocity(const glm::vec3& vel) = 0;

  virtual void setGravity(float x, float y, float z) = 0;
  virtual void setGravity(const glm::vec3& grav) = 0;
  virtual glm::vec3 getGravity() const = 0;

  virtual void setFriction(float friction) = 0;
  virtual float getFriction() const = 0;

  virtual void setUserValue(int32_t userValue) = 0;
  virtual int32_t getUserValue() const = 0;

  virtual void setUserData(void* userData) = 0;
  virtual const void* getUserData() const = 0;
  virtual void* getUserData() = 0;

  virtual void applyCentralImpulse(const glm::vec3& acc) = 0;
  virtual void applyImpulse(const glm::vec3& force, const glm::vec3& rel_pos) = 0;
  virtual void applyCentralForce(const glm::vec3& force) = 0;
  virtual void applyForce(const glm::vec3& force, const glm::vec3& rel_pos) = 0;
  virtual void disableSleep() = 0;
  virtual void forceActivate() = 0;

  virtual void disableContactResponse() = 0;
  virtual void enableContactResponse() = 0;

  virtual void dumpData() = 0;

  virtual const PhysicShapeDef& getShapeDefinition() const = 0;
};

using BodyWeakRef = data_pool_weak_ref<AbstractPhysicBody>;

} // namespace physics
} // namespace gero
