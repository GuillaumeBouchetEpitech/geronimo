
#pragma once

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

#include "PhysicBody.hpp"
#include "PhysicBodyDef.hpp"

namespace gero {
namespace physics {

class PhysicWorld;

class PhysicBodyManager {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;
  BodyContainer _bodies;

private:
  PhysicBodyManager(PhysicWorld& physicWorld);
  ~PhysicBodyManager();

private:
  void clear();

public:
  BodyWeakRef createBody(const PhysicBodyDef& def);
  BodyWeakRef createAndAddBody(const PhysicBodyDef& def);
  void destroyBody(BodyWeakRef ref);

  void addBody(BodyWeakRef ref, short group, short mask);
  void removeBody(AbstractPhysicBody& body);
  void removeBody(BodyWeakRef ref);

  BodyWeakRef getBody(uint32_t index);
  const BodyWeakRef getBody(uint32_t index) const;
  // BodyWeakRef getBody(const AbstractPhysicBody& body);
  // const BodyWeakRef getBody(const AbstractPhysicBody& body) const;
  std::size_t size() const;
  bool empty() const;

  void dumpData();
};

} // namespace physics
} // namespace gero
