
#pragma once

#include "BaseCustomRayResultCallback.hpp"

namespace gero {
namespace physics {

struct CustomConvexResultCallback : public BaseCustomRayResultCallback<btCollisionWorld::ConvexResultCallback> {
  CustomConvexResultCallback(PhysicWorld& physicWorld,
                             float radius,
                             void* toIgnore,
                             Raycaster::RaycastParams::Type type,
                             const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);

  float _radius;

  btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) override;
};

} // namespace physics
} // namespace gero
