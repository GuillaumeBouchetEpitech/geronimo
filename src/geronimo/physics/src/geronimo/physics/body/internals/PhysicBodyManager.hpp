
#pragma once

#include "../AbstractPhysicBodyManager.hpp"
#include "PhysicBody.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

namespace gero {
namespace physics {

class AbstractPhysicWorld;

class PhysicBodyManager : public AbstractPhysicBodyManager {
  friend AbstractPhysicWorld;

private:
  AbstractPhysicWorld& _physicWorld;
  BodyContainer _bodies;

public:
  PhysicBodyManager(AbstractPhysicWorld& physicWorld, std::size_t pre_allocated_size = 256);
  ~PhysicBodyManager();

public:
  void clear() override;

public:
  BodyWeakRef createBody(const PhysicBodyDef& def) override;
  BodyWeakRef createAndAddBody(const PhysicBodyDef& def) override;
  void destroyBody(BodyWeakRef ref) override;

  void addBody(BodyWeakRef ref, short group, short mask) override;
  void removeBody(AbstractPhysicBody& body) override;
  void removeBody(BodyWeakRef ref) override;

  BodyWeakRef getBody(uint32_t index) override;
  const BodyWeakRef getBody(uint32_t index) const override;
  std::size_t size() const override;
  bool empty() const override;

  void dumpData() override;
};

} // namespace physics
} // namespace gero
