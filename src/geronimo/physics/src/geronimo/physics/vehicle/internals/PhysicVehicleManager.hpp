
#pragma once

#include "../AbstractPhysicVehicleManager.hpp"
#include "PhysicVehicle.hpp"

namespace gero {
namespace physics {

class AbstractPhysicWorld;

class PhysicVehicleManager : public AbstractPhysicVehicleManager {
  friend AbstractPhysicWorld;

private:
  AbstractPhysicWorld& _physicWorld;
  VehiclesPool _vehicles;

  uint32_t _totalLiveVehicles = 0;

public:
  PhysicVehicleManager(AbstractPhysicWorld& physicWorld, std::size_t pre_allocated_size = 256);
  ~PhysicVehicleManager();

public:
  void clear() override;

public:
  VehicleWeakRef createVehicle(const PhysicVehicleDef& def) override;
  VehicleWeakRef createAndAddVehicle(const PhysicVehicleDef& def) override;
  void destroyVehicle(VehicleWeakRef ref) override;

  void addVehicle(VehicleWeakRef ref) override;
  void removeVehicle(VehicleWeakRef ref) override;

  VehicleWeakRef getVehicle(uint32_t index) override;
  const VehicleWeakRef getVehicle(uint32_t index) const override;
  std::size_t vehicleSize() const override;
  uint32_t totalLiveVehicles() const override;
  bool vehicleEmpty() const override;
};

} // namespace physics
} // namespace gero
