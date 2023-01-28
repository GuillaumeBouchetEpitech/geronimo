
#include "QueryShape.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

namespace {

using OnNewPhysicBodyCallback = std::function<bool(AbstractPhysicBody*)>;

/// ContactResultCallback is used to report contact points
struct MyContactResultCallback
  : public btCollisionWorld::ContactResultCallback {

  int m_bodyUniqueIdA;
  int m_bodyUniqueIdB;
  int m_linkIndexA;
  int m_linkIndexB;
  btScalar m_deltaTime;

  PhysicWorld& _physicWorld;

  btPairCachingGhostObject& _volume;
  void* _toIgnore;

  OnNewPhysicBodyCallback _onNewPhysicBodyCallback;
  bool _isCompleted = false;

  MyContactResultCallback(PhysicWorld& physicWorld,
                          btPairCachingGhostObject& volume,
                          int collisionGroup,
                          int collisionMask,
                          void* toIgnore,
                          const OnNewPhysicBodyCallback& onNewPhysicBodyCallback)
    : _physicWorld(physicWorld)
    , _volume(volume)
    , _toIgnore(toIgnore)
    , _onNewPhysicBodyCallback(onNewPhysicBodyCallback)
  {
    m_collisionFilterGroup = collisionGroup;
    m_collisionFilterMask = collisionMask;
  }

  virtual ~MyContactResultCallback() {}

  virtual bool needsCollision(btBroadphaseProxy* proxy0) const {
    return ((proxy0->m_collisionFilterGroup & m_collisionFilterMask) != 0 &&
            (m_collisionFilterGroup & proxy0->m_collisionFilterMask));
  }

  virtual btScalar addSingleResult(btManifoldPoint& cp,
                                   const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,
                                   const btCollisionObjectWrapper* colObj1Wrap,
                                   int partId1, int index1) {
    static_cast<void>(cp);      // unused
    static_cast<void>(partId0); // unused
    static_cast<void>(index0);  // unused
    static_cast<void>(partId1); // unused
    static_cast<void>(index1);  // unused

    if (_isCompleted)
      return 0;

    const btCollisionObjectWrapper* pObject = (colObj0Wrap->m_collisionObject != &_volume ? colObj0Wrap : colObj1Wrap);

    const btRigidBody* pRigidBody =
      btRigidBody::upcast(pObject->m_collisionObject);
    if (pRigidBody == nullptr || !pRigidBody->hasContactResponse() ||
        pRigidBody->getUserPointer() == nullptr ||
        (_toIgnore != nullptr && pRigidBody->getUserPointer() == _toIgnore)) {
      return 1;
    }

    AbstractPhysicBody* pPhysicBody =
      static_cast<AbstractPhysicBody*>(pRigidBody->getUserPointer());

    if (!_onNewPhysicBodyCallback(pPhysicBody))
      _isCompleted = true;

    return 1;
  }
};

} // namespace

QueryShape::QueryShape(PhysicWorld& physicWorld) : _physicWorld(physicWorld) {}

bool QueryShape::_queryShape(QueryShapeParams& inParams,
                             QueryShapeParams::ResultRaw& outResultArray) {
  outResultArray.allBodiesTotal = 0;

  PhysicShape* pShape = PhysicShape::create(inParams.shape, false);

  btPairCachingGhostObject volume = btPairCachingGhostObject();
  volume.setCollisionShape(pShape->getRawShape());
  volume.setCollisionFlags(volume.getCollisionFlags() |
                           btCollisionObject::CF_NO_CONTACT_RESPONSE);

  btTransform tr;
  tr.setIdentity();
  tr.setOrigin(
    btVector3(inParams.position.x, inParams.position.y, inParams.position.z));
  volume.setWorldTransform(tr);

  const OnNewPhysicBodyCallback callback = [&outResultArray](AbstractPhysicBody* pPhysicBody) -> bool
  {
    if (outResultArray.allBodiesTotal >= outResultArray.allBodiesMaxSize)
      return false;

    // check duplicates
    for (std::size_t ii = 0; ii < outResultArray.allBodiesTotal; ++ii)
      if (outResultArray.allRawBodiesData[ii] == pPhysicBody)
        return true;

    outResultArray.allRawBodiesData[outResultArray.allBodiesTotal] = pPhysicBody;
    outResultArray.allBodiesTotal += 1;
    return true;
  };

  MyContactResultCallback cr(_physicWorld, volume,
                             inParams.collisionGroup, inParams.collisionMask,
                             inParams.toIgnore,
                             callback);

  _physicWorld._bullet.dynamicsWorld->contactTest(&volume, cr);

  delete pShape;

  outResultArray.hasHit = outResultArray.allBodiesTotal > 0;
  return outResultArray.hasHit;
}

bool QueryShape::queryShape(QueryShapeParams& inParams,
                            std::vector<AbstractPhysicBody*>& outResultVector) {

  outResultVector.clear();
  outResultVector.reserve(256);

  PhysicShape* pShape = PhysicShape::create(inParams.shape, false);

  btPairCachingGhostObject volume = btPairCachingGhostObject();
  volume.setCollisionShape(pShape->getRawShape());
  volume.setCollisionFlags(volume.getCollisionFlags() |
                           btCollisionObject::CF_NO_CONTACT_RESPONSE);

  btTransform tr;
  tr.setIdentity();
  tr.setOrigin(
    btVector3(inParams.position.x, inParams.position.y, inParams.position.z));
  volume.setWorldTransform(tr);

  const OnNewPhysicBodyCallback callback = [&outResultVector](AbstractPhysicBody* pPhysicBody) -> bool
  {
    // check duplicates
    for (std::size_t ii = 0; ii < outResultVector.size(); ++ii)
      if (outResultVector[ii] == pPhysicBody)
        return true;

    outResultVector.push_back(pPhysicBody);
    return true;
  };

  MyContactResultCallback cr(_physicWorld, volume,
                             inParams.collisionGroup, inParams.collisionMask,
                             inParams.toIgnore,
                             callback);

  _physicWorld._bullet.dynamicsWorld->contactTest(&volume, cr);

  delete pShape;

  return !outResultVector.empty();
}

} // namespace physics
} // namespace gero
