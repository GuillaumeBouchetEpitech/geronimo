
#include "CustomRayResultCallback.hpp"

namespace gero {
namespace physics {

CustomRayResultCallback::CustomRayResultCallback(PhysicWorld& physicWorld,
                                                 const btVector3& rayFromWorld,
                                                 const btVector3& rayToWorld,
                                                 void* toIgnore,
                                                 Raycaster::RaycastParams::Type type,
                                                 const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
  : BaseCustomRayResultCallback(physicWorld, toIgnore, type, onNewPhysicBodyCallback), _rayFromWorld(rayFromWorld),
    _rayToWorld(rayToWorld) {

  // //@BP Mod - allow backface filtering and unflipped normals
  // enum EFlags
  // {
  //   kF_None                 = 0,
  //   kF_FilterBackfaces      = 1 << 0,
  //   kF_KeepUnflippedNormal  = 1 << 1,   // Prevents returned face normal
  //   getting flipped when a ray hits a back-facing triangle
  //   ///SubSimplexConvexCastRaytest is the default, even if kF_None is set.
  //   kF_UseSubSimplexConvexCastRaytest = 1 << 2,   // Uses an approximate
  //   but faster ray versus convex intersection algorithm
  //   kF_UseGjkConvexCastRaytest = 1 << 3,
  //   kF_Terminator        = 0xFFFFFFFF
  // };

  m_flags = btTriangleRaycastCallback::EFlags::kF_None;

  // m_flags |= btTriangleRaycastCallback::EFlags::kF_FilterBackfaces;
  // m_flags |= btTriangleRaycastCallback::EFlags::kF_KeepUnflippedNormal;

  /// SubSimplexConvexCastRaytest is the default, even if kF_None is set.
  // Uses an approximate but faster ray versus convex intersection algorithm
  // m_flags |= btTriangleRaycastCallback::EFlags::kF_UseSubSimplexConvexCastRaytest;

  // m_flags |= btTriangleRaycastCallback::EFlags::kF_UseGjkConvexCastRaytest;
}

btScalar CustomRayResultCallback::addSingleResult(btCollisionWorld::LocalRayResult& rayResult,
                                                  bool normalInWorldSpace) {

  if (rayResult.m_hitFraction > m_closestHitFraction) {
    return 1.0f; // ignore this result
  }

  if (_isValid(rayResult.m_collisionObject)) {

    m_collisionObject = rayResult.m_collisionObject;
    m_closestHitFraction = rayResult.m_hitFraction;

    btVector3 hitPos;
    hitPos.setInterpolate3(_rayFromWorld, _rayToWorld, m_closestHitFraction);

    _process(
      rayResult.m_collisionObject, rayResult.m_hitNormalLocal, hitPos, rayResult.m_hitFraction, normalInWorldSpace);
  }

  if (_type == Raycaster::RaycastParams::Type::everything) {
    return 1.0f;
  }
  return m_closestHitFraction;
}

} // namespace physics
} // namespace gero
