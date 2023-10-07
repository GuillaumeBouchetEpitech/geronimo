
#include "PhysicVehicle.hpp"

#include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/physics/body/internals/PhysicBody.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace {

class CustomVehicleRayCaster : public btDefaultVehicleRaycaster {
private:
  // must keep it as btDefaultVehicleRaycaster::m_dynamicsWorld is private
  // => that a design flaw from bullet3, kind of sad (;_;)
  btDynamicsWorld& _dynamicsWorld;

  short _group;
  short _mask;

public:
  CustomVehicleRayCaster(btDynamicsWorld& world, short group, short mask)
    : btDefaultVehicleRaycaster(&world), _dynamicsWorld(world), _group(group), _mask(mask) {}

  virtual void* castRay(const btVector3& from, const btVector3& to, btVehicleRaycasterResult& result) override {
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

    // added value => we can specify the group/mask
    rayCallback.m_collisionFilterGroup = _group;
    rayCallback.m_collisionFilterMask = _mask;

    _dynamicsWorld.rayTest(from, to, rayCallback);

    if (!rayCallback.hasHit())
      return nullptr;

    const btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
    if (!body || !body->hasContactResponse())
      return nullptr;

    result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
    result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
    result.m_hitNormalInWorld.normalize();
    result.m_distFraction = rayCallback.m_closestHitFraction;
    return static_cast<void*>(const_cast<btRigidBody*>(body));
  }
};

} // namespace

namespace gero {
namespace physics {

PhysicVehicle::PhysicVehicle(btDiscreteDynamicsWorld& dynamicsWorld, const PhysicVehicleDef& def) {
  if (!def.body)
    D_THROW(std::runtime_error, "physic vehicle need a physic body");

  _body = def.body;

  btRaycastVehicle::btVehicleTuning tuning;
  _bullet.vehicleRayCaster =
    new CustomVehicleRayCaster(dynamicsWorld, def.wheelsCollisionGroup, def.wheelsCollisionMask);

  PhysicBody* implementation = reinterpret_cast<PhysicBody*>(_body.get());

  _bullet.vehicle = new btRaycastVehicle(tuning, implementation->_bullet.body, _bullet.vehicleRayCaster);

  // // never allow the deactivation (sleep) state of the vehicle
  // _bullet.carChassis->setActivationState(DISABLE_DEACTIVATION);

  //
  //
  // WHEELS

  // float connectionHeight = 0.5f;

  // choose coordinate system
  _bullet.vehicle->setCoordinateSystem(def.coordinateSystem.x, def.coordinateSystem.y, def.coordinateSystem.z);

  for (const auto& wheelStats : def.allWheelStats) {
    btWheelInfo& wheelInfo = _bullet.vehicle->addWheel(
      btVector3(wheelStats.connectionPoint.x, wheelStats.connectionPoint.y, wheelStats.connectionPoint.z),
      btVector3(wheelStats.wheelDirectionCS0.x, wheelStats.wheelDirectionCS0.y, wheelStats.wheelDirectionCS0.z),
      btVector3(wheelStats.wheelAxleCS.x, wheelStats.wheelAxleCS.y, wheelStats.wheelAxleCS.z),
      wheelStats.suspensionRestLength,
      wheelStats.wheelRadius,
      tuning,
      wheelStats.isFrontWheel);

    wheelInfo.m_suspensionStiffness = wheelStats.suspensionStiffness;
    wheelInfo.m_maxSuspensionTravelCm = wheelStats.maxSuspensionTravelCm;
    wheelInfo.m_wheelsDampingRelaxation = wheelStats.wheelsDampingRelaxation;
    wheelInfo.m_wheelsDampingCompression = wheelStats.wheelsDampingCompression;
    wheelInfo.m_frictionSlip = wheelStats.wheelFriction;
    wheelInfo.m_rollInfluence = wheelStats.rollInfluence;
  }
}

PhysicVehicle::~PhysicVehicle() {
  delete _bullet.vehicle;
  delete _bullet.vehicleRayCaster;
}

PhysicVehicle::PhysicVehicle(PhysicVehicle&& other) {
  if (&other == this)
    return;

  std::swap(_bullet.vehicleRayCaster, other._bullet.vehicleRayCaster);
  std::swap(_bullet.vehicle, other._bullet.vehicle);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);
}

PhysicVehicle& PhysicVehicle::operator=(PhysicVehicle&& other) {
  if (&other == this)
    return *this;

  std::swap(_bullet.vehicleRayCaster, other._bullet.vehicleRayCaster);
  std::swap(_bullet.vehicle, other._bullet.vehicle);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);

  return *this;
}

//
//
//
//
//

void PhysicVehicle::applyEngineForce(int32_t index, float force) { _bullet.vehicle->applyEngineForce(force, index); }

void PhysicVehicle::applyBrake(int32_t index, float force) { _bullet.vehicle->setBrake(force, index); }

void PhysicVehicle::setSteeringValue(int32_t index, float steering) {
  _bullet.vehicle->setSteeringValue(steering, index);
}

void PhysicVehicle::reset() {
  _body->clearForces();
  _body->setLinearVelocity(0, 0, 0);
  _body->setAngularVelocity(0, 0, 0);
  _body->forceActivate();

  for (int32_t ii = 0; ii < getNumWheels(); ++ii)
    applyBrake(ii, 1000);

  _bullet.vehicle->resetSuspension();
  _bullet.vehicle->updateVehicle(0);

  for (int32_t ii = 0; ii < getNumWheels(); ++ii)
    applyBrake(ii, 0);
}

int32_t PhysicVehicle::getNumWheels() const { return _bullet.vehicle->getNumWheels(); }

const glm::mat4& PhysicVehicle::getWheelTransform(int32_t index, glm::mat4& mat4x4) const {

  // _bullet.vehicle->getWheelTransformWS(index).getOpenGLMatrix(glm::value_ptr(mat4x4));

  btScalar tmpMat[16];
  _bullet.vehicle->getWheelTransformWS(index).getOpenGLMatrix(tmpMat);
  getGlmFromBullet(tmpMat, mat4x4);
  return mat4x4;
}

glm::vec3 PhysicVehicle::getWheelPosition(int32_t index) const {
  auto transform = _bullet.vehicle->getWheelTransformWS(index);
  auto origin = transform.getOrigin();
  return glm::vec3(origin[0], origin[1], origin[2]);
}

glm::quat PhysicVehicle::getWheelOrientation(int32_t index) const {
  auto transform = _bullet.vehicle->getWheelTransformWS(index);
  auto rotation = transform.getRotation();
  return glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
}

float PhysicVehicle::getCurrentSpeedKmHour() const { return float(_bullet.vehicle->getCurrentSpeedKmHour()); }

BodyWeakRef PhysicVehicle::getPhysicBody() { return _body; }
const BodyWeakRef PhysicVehicle::getPhysicBody() const { return _body; }

} // namespace physics
} // namespace gero
