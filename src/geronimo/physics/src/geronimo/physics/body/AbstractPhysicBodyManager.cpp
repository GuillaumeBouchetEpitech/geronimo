
#include "AbstractPhysicBodyManager.hpp"

#include "internals/PhysicBodyManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicBodyManager> AbstractPhysicBodyManager::create(AbstractPhysicWorld& inPhysicWorld,
                                                                             std::size_t pre_allocated_size) {
  return std::make_unique<PhysicBodyManager>(inPhysicWorld, pre_allocated_size);
}

} // namespace physics
} // namespace gero
