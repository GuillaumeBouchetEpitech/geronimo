
#pragma once

#include "AbstractPhysicBody.hpp"
#include "PhysicBodyDef.hpp"

#include <memory>

namespace gero {
namespace physics {

class AbstractPhysicWorld;

class AbstractPhysicBodyManager {

public:
  virtual ~AbstractPhysicBodyManager() = default;

public:
  virtual void clear() = 0;

  virtual BodyWeakRef createBody(const PhysicBodyDef& def) = 0;
  virtual BodyWeakRef createAndAddBody(const PhysicBodyDef& def) = 0;
  virtual void destroyBody(BodyWeakRef ref) = 0;

  virtual void addBody(BodyWeakRef ref, short group, short mask) = 0;
  virtual void removeBody(AbstractPhysicBody& body) = 0;
  virtual void removeBody(BodyWeakRef ref) = 0;

  virtual BodyWeakRef getBody(uint32_t index) = 0;
  virtual const BodyWeakRef getBody(uint32_t index) const = 0;
  virtual std::size_t size() const = 0;
  virtual bool empty() const = 0;

  virtual void dumpData() = 0;

public:
  static std::unique_ptr<AbstractPhysicBodyManager> create(AbstractPhysicWorld& inPhysicWorld,
                                                           std::size_t pre_allocated_size = 256);
};

} // namespace physics
} // namespace gero
