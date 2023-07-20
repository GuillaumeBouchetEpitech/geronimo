
#pragma once

#include "AbstractPhysicConeTwistConstraint.hpp"
#include "PhysicConeTwistConstraintDef.hpp"

namespace gero {
namespace physics {

class AbstractPhysicConeTwistConstraintManager {

public:
  virtual ~AbstractPhysicConeTwistConstraintManager() = default;

public:
  virtual void clear() = 0;

public:
  virtual PhysicConeTwistConstraintWeakRef create(const PhysicConeTwistConstraintDef& def) = 0;
  virtual PhysicConeTwistConstraintWeakRef createAndAdd(const PhysicConeTwistConstraintDef& def) = 0;
  virtual void destroy(PhysicConeTwistConstraintWeakRef ref) = 0;

  virtual void add(PhysicConeTwistConstraintWeakRef ref) = 0;
  virtual void remove(PhysicConeTwistConstraintWeakRef ref) = 0;

  virtual PhysicConeTwistConstraintWeakRef get(uint32_t index) = 0;
  virtual const PhysicConeTwistConstraintWeakRef get(uint32_t index) const = 0;
  virtual std::size_t size() const = 0;
  virtual uint32_t totalLive() const = 0;
  virtual bool isEmpty() const = 0;
};

} // namespace physics
} // namespace gero
