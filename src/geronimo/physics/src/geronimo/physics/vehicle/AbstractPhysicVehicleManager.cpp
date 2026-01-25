
#include "AbstractPhysicVehicleManager.hpp"

#include "internals/PhysicVehicleManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicVehicleManager> AbstractPhysicVehicleManager::create(AbstractPhysicWorld& inPhysicWorld,
                                                                                   std::size_t pre_allocated_size) {
  return std::make_unique<PhysicVehicleManager>(inPhysicWorld, pre_allocated_size);
}

} // namespace physics
} // namespace gero
