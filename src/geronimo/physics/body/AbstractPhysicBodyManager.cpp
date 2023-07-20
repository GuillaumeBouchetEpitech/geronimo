
#include "AbstractPhysicBodyManager.hpp"

#include "internals/PhysicBodyManager.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicBodyManager> AbstractPhysicBodyManager::create(PhysicWorld& inPhysicWorld)
{
  return std::make_unique<PhysicBodyManager>(inPhysicWorld);
}

} // namespace physics
} // namespace gero
