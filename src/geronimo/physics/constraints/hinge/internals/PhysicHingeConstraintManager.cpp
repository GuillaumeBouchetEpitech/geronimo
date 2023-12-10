
#include "PhysicHingeConstraintManager.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/AbstractPhysicWorld.hpp"

namespace gero {
namespace physics {

PhysicHingeConstraintManager::PhysicHingeConstraintManager(AbstractPhysicWorld& physicWorld, std::size_t pre_allocated_size) : _physicWorld(physicWorld) {
  _hingeConstraints.pre_allocate(pre_allocated_size);
}

PhysicHingeConstraintManager::~PhysicHingeConstraintManager() { clear(); }

void PhysicHingeConstraintManager::clear() {
  while (!_hingeConstraints.is_empty())
    destroy(_hingeConstraints.get(0));
  _hingeConstraints.clear();
}

HingeConstraintWeakRef PhysicHingeConstraintManager::create(const PhysicHingeConstraintDef& def) {
  return _hingeConstraints.acquire(def);
}

HingeConstraintWeakRef PhysicHingeConstraintManager::createAndAdd(const PhysicHingeConstraintDef& def) {
  auto ref = create(def);
  add(ref);
  return ref;
}

void PhysicHingeConstraintManager::destroy(HingeConstraintWeakRef ref) {
  if (!ref)
    return;

  remove(ref);
  _hingeConstraints.release(ref);
}

void PhysicHingeConstraintManager::add(HingeConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicHingeConstraint* implementation = reinterpret_cast<PhysicHingeConstraint*>(ref.get());
  if (implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->addConstraint(implementation->_bullet.constraint, true);
  implementation->_isAdded = true;

  _totalLiveHingeConstraints += 1;
}

void PhysicHingeConstraintManager::remove(HingeConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicHingeConstraint* implementation = reinterpret_cast<PhysicHingeConstraint*>(ref.get());
  if (!implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->removeConstraint(implementation->_bullet.constraint);
  implementation->_isAdded = false;

  _totalLiveHingeConstraints -= 1;
}

HingeConstraintWeakRef PhysicHingeConstraintManager::get(uint32_t index) { return _hingeConstraints.get(index); }

const HingeConstraintWeakRef PhysicHingeConstraintManager::get(uint32_t index) const {
  return _hingeConstraints.get(index);
}

std::size_t PhysicHingeConstraintManager::size() const { return _hingeConstraints.size(); }

uint32_t PhysicHingeConstraintManager::totalLive() const { return _totalLiveHingeConstraints; }

bool PhysicHingeConstraintManager::isEmpty() const { return _hingeConstraints.is_empty(); }

} // namespace physics
} // namespace gero
