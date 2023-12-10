
#include "CustomConvexResultCallback.hpp"

#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

CustomConvexResultCallback::CustomConvexResultCallback(
  AbstractPhysicWorld& physicWorld,
  float radius,
  void* toIgnore,
  RayCaster::RayCastParams::Type type,
  const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
  : BaseCustomRayResultCallback(physicWorld, toIgnore, type, onNewPhysicBodyCallback), _radius(radius) {}

btScalar CustomConvexResultCallback::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,
                                                     bool normalInWorldSpace) {

  if (_isValid(convexResult.m_hitCollisionObject)) {
    m_closestHitFraction = convexResult.m_hitFraction;
    _process(convexResult.m_hitCollisionObject,
             convexResult.m_hitNormalLocal,
             convexResult.m_hitPointLocal,
             convexResult.m_hitFraction,
             normalInWorldSpace);
  }

  if (_type == RayCaster::RayCastParams::Type::everything) {
    return 1.0f;
  }
  return m_closestHitFraction;
}

} // namespace physics
} // namespace gero
