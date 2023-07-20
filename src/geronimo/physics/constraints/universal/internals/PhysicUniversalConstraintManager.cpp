
#include "PhysicUniversalConstraintManager.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"

namespace gero {
namespace physics {

PhysicUniversalConstraintManager::PhysicUniversalConstraintManager(PhysicWorld& physicWorld) : _physicWorld(physicWorld) {
  _universalConstraints.pre_allocate(1024);
}

PhysicUniversalConstraintManager::~PhysicUniversalConstraintManager() { clear(); }

void PhysicUniversalConstraintManager::clear() {
  while (!_universalConstraints.is_empty())
    destroy(_universalConstraints.get(0));
  _universalConstraints.clear();
}

UniversalConstraintWeakRef PhysicUniversalConstraintManager::create(const PhysicUniversalConstraintDef& def) {
  return _universalConstraints.acquire(def);
}

UniversalConstraintWeakRef PhysicUniversalConstraintManager::createAndAdd(const PhysicUniversalConstraintDef& def) {
  auto ref = create(def);
  add(ref);
  return ref;
}

void PhysicUniversalConstraintManager::destroy(UniversalConstraintWeakRef ref) {
  if (!ref)
    return;

  remove(ref);
  _universalConstraints.release(ref);
}

void PhysicUniversalConstraintManager::add(UniversalConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicUniversalConstraint* implementation = reinterpret_cast<PhysicUniversalConstraint*>(ref.get());
  if (implementation->_isAdded)
    return;
  _physicWorld._bullet.dynamicsWorld->addConstraint(implementation->_bullet.constraint, true);
  implementation->_isAdded = true;

  _totalLiveUniversalConstraints += 1;
}

void PhysicUniversalConstraintManager::remove(UniversalConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicUniversalConstraint* implementation = reinterpret_cast<PhysicUniversalConstraint*>(ref.get());
  if (!implementation->_isAdded)
    return;
  _physicWorld._bullet.dynamicsWorld->removeConstraint(implementation->_bullet.constraint);
  implementation->_isAdded = false;

  _totalLiveUniversalConstraints -= 1;
}

UniversalConstraintWeakRef PhysicUniversalConstraintManager::get(uint32_t index) { return _universalConstraints.get(index); }

const UniversalConstraintWeakRef PhysicUniversalConstraintManager::get(uint32_t index) const { return _universalConstraints.get(index); }

std::size_t PhysicUniversalConstraintManager::size() const { return _universalConstraints.size(); }

uint32_t PhysicUniversalConstraintManager::totalLive() const { return _totalLiveUniversalConstraints; }

bool PhysicUniversalConstraintManager::isEmpty() const { return _universalConstraints.is_empty(); }

} // namespace physics
} // namespace gero
