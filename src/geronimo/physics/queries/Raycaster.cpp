
#include "Raycaster.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

Raycaster::Raycaster(PhysicWorld& physicWorld) : _physicWorld(physicWorld) {}

//
//
//
//
//
//
//
//
//
//

template <typename ResultCallback>
struct BaseCustomRayResultCallback : public ResultCallback {

  PhysicWorld& _physicWorld;
  void* _toIgnore;
  Raycaster::RaycastParams::Type _type;
  Raycaster::OnNewPhysicBodyCallback _onNewPhysicBodyCallback;
  bool _isCompleted = false;

  BaseCustomRayResultCallback(PhysicWorld& physicWorld, void* toIgnore,
                              Raycaster::RaycastParams::Type type,
                              const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
    : _physicWorld(physicWorld)
    , _toIgnore(toIgnore)
    , _type(type)
    , _onNewPhysicBodyCallback(onNewPhysicBodyCallback)
  {}

  bool _isValid(const btCollisionObject* pCollisionObject) const {
    const btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject);
    return (
      _isCompleted == false &&
      pRigidBody && pRigidBody->hasContactResponse() &&
      pRigidBody->getUserPointer() != nullptr &&
      (_toIgnore == nullptr || pRigidBody->getUserPointer() != _toIgnore));
  }

  void _process(const btCollisionObject* pCollisionObject,
                const btVector3& hitNormalLocal, const btVector3& hitPointWorld,
                bool normalInWorldSpace) {

    const btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject);

    btVector3 hitNormal;
    if (normalInWorldSpace) {
      hitNormal = hitNormalLocal;
    } else {
      /// need to transform normal into worldspace
      hitNormal = pRigidBody->getWorldTransform().getBasis() * hitNormalLocal;
    }

    const glm::vec3 impactPoint =
      glm::vec3(hitPointWorld[0], hitPointWorld[1], hitPointWorld[2]);
    const glm::vec3 impactNormal =
      glm::vec3(hitNormal[0], hitNormal[1], hitNormal[2]);

    AbstractPhysicBody* pPhysicBody =
      static_cast<AbstractPhysicBody*>(pRigidBody->getUserPointer());

    if (!_onNewPhysicBodyCallback({ impactPoint, impactNormal, pPhysicBody }))
      _isCompleted = true;
  }
};

//
//
//
//
//
//
//
//
//
//

struct CustomRayResultCallback
  : public BaseCustomRayResultCallback<btCollisionWorld::RayResultCallback> {

  CustomRayResultCallback(PhysicWorld& physicWorld,
                          const btVector3& rayFromWorld,
                          const btVector3& rayToWorld, void* toIgnore,
                          Raycaster::RaycastParams::Type type,
                          const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
    : BaseCustomRayResultCallback(physicWorld, toIgnore, type, onNewPhysicBodyCallback),
      _rayFromWorld(rayFromWorld), _rayToWorld(rayToWorld) {

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

    // this->m_flags |= btTriangleRaycastCallback::EFlags::kF_FilterBackfaces;
    // this->m_flags |=
    // btTriangleRaycastCallback::EFlags::kF_KeepUnflippedNormal; this->m_flags
    // |= btTriangleRaycastCallback::EFlags::kF_UseSubSimplexConvexCastRaytest;
    // this->m_flags |=
    // btTriangleRaycastCallback::EFlags::kF_UseGjkConvexCastRaytest;
  }

  btVector3 _rayFromWorld;
  btVector3 _rayToWorld;

  virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,
                                   bool normalInWorldSpace) override {

    m_collisionObject = rayResult.m_collisionObject;
    m_closestHitFraction = rayResult.m_hitFraction;

    if (_isValid(rayResult.m_collisionObject)) {

      btVector3 hitPos;
      hitPos.setInterpolate3(_rayFromWorld, _rayToWorld, m_closestHitFraction);

      _process(rayResult.m_collisionObject, rayResult.m_hitNormalLocal, hitPos,
               normalInWorldSpace);
    }

    if (_type == Raycaster::RaycastParams::Type::everything)
      return 1.0f;
    return m_closestHitFraction;
  }
};

void Raycaster::_normalRaycast(RaycastParams& params,
                               const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  btVector3 rayFrom(params.from.x, params.from.y, params.from.z);
  btVector3 rayTo(params.to.x, params.to.y, params.to.z);

  CustomRayResultCallback rayCallback(_physicWorld, rayFrom, rayTo, params.toIgnore, params.type, onNewPhysicBodyCallback);
  rayCallback.m_collisionFilterGroup = params.collisionGroup;
  rayCallback.m_collisionFilterMask = params.collisionMask;

  _physicWorld._bullet.dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
}

//
//
//
//
//
//
//
//
//
//

struct CustomConvexResultCallback
  : public BaseCustomRayResultCallback<btCollisionWorld::ConvexResultCallback> {
  CustomConvexResultCallback(PhysicWorld& physicWorld, void* toIgnore,
                             Raycaster::RaycastParams::Type type,
                              const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
    : BaseCustomRayResultCallback(physicWorld, toIgnore, type, onNewPhysicBodyCallback) {}

  virtual btScalar
  addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {

    // // caller already does the filter on the m_closestHitFraction
    // btAssert(convexResult.m_hitFraction <= m_closestHitFraction);

    m_closestHitFraction = convexResult.m_hitFraction;

    if (_isValid(convexResult.m_hitCollisionObject)) {
      _process(convexResult.m_hitCollisionObject, convexResult.m_hitNormalLocal,
               convexResult.m_hitPointLocal, normalInWorldSpace);
    }

    if (_type == Raycaster::RaycastParams::Type::everything)
      return 1.0f;
    return m_closestHitFraction;
  }
};

void Raycaster::_convexSweep(RaycastParams& params,
                             const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  btVector3 rayFrom(params.from.x, params.from.y, params.from.z);
  btVector3 rayTo(params.to.x, params.to.y, params.to.z);

  // replace raycast with convex sweep test

  btSphereShape sphereShape(params.radius);

  btTransform from;
  from.setIdentity();
  from.setOrigin(rayFrom);

  btTransform to;
  to.setIdentity();
  to.setOrigin(rayTo);

  CustomConvexResultCallback sweepCallback(_physicWorld, params.toIgnore,
                                           params.type, onNewPhysicBodyCallback);
  sweepCallback.m_collisionFilterGroup = params.collisionGroup;
  sweepCallback.m_collisionFilterMask = params.collisionMask;

  btScalar allowedCcdPenetration = 0.0f;

  _physicWorld._bullet.dynamicsWorld->convexSweepTest(
    &sphereShape, from, to, sweepCallback, allowedCcdPenetration);
}

//
//
//
//
//
//
//
//
//
//

void Raycaster::_raycast(RaycastParams& inParams,
                         const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  if (inParams.radius <= 0.0f) {
    _normalRaycast(inParams, onNewPhysicBodyCallback);
  } else {
    _convexSweep(inParams, onNewPhysicBodyCallback);
  }
}

bool Raycaster::raycast(RaycastParams& inParams, std::vector<RaycastParams::ResultImpact>& outResultVector)
{
  outResultVector.reserve(256);

  const OnNewPhysicBodyCallback callback = [&inParams, &outResultVector](const RaycastParams::ResultImpact& inResult) -> bool
  {
    if (inParams.type == RaycastParams::Type::closest)
    {
      if (outResultVector.empty())
      {
        outResultVector.push_back(inResult);
      }
      else
      {
        outResultVector.front() = inResult;
      }
    }
    else
    {
      // check duplicates
      for (std::size_t ii = 0; ii < outResultVector.size(); ++ii)
        if (outResultVector[ii].body == inResult.body)
          return true;

      outResultVector.push_back(inResult);
    }

    return true;
  };

  _raycast(inParams, callback);

  return (outResultVector.empty() == false);
}

} // namespace physics
} // namespace gero
