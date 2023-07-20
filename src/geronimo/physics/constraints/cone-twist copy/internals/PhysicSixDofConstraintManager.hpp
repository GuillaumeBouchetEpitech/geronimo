
#pragma once

#include "../AbstractPhysicConeTwistConstraintManager.hpp"
#include "PhysicConeTwistConstraint.hpp"

namespace gero {
namespace physics {

class PhysicWorld;

class PhysicConeTwistConstraintManager : public AbstractPhysicConeTwistConstraintManager {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;
  PhysicConeTwistConstraintsPool _pool;

  uint32_t _totalLive = 0;

public:
  PhysicConeTwistConstraintManager(PhysicWorld& physicWorld);
  ~PhysicConeTwistConstraintManager();

public:
  void clear() override;

public:
  PhysicConeTwistConstraintWeakRef create(const PhysicConeTwistConstraintDef& def) override;
  PhysicConeTwistConstraintWeakRef createAndAdd(const PhysicConeTwistConstraintDef& def) override;
  void destroy(PhysicConeTwistConstraintWeakRef ref) override;

  void add(PhysicConeTwistConstraintWeakRef ref) override;
  void remove(PhysicConeTwistConstraintWeakRef ref) override;

  PhysicConeTwistConstraintWeakRef get(uint32_t index) override;
  const PhysicConeTwistConstraintWeakRef get(uint32_t index) const override;
  std::size_t size() const override;
  uint32_t totalLive() const override;
  bool isEmpty() const override;
};

} // namespace physics
} // namespace gero
