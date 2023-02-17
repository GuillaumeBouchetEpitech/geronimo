
#pragma once

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

#include "PhysicVehicle.hpp"
#include "PhysicVehicleDef.hpp"

namespace gero {
namespace physics {

class PhysicWorld;

class PhysicVehicleManager {
  friend PhysicWorld;

public:
  using VehiclesPool =
    weak_ref_data_pool<PhysicVehicle, AbstractPhysicVehicle, 256, false>;
  using VehicleWeakRef = VehiclesPool::weak_ref;

private:
  PhysicWorld& _physicWorld;
  VehiclesPool _vehicles;

  uint32_t _totalLiveVehicles = 0;

private:
  PhysicVehicleManager(PhysicWorld& physicWorld);
  ~PhysicVehicleManager();

private:
  void clear();

public:
  VehicleWeakRef createVehicle(const PhysicVehicleDef& def);
  VehicleWeakRef createAndAddVehicle(const PhysicVehicleDef& def);
  void destroyVehicle(VehicleWeakRef ref);

  void addVehicle(VehicleWeakRef ref);
  void removeVehicle(VehicleWeakRef ref);

  VehicleWeakRef getVehicle(uint32_t index);
  const VehicleWeakRef getVehicle(uint32_t index) const;
  std::size_t vehicleSize() const;
  uint32_t totalLiveVehicles() const;
  bool vehicleEmpty() const;
};

} // namespace physics
} // namespace gero
