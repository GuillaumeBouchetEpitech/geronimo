
#include "AbstractPhysicWorld.hpp"

#include "world/PhysicWorld.hpp"

namespace gero {
namespace physics {

std::unique_ptr<AbstractPhysicWorld> AbstractPhysicWorld::create() { return std::make_unique<PhysicWorld>(); }

} // namespace physics
} // namespace gero
