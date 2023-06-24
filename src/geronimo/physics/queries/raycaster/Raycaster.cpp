
#include "Raycaster.hpp"

#include "internals/CustomConvexResultCallback.hpp"
#include "internals/CustomRayResultCallback.hpp"

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

void Raycaster::_normalRaycast(RaycastParams& params,
                               const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  btVector3 rayFrom(params.from.x, params.from.y, params.from.z);
  btVector3 rayTo(params.to.x, params.to.y, params.to.z);

  CustomRayResultCallback rayCallback(
    _physicWorld, rayFrom, rayTo, params.toIgnore, params.type, onNewPhysicBodyCallback);
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

void Raycaster::_convexSweep(RaycastParams& params, const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

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

  CustomConvexResultCallback sweepCallback(
    _physicWorld, params.radius, params.toIgnore, params.type, onNewPhysicBodyCallback);
  sweepCallback.m_collisionFilterGroup = params.collisionGroup;
  sweepCallback.m_collisionFilterMask = params.collisionMask;

  btScalar allowedCcdPenetration = 0.0f;

  _physicWorld._bullet.dynamicsWorld->convexSweepTest(&sphereShape, from, to, sweepCallback, allowedCcdPenetration);
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

void Raycaster::_raycast(RaycastParams& inParams, const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  if (inParams.radius <= 0.0f) {
    _normalRaycast(inParams, onNewPhysicBodyCallback);
  } else {
    _convexSweep(inParams, onNewPhysicBodyCallback);
  }
}

bool Raycaster::raycast(RaycastParams& inParams, std::vector<RaycastParams::ResultImpact>& outResultVector) {
  outResultVector.reserve(256);

  const OnNewPhysicBodyCallback callback = [&inParams,
                                            &outResultVector](const RaycastParams::ResultImpact& inResult) -> bool {
    if (inParams.type == RaycastParams::Type::closest) {
      if (outResultVector.empty()) {
        outResultVector.push_back(inResult);
      } else {
        outResultVector.front() = inResult;
      }
      return true;
    } else {
      // check duplicates
      if (inResult.body != nullptr) {
        for (std::size_t ii = 0; ii < outResultVector.size(); ++ii)
          if (outResultVector[ii].body == inResult.body) {
            if (outResultVector[ii].hitFraction > inResult.hitFraction) {
              // replace if new one is closer
              outResultVector[ii] = inResult;
            }
            return true;
          }
      }

      outResultVector.push_back(inResult);
      return true;
    }
  };

  _raycast(inParams, callback);

  return (outResultVector.empty() == false);
}

} // namespace physics
} // namespace gero
