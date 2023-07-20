
#pragma once

#include "AbstractPhysicUniversalConstraint.hpp"
#include "PhysicUniversalConstraintDef.hpp"

namespace gero {
namespace physics {

class AbstractPhysicUniversalConstraintManager {

public:
  virtual ~AbstractPhysicUniversalConstraintManager() = default;

public:
  virtual void clear() = 0;

public:
  virtual UniversalConstraintWeakRef create(const PhysicUniversalConstraintDef& def) = 0;
  virtual UniversalConstraintWeakRef createAndAdd(const PhysicUniversalConstraintDef& def) = 0;
  virtual void destroy(UniversalConstraintWeakRef ref) = 0;

  virtual void add(UniversalConstraintWeakRef ref) = 0;
  virtual void remove(UniversalConstraintWeakRef ref) = 0;

  virtual UniversalConstraintWeakRef get(uint32_t index) = 0;
  virtual const UniversalConstraintWeakRef get(uint32_t index) const = 0;
  virtual std::size_t size() const = 0;
  virtual uint32_t totalLive() const = 0;
  virtual bool isEmpty() const = 0;
};

} // namespace physics
} // namespace gero
