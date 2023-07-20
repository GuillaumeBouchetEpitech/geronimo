
#pragma once

#include "AbstractPhysicHingeConstraint.hpp"
#include "PhysicHingeConstraintDef.hpp"

#include <memory>

namespace gero {
namespace physics {

class PhysicWorld;

class AbstractPhysicHingeConstraintManager {

public:
  virtual ~AbstractPhysicHingeConstraintManager() = default;

public:
  virtual void clear() = 0;

public:
  virtual HingeConstraintWeakRef create(const PhysicHingeConstraintDef& def) = 0;
  virtual HingeConstraintWeakRef createAndAdd(const PhysicHingeConstraintDef& def) = 0;
  virtual void destroy(HingeConstraintWeakRef ref) = 0;

  virtual void add(HingeConstraintWeakRef ref) = 0;
  virtual void remove(HingeConstraintWeakRef ref) = 0;

  virtual HingeConstraintWeakRef get(uint32_t index) = 0;
  virtual const HingeConstraintWeakRef get(uint32_t index) const = 0;
  virtual std::size_t size() const = 0;
  virtual uint32_t totalLive() const = 0;
  virtual bool isEmpty() const = 0;

public:
  static std::unique_ptr<AbstractPhysicHingeConstraintManager> create(PhysicWorld& inPhysicWorld);
};

} // namespace physics
} // namespace gero
