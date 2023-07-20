
#include "AbstractPhysicVehicleManager.hpp"

#include "internals/PhysicVehicleManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicVehicleManager> AbstractPhysicVehicleManager::create(PhysicWorld& inPhysicWorld) {
  return std::make_unique<PhysicVehicleManager>(inPhysicWorld);
}

} // namespace physics
} // namespace gero
