
#pragma once

#include "../AbstractPhysicUniversalConstraintManager.hpp"
#include "PhysicUniversalConstraint.hpp"

namespace gero {
namespace physics {

class PhysicWorld;

class PhysicUniversalConstraintManager : public AbstractPhysicUniversalConstraintManager {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;
  PhysicUniversalConstraintsPool _universalConstraints;

  uint32_t _totalLiveUniversalConstraints = 0;

public:
  PhysicUniversalConstraintManager(PhysicWorld& physicWorld);
  ~PhysicUniversalConstraintManager();

public:
  void clear() override;

public:
  UniversalConstraintWeakRef create(const PhysicUniversalConstraintDef& def) override;
  UniversalConstraintWeakRef createAndAdd(const PhysicUniversalConstraintDef& def) override;
  void destroy(UniversalConstraintWeakRef ref) override;

  void add(UniversalConstraintWeakRef ref) override;
  void remove(UniversalConstraintWeakRef ref) override;

  UniversalConstraintWeakRef get(uint32_t index) override;
  const UniversalConstraintWeakRef get(uint32_t index) const override;
  std::size_t size() const override;
  uint32_t totalLive() const override;
  bool isEmpty() const override;
};

} // namespace physics
} // namespace gero
