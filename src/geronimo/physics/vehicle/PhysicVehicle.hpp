
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

class AbstractPhysicVehicle {
public:
  AbstractPhysicVehicle() = default;
  virtual ~AbstractPhysicVehicle() = default;
  AbstractPhysicVehicle(AbstractPhysicVehicle& other) = delete;
  AbstractPhysicVehicle& operator=(const AbstractPhysicVehicle& other) = delete;
  AbstractPhysicVehicle&
  operator=(const AbstractPhysicVehicle&& other) = delete;

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
  virtual PhysicBodyManager::BodyWeakRef getPhysicBody() = 0;
};

class PhysicVehicle : public AbstractPhysicVehicle {
  friend PhysicWorld;
  friend PhysicVehicleManager;

private:
  struct Bullet {
    btDefaultVehicleRaycaster* vehicleRayCaster = nullptr;
    btRaycastVehicle* vehicle = nullptr;
  } _bullet;

  PhysicBodyManager::BodyWeakRef _body;

  bool _isAdded = false;

public:
  PhysicVehicle(btDiscreteDynamicsWorld& dynamicsWorld,
                const PhysicVehicleDef& def);
  virtual ~PhysicVehicle();

  PhysicVehicle(PhysicVehicle& other) = delete;
  PhysicVehicle(PhysicVehicle&& other);
  PhysicVehicle& operator=(const PhysicVehicle& other) = delete;
  PhysicVehicle& operator=(PhysicVehicle&& other);

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
  virtual PhysicBodyManager::BodyWeakRef getPhysicBody() override;
};

} // namespace physics
} // namespace gero
