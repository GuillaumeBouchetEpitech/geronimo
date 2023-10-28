
#include "PhysicConeTwistConstraintManager.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"

namespace gero {
namespace physics {

PhysicConeTwistConstraintManager::PhysicConeTwistConstraintManager(PhysicWorld& physicWorld)
  : _physicWorld(physicWorld) {
  _pool.pre_allocate(1024);
}

PhysicConeTwistConstraintManager::~PhysicConeTwistConstraintManager() { clear(); }

void PhysicConeTwistConstraintManager::clear() {
  while (!_pool.is_empty())
    destroy(_pool.get(0));
  _pool.clear();
}

PhysicConeTwistConstraintWeakRef PhysicConeTwistConstraintManager::create(const PhysicConeTwistConstraintDef& def) {
  return _pool.acquire(def);
}

PhysicConeTwistConstraintWeakRef
PhysicConeTwistConstraintManager::createAndAdd(const PhysicConeTwistConstraintDef& def) {
  auto ref = create(def);
  add(ref);
  return ref;
}

void PhysicConeTwistConstraintManager::destroy(PhysicConeTwistConstraintWeakRef ref) {
  if (!ref)
    return;

  remove(ref);
  _pool.release(ref);
}

void PhysicConeTwistConstraintManager::add(PhysicConeTwistConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicConeTwistConstraint* implementation = reinterpret_cast<PhysicConeTwistConstraint*>(ref.get());
  if (implementation->_isAdded)
    return;
  _physicWorld._bullet.dynamicsWorld->addConstraint(implementation->_bullet.constraint, true);
  implementation->_isAdded = true;

  _totalLive += 1;
}

void PhysicConeTwistConstraintManager::remove(PhysicConeTwistConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicConeTwistConstraint* implementation = reinterpret_cast<PhysicConeTwistConstraint*>(ref.get());
  if (!implementation || !implementation->_isAdded)
    return;
  _physicWorld._bullet.dynamicsWorld->removeConstraint(implementation->_bullet.constraint);
  implementation->_isAdded = false;

  _totalLive -= 1;
}

PhysicConeTwistConstraintWeakRef PhysicConeTwistConstraintManager::get(uint32_t index) { return _pool.get(index); }

const PhysicConeTwistConstraintWeakRef PhysicConeTwistConstraintManager::get(uint32_t index) const {
  return _pool.get(index);
}

std::size_t PhysicConeTwistConstraintManager::size() const { return _pool.size(); }

uint32_t PhysicConeTwistConstraintManager::totalLive() const { return _totalLive; }

bool PhysicConeTwistConstraintManager::isEmpty() const { return _pool.is_empty(); }

} // namespace physics
} // namespace gero
