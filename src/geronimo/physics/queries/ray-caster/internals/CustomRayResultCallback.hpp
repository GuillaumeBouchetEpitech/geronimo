
#pragma once

#include "BaseCustomRayResultCallback.hpp"

namespace gero {
namespace physics {

struct CustomRayResultCallback : public BaseCustomRayResultCallback<btCollisionWorld::RayResultCallback> {

  CustomRayResultCallback(AbstractPhysicWorld& physicWorld,
                          const btVector3& rayFromWorld,
                          const btVector3& rayToWorld,
                          void* toIgnore,
                          RayCaster::RayCastParams::Type type,
                          const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);

  btVector3 _rayFromWorld;
  btVector3 _rayToWorld;

  btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;
};

} // namespace physics
} // namespace gero
