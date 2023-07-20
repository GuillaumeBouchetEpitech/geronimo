
#include "AbstractPhysicHingeConstraintManager.hpp"

#include "internals/PhysicHingeConstraintManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicHingeConstraintManager>
AbstractPhysicHingeConstraintManager::create(PhysicWorld& inPhysicWorld) {
  return std::make_unique<PhysicHingeConstraintManager>(inPhysicWorld);
}

} // namespace physics
} // namespace gero
