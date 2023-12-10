
#pragma once

#include "../AbstractPhysicHingeConstraintManager.hpp"
#include "PhysicHingeConstraint.hpp"

namespace gero {
namespace physics {

class AbstractPhysicWorld;

class PhysicHingeConstraintManager : public AbstractPhysicHingeConstraintManager {
  friend AbstractPhysicWorld;

private:
  AbstractPhysicWorld& _physicWorld;
  PhysicHingeConstraintsPool _hingeConstraints;

  uint32_t _totalLiveHingeConstraints = 0;

public:
  PhysicHingeConstraintManager(AbstractPhysicWorld& physicWorld, std::size_t pre_allocated_size = 256);
  ~PhysicHingeConstraintManager();

public:
  void clear() override;

public:
  HingeConstraintWeakRef create(const PhysicHingeConstraintDef& def) override;
  HingeConstraintWeakRef createAndAdd(const PhysicHingeConstraintDef& def) override;
  void destroy(HingeConstraintWeakRef ref) override;

  void add(HingeConstraintWeakRef ref) override;
  void remove(HingeConstraintWeakRef ref) override;

  HingeConstraintWeakRef get(uint32_t index) override;
  const HingeConstraintWeakRef get(uint32_t index) const override;
  std::size_t size() const override;
  uint32_t totalLive() const override;
  bool isEmpty() const override;
};

} // namespace physics
} // namespace gero
