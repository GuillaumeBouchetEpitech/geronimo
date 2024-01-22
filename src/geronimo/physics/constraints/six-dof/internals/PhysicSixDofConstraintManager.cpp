
#include "PhysicSixDofConstraintManager.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/AbstractPhysicWorld.hpp"

namespace gero {
namespace physics {

PhysicSixDofConstraintManager::PhysicSixDofConstraintManager(AbstractPhysicWorld& physicWorld,
                                                             std::size_t pre_allocated_size)
  : _physicWorld(physicWorld) {
  _pool.pre_allocate(pre_allocated_size);
}

PhysicSixDofConstraintManager::~PhysicSixDofConstraintManager() { clear(); }

void PhysicSixDofConstraintManager::clear() {
  while (!_pool.is_empty())
    destroy(_pool.get(0));
  _pool.clear();
}

SixDofConstraintWeakRef PhysicSixDofConstraintManager::create(const PhysicSixDofConstraintDef& def) {
  return _pool.acquire(def);
}

SixDofConstraintWeakRef PhysicSixDofConstraintManager::createAndAdd(const PhysicSixDofConstraintDef& def) {
  auto ref = create(def);
  add(ref);
  return ref;
}

void PhysicSixDofConstraintManager::destroy(SixDofConstraintWeakRef ref) {
  if (!ref)
    return;

  remove(ref);
  _pool.release(ref);
}

void PhysicSixDofConstraintManager::add(SixDofConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicSixDofConstraint* implementation = reinterpret_cast<PhysicSixDofConstraint*>(ref.get());
  if (implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->addConstraint(implementation->_bullet.constraint, true);
  implementation->_isAdded = true;

  _totalLive += 1;
}

void PhysicSixDofConstraintManager::remove(SixDofConstraintWeakRef ref) {
  if (!ref)
    return;
  PhysicSixDofConstraint* implementation = reinterpret_cast<PhysicSixDofConstraint*>(ref.get());
  if (!implementation || !implementation->_isAdded)
    return;
  _physicWorld.getRawDynamicsWorld()->removeConstraint(implementation->_bullet.constraint);
  implementation->_isAdded = false;

  _totalLive -= 1;
}

SixDofConstraintWeakRef PhysicSixDofConstraintManager::get(uint32_t index) { return _pool.get(index); }

const SixDofConstraintWeakRef PhysicSixDofConstraintManager::get(uint32_t index) const { return _pool.get(index); }

std::size_t PhysicSixDofConstraintManager::size() const { return _pool.size(); }

uint32_t PhysicSixDofConstraintManager::totalLive() const { return _totalLive; }

bool PhysicSixDofConstraintManager::isEmpty() const { return _pool.is_empty(); }

} // namespace physics
} // namespace gero
