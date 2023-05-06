
#pragma once

#include "PhysicBodyDef.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/shape/PhysicShape.hpp"
#include "geronimo/system/containers/dynamic_heap_array.hpp"
#include "geronimo/system/containers/weak_ref_data_pool.hpp"

// #include <vector>
#include <cstdint>
#include <functional>
#include <memory>

class btDefaultMotionState;
class btRigidBody;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicBodyManager;
class PhysicVehicle;
// class PhysicHingeConstraint;
// class PhysicFixedConstraint;

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
  virtual void setRotation(const glm::vec4& rot) = 0;
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

class PhysicBody : public AbstractPhysicBody {

  friend PhysicWorld;
  friend PhysicBodyManager;
  friend PhysicVehicle;

private:
  PhysicShapeDef _shapeDef;

  struct Bullet {
    btDefaultMotionState* motionState = nullptr;
    btRigidBody* body = nullptr;
  } _bullet;

  PhysicShape* _shape{nullptr};

  float _mass = 0.0f;

  // std::vector<ContactPoint> _contacts;
  // std::function<void()> _contactCallback;

  int32_t _userValue = 0;
  void* _userData = nullptr;

  bool _isAdded = false;

  // private:
public:
  PhysicBody(const PhysicBodyDef& def);
  virtual ~PhysicBody();

  PhysicBody(PhysicBody& other) = delete;
  PhysicBody(PhysicBody&& other);
  PhysicBody& operator=(const PhysicBody& other) = delete;
  PhysicBody& operator=(PhysicBody&& other);

public:
  bool isDynamic() const override;

  glm::vec3 getPosition() const override;
  glm::quat getOrientation() const override;
  glm::mat4& getTransform(glm::mat4& mat4x4) const override;

  void setPosition(const glm::vec3& pos) override;
  void setRotation(const glm::vec4& rot) override;
  void setTransform(const glm::mat4& mat4x4) override;

  glm::vec3 getEulerXYZ() const override;
  void setEulerXYZ(float x, float y, float z) override;
  void setEulerXYZ(const glm::vec3& euler) override;

  void setAngularFactor(float x, float y, float z) override;
  void setAngularFactor(const glm::vec3& factor) override;

  void clearForces() override;

  glm::vec3 getLinearVelocity() const override;
  glm::vec3 getAngularVelocity() const override;
  void setLinearVelocity(float x, float y, float z) override;
  void setLinearVelocity(const glm::vec3& vel) override;
  void setAngularVelocity(float x, float y, float z) override;
  void setAngularVelocity(const glm::vec3& vel) override;

  void setGravity(float x, float y, float z) override;
  void setGravity(const glm::vec3& grav) override;
  glm::vec3 getGravity() const override;

  void setFriction(float friction) override;
  float getFriction() const override;

  void setUserValue(int32_t userValue) override;
  int32_t getUserValue() const override;

  void setUserData(void* userData) override;
  const void* getUserData() const override;
  void* getUserData() override;

  void applyCentralImpulse(const glm::vec3& acc) override;
  void applyImpulse(const glm::vec3& force, const glm::vec3& rel_pos) override;
  void applyCentralForce(const glm::vec3& force) override;
  void applyForce(const glm::vec3& force, const glm::vec3& rel_pos) override;
  void disableSleep() override;
  void forceActivate() override;

  void disableContactResponse() override;
  void enableContactResponse() override;

  void dumpData() override;

  const PhysicShapeDef& getShapeDefinition() const override;

  // const std::vector<ContactPoint>& getContacts() const;
  // void setContactCallback(const std::function<void()>& callback);
};

using BodyContainer = safe_weak_ref_data_pool<PhysicBody, AbstractPhysicBody, 256, false>;
using BodyWeakRef = BodyContainer::weak_ref;

} // namespace physics
} // namespace gero
