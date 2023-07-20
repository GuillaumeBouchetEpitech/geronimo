
#pragma once

#include "../AbstractPhysicSixDofConstraintManager.hpp"
#include "PhysicSixDofConstraint.hpp"

namespace gero {
namespace physics {

class PhysicWorld;

class PhysicSixDofConstraintManager : public AbstractPhysicSixDofConstraintManager {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;
  PhysicSixDofConstraintsPool _pool;

  uint32_t _totalLive = 0;

public:
  PhysicSixDofConstraintManager(PhysicWorld& physicWorld);
  ~PhysicSixDofConstraintManager();

public:
  void clear() override;

public:
  SixDofConstraintWeakRef create(const PhysicSixDofConstraintDef& def) override;
  SixDofConstraintWeakRef createAndAdd(const PhysicSixDofConstraintDef& def) override;
  void destroy(SixDofConstraintWeakRef ref) override;

  void add(SixDofConstraintWeakRef ref) override;
  void remove(SixDofConstraintWeakRef ref) override;

  SixDofConstraintWeakRef get(uint32_t index) override;
  const SixDofConstraintWeakRef get(uint32_t index) const override;
  std::size_t size() const override;
  uint32_t totalLive() const override;
  bool isEmpty() const override;
};

} // namespace physics
} // namespace gero
