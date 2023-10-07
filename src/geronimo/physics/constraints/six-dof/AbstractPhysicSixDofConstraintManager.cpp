
#include "AbstractPhysicSixDofConstraintManager.hpp"

#include "internals/PhysicSixDofConstraintManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicSixDofConstraintManager>
AbstractPhysicSixDofConstraintManager::create(PhysicWorld& inPhysicWorld) {
  return std::make_unique<PhysicSixDofConstraintManager>(inPhysicWorld);
}

} // namespace physics
} // namespace gero
