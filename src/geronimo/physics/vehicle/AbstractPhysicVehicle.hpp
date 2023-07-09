
#pragma once

#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"
#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace physics {

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

using VehicleWeakRef = data_pool_weak_ref<AbstractPhysicVehicle>;

} // namespace physics
} // namespace gero
