
#pragma once

#include "AbstractPhysicVehicle.hpp"
#include "PhysicVehicleDef.hpp"

namespace gero {
namespace physics {

class AbstractPhysicWorld;

class AbstractPhysicVehicleManager {

public:
  virtual ~AbstractPhysicVehicleManager() = default;

public:
  virtual void clear() = 0;

public:
  virtual VehicleWeakRef createVehicle(const PhysicVehicleDef& def) = 0;
  virtual VehicleWeakRef createAndAddVehicle(const PhysicVehicleDef& def) = 0;
  virtual void destroyVehicle(VehicleWeakRef ref) = 0;

  virtual void addVehicle(VehicleWeakRef ref) = 0;
  virtual void removeVehicle(VehicleWeakRef ref) = 0;

  virtual VehicleWeakRef getVehicle(uint32_t index) = 0;
  virtual const VehicleWeakRef getVehicle(uint32_t index) const = 0;
  virtual std::size_t vehicleSize() const = 0;
  virtual uint32_t totalLiveVehicles() const = 0;
  virtual bool vehicleEmpty() const = 0;

public:
  static std::unique_ptr<AbstractPhysicVehicleManager> create(AbstractPhysicWorld& inPhysicWorld,
                                                              std::size_t pre_allocated_size = 256);
};

} // namespace physics
} // namespace gero
