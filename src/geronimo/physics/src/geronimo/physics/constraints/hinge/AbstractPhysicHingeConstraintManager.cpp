
#include "AbstractPhysicHingeConstraintManager.hpp"

#include "internals/PhysicHingeConstraintManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicHingeConstraintManager>
AbstractPhysicHingeConstraintManager::create(AbstractPhysicWorld& inPhysicWorld, std::size_t pre_allocated_size) {
  return std::make_unique<PhysicHingeConstraintManager>(inPhysicWorld, pre_allocated_size);
}

} // namespace physics
} // namespace gero
