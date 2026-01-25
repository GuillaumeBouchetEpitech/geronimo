
#include "AbstractPhysicSixDofConstraintManager.hpp"

#include "internals/PhysicSixDofConstraintManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicSixDofConstraintManager>
AbstractPhysicSixDofConstraintManager::create(AbstractPhysicWorld& inPhysicWorld, std::size_t pre_allocated_size) {
  return std::make_unique<PhysicSixDofConstraintManager>(inPhysicWorld, pre_allocated_size);
}

} // namespace physics
} // namespace gero
