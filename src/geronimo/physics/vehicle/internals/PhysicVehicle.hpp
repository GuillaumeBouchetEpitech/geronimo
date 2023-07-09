
#pragma once

#include "../AbstractPhysicVehicle.hpp"
#include "../PhysicVehicleDef.hpp"

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

class btDiscreteDynamicsWorld;
class btDefaultVehicleRaycaster;
class btRaycastVehicle;

namespace gero {
namespace physics {

class PhysicWorld;
class PhysicVehicleManager;

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
  PhysicVehicle(PhysicVehicle&& other);
  PhysicVehicle& operator=(const PhysicVehicle& other) = delete;
  PhysicVehicle& operator=(PhysicVehicle&& other);

public:
  void applyEngineForce(int32_t index, float force) override;
  void applyBrake(int32_t index, float force) override;
  void setSteeringValue(int32_t index, float steering) override;
  void reset() override;

public:
  int32_t getNumWheels() const override;
  const glm::mat4& getWheelTransform(int32_t index, glm::mat4& mat4x4) const override;
  glm::vec3 getWheelPosition(int32_t index) const override;
  glm::quat getWheelOrientation(int32_t index) const override;
  float getCurrentSpeedKmHour() const override;
  BodyWeakRef getPhysicBody() override;
  const BodyWeakRef getPhysicBody() const override;
};

using VehiclesPool = safe_weak_ref_data_pool<PhysicVehicle, AbstractPhysicVehicle, 256, false>;

} // namespace physics
} // namespace gero
