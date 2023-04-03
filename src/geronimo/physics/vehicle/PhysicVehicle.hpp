
#pragma once

#include "PhysicVehicleDef.hpp"

#include "geronimo/physics/body/PhysicBodyManager.hpp"

class btDiscreteDynamicsWorld;
class btDefaultVehicleRaycaster;
class btRaycastVehicle;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicVehicleManager;

class AbstractPhysicVehicle : public gero::weak_ref_data_pool_base_class {
public:
  AbstractPhysicVehicle() = default;
  virtual ~AbstractPhysicVehicle() = default;
  AbstractPhysicVehicle(const AbstractPhysicVehicle& other) = delete;
  AbstractPhysicVehicle(AbstractPhysicVehicle&& other) = default;
  AbstractPhysicVehicle& operator=(const AbstractPhysicVehicle& other) = delete;
  AbstractPhysicVehicle& operator=(AbstractPhysicVehicle&& other) = default;

public:
  virtual void applyEngineForce(int32_t index, float force) = 0;
  virtual void applyBrake(int32_t index, float force) = 0;
  virtual void setSteeringValue(int32_t index, float steering) = 0;
  virtual void reset() = 0;

public:
  virtual int32_t getNumWheels() const = 0;
  virtual const glm::mat4& getWheelTransform(int32_t index, glm::mat4& mat4x4) const = 0;
  virtual glm::vec3 getWheelPosition(int32_t index) const = 0;
  virtual glm::quat getWheelOrientation(int32_t index) const = 0;
  virtual float getCurrentSpeedKmHour() const = 0;
  virtual BodyWeakRef getPhysicBody() = 0;
  virtual const BodyWeakRef getPhysicBody() const = 0;
};

class PhysicVehicle : public AbstractPhysicVehicle {
  friend PhysicWorld;
  friend PhysicVehicleManager;

private:
  struct Bullet {
    btDefaultVehicleRaycaster* vehicleRayCaster = nullptr;
    btRaycastVehicle* vehicle = nullptr;
  } _bullet;

  BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicVehicle(btDiscreteDynamicsWorld& dynamicsWorld, const PhysicVehicleDef& def);
  virtual ~PhysicVehicle();

  PhysicVehicle(const PhysicVehicle& other) = delete;
  PhysicVehicle(PhysicVehicle&& other) = default;
  PhysicVehicle& operator=(const PhysicVehicle& other) = delete;
  PhysicVehicle& operator=(PhysicVehicle&& other) = default;

public:
  virtual void applyEngineForce(int32_t index, float force) override;
  virtual void applyBrake(int32_t index, float force) override;
  virtual void setSteeringValue(int32_t index, float steering) override;
  virtual void reset() override;

public:
  virtual int32_t getNumWheels() const override;
  virtual const glm::mat4& getWheelTransform(int32_t index, glm::mat4& mat4x4) const override;
  virtual glm::vec3 getWheelPosition(int32_t index) const override;
  virtual glm::quat getWheelOrientation(int32_t index) const override;
  virtual float getCurrentSpeedKmHour() const override;
  virtual BodyWeakRef getPhysicBody() override;
  virtual const BodyWeakRef getPhysicBody() const override;
};

using VehiclesPool = safe_weak_ref_data_pool<PhysicVehicle, AbstractPhysicVehicle, 256, false>;
using VehicleWeakRef = VehiclesPool::weak_ref;

} // namespace physics
} // namespace gero
