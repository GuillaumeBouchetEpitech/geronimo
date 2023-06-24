
#pragma once

#include "../Raycaster.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

template <typename ResultCallback> struct BaseCustomRayResultCallback : public ResultCallback {

  PhysicWorld& _physicWorld;
  void* _toIgnore;
  Raycaster::RaycastParams::Type _type;
  Raycaster::OnNewPhysicBodyCallback _onNewPhysicBodyCallback;
  bool _isCompleted = false;

  BaseCustomRayResultCallback(PhysicWorld& physicWorld,
                              void* toIgnore,
                              Raycaster::RaycastParams::Type type,
                              const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
    : _physicWorld(physicWorld), _toIgnore(toIgnore), _type(type), _onNewPhysicBodyCallback(onNewPhysicBodyCallback) {}

  bool _isValid(const btCollisionObject* pCollisionObject) const {
    const btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject);
    return (_isCompleted == false && pRigidBody && pRigidBody->hasContactResponse() &&
            pRigidBody->getUserPointer() != nullptr &&
            (_toIgnore == nullptr || pRigidBody->getUserPointer() != _toIgnore));
  }

  void _process(const btCollisionObject* pCollisionObject,
                const btVector3& hitNormalLocal,
                const btVector3& hitPointWorld,
                float hitFraction,
                bool normalInWorldSpace) {

    const btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject);

    btVector3 hitNormal;
    if (normalInWorldSpace) {
      hitNormal = hitNormalLocal;
    } else {
      /// need to transform normal into worldspace
      hitNormal = pRigidBody->getWorldTransform().getBasis() * hitNormalLocal;
    }

    const glm::vec3 impactPoint = glm::vec3(hitPointWorld[0], hitPointWorld[1], hitPointWorld[2]);
    const glm::vec3 impactNormal = glm::vec3(hitNormal[0], hitNormal[1], hitNormal[2]);

    AbstractPhysicBody* pPhysicBody = static_cast<AbstractPhysicBody*>(pRigidBody->getUserPointer());

    if (!_onNewPhysicBodyCallback({impactPoint, impactNormal, hitFraction, pPhysicBody})) {
      _isCompleted = true;
    }
  }
};

} // namespace physics
} // namespace gero
