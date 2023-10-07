
#pragma once

#include "BaseCustomRayResultCallback.hpp"

namespace gero {
namespace physics {

struct CustomConvexResultCallback : public BaseCustomRayResultCallback<btCollisionWorld::ConvexResultCallback> {
  CustomConvexResultCallback(PhysicWorld& physicWorld,
                             float radius,
                             void* toIgnore,
                             RayCaster::RayCastParams::Type type,
                             const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);

  float _radius;

  btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) override;
};

} // namespace physics
} // namespace gero
