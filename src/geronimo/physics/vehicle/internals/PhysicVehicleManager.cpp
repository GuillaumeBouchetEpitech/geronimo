
#include "PhysicVehicleManager.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/AbstractPhysicWorld.hpp"

namespace gero {
namespace physics {

PhysicVehicleManager::PhysicVehicleManager(AbstractPhysicWorld& physicWorld, std::size_t pre_allocated_size)
  : _physicWorld(physicWorld) {
  _vehicles.pre_allocate(pre_allocated_size);
}

PhysicVehicleManager::~PhysicVehicleManager() { clear(); }

void PhysicVehicleManager::clear() {
  while (!_vehicles.is_empty())
    destroyVehicle(_vehicles.get(0));
  _vehicles.clear();
}

VehicleWeakRef PhysicVehicleManager::createVehicle(const PhysicVehicleDef& def) {
  return _vehicles.acquire(*_physicWorld.getRawDynamicsWorld(), def);
}

VehicleWeakRef PhysicVehicleManager::createAndAddVehicle(const PhysicVehicleDef& def) {
  auto ref = createVehicle(def);
  addVehicle(ref);
  return ref;
}

void PhysicVehicleManager::destroyVehicle(VehicleWeakRef ref) {
  if (!ref)
    return;

  removeVehicle(ref);
  _vehicles.release(ref);
}

void PhysicVehicleManager::addVehicle(VehicleWeakRef ref) {
  if (!ref)
    return;
  PhysicVehicle* implementation = reinterpret_cast<PhysicVehicle*>(ref.get());
  if (implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->addVehicle(implementation->_bullet.vehicle);
  implementation->_isAdded = true;

  _totalLiveVehicles += 1;
}

void PhysicVehicleManager::removeVehicle(VehicleWeakRef ref) {
  if (!ref)
    return;
  PhysicVehicle* implementation = reinterpret_cast<PhysicVehicle*>(ref.get());
  if (!implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->removeVehicle(implementation->_bullet.vehicle);
  implementation->_isAdded = false;

  _totalLiveVehicles -= 1;
}

VehicleWeakRef PhysicVehicleManager::getVehicle(uint32_t index) { return _vehicles.get(index); }

const VehicleWeakRef PhysicVehicleManager::getVehicle(uint32_t index) const { return _vehicles.get(index); }

std::size_t PhysicVehicleManager::vehicleSize() const { return _vehicles.size(); }

uint32_t PhysicVehicleManager::totalLiveVehicles() const { return _totalLiveVehicles; }

bool PhysicVehicleManager::vehicleEmpty() const { return _vehicles.is_empty(); }

} // namespace physics
} // namespace gero
