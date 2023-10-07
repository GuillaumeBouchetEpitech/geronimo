
#pragma once

#include "AbstractPhysicSixDofConstraint.hpp"
#include "PhysicSixDofConstraintDef.hpp"

#include <memory>

namespace gero {
namespace physics {

class PhysicWorld;

class AbstractPhysicSixDofConstraintManager {

public:
  virtual ~AbstractPhysicSixDofConstraintManager() = default;

public:
  virtual void clear() = 0;

public:
  virtual SixDofConstraintWeakRef create(const PhysicSixDofConstraintDef& def) = 0;
  virtual SixDofConstraintWeakRef createAndAdd(const PhysicSixDofConstraintDef& def) = 0;
  virtual void destroy(SixDofConstraintWeakRef ref) = 0;

  virtual void add(SixDofConstraintWeakRef ref) = 0;
  virtual void remove(SixDofConstraintWeakRef ref) = 0;

  virtual SixDofConstraintWeakRef get(uint32_t index) = 0;
  virtual const SixDofConstraintWeakRef get(uint32_t index) const = 0;
  virtual std::size_t size() const = 0;
  virtual uint32_t totalLive() const = 0;
  virtual bool isEmpty() const = 0;

public:
  static std::unique_ptr<AbstractPhysicSixDofConstraintManager> create(PhysicWorld& inPhysicWorld);

};

} // namespace physics
} // namespace gero
