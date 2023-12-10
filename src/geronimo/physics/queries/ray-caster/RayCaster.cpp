
#include "RayCaster.hpp"

#include "internals/CustomConvexResultCallback.hpp"
#include "internals/CustomRayResultCallback.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/AbstractPhysicWorld.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

RayCaster::RayCaster(AbstractPhysicWorld& physicWorld) : _physicWorld(physicWorld) {}

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

void RayCaster::_normalRayCast(RayCastParams& params,
                               const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  btVector3 rayFrom(params.from.x, params.from.y, params.from.z);
  btVector3 rayTo(params.to.x, params.to.y, params.to.z);

  CustomRayResultCallback rayCallback(
    _physicWorld, rayFrom, rayTo, params.toIgnore, params.type, onNewPhysicBodyCallback);
  rayCallback.m_collisionFilterGroup = params.collisionGroup;
  rayCallback.m_collisionFilterMask = params.collisionMask;

  _physicWorld.getRawDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);
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

void RayCaster::_convexSweep(RayCastParams& params, const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  btVector3 rayFrom(params.from.x, params.from.y, params.from.z);
  btVector3 rayTo(params.to.x, params.to.y, params.to.z);

  // replace rayCast with convex sweep test

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

  _physicWorld.getRawDynamicsWorld()->convexSweepTest(&sphereShape, from, to, sweepCallback, allowedCcdPenetration);
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

void RayCaster::_rayCast(RayCastParams& inParams, const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback) {

  if (inParams.radius <= 0.0f) {
    _normalRayCast(inParams, onNewPhysicBodyCallback);
  } else {
    _convexSweep(inParams, onNewPhysicBodyCallback);
  }
}

bool RayCaster::rayCast(RayCastParams& inParams, std::vector<RayCastParams::ResultImpact>& outResultVector) {
  outResultVector.reserve(256);

  const OnNewPhysicBodyCallback callback = [&inParams,
                                            &outResultVector](const RayCastParams::ResultImpact& inResult) -> bool {
    if (inParams.type == RayCastParams::Type::closest) {
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

  _rayCast(inParams, callback);

  return (outResultVector.empty() == false);
}

} // namespace physics
} // namespace gero
