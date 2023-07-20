
#include "PhysicHingeConstraint.hpp"

#include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/physics/body/internals/PhysicBody.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

PhysicHingeConstraint::PhysicHingeConstraint(const PhysicHingeConstraintDef& def) {
  if (!def.body_a)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_a)");
  if (!def.body_b)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_b)");

  const btVector3 pivot_in_a = btVector3(def.coord_a.x, def.coord_a.y, def.coord_a.z);
  const btVector3 pivot_in_b = btVector3(def.coord_b.x, def.coord_b.y, def.coord_b.z);
  const btVector3 axis = btVector3(def.axis.x, def.axis.y, def.axis.z);
  const bool useReferenceFrameA = true;

  _bullet.constraint = new btHingeConstraint(*(def.body_a->getRawRigidBody()),
                                             *(def.body_b->getRawRigidBody()),
                                             pivot_in_a,
                                             pivot_in_b,
                                             // axisInA, axisInB,
                                             axis,
                                             axis,
                                             useReferenceFrameA);

  _bullet.constraint->setDbgDrawSize(1.0f);

  // _body = def.body;

  // btRaycastVehicle::btVehicleTuning tuning;
  // _bullet.vehicleRayCaster =
  //   new CustomVehicleRaycaster(dynamicsWorld, def.wheelsCollisionGroup, def.wheelsCollisionMask);

  // PhysicBody* implementation = reinterpret_cast<PhysicBody*>(_body.get());

  // _bullet.vehicle = new btRaycastVehicle(tuning, implementation->_bullet.body, _bullet.vehicleRayCaster);

  // // // never allow the deactivation (sleep) state of the vehicle
  // // _bullet.carChassis->setActivationState(DISABLE_DEACTIVATION);

  // //
  // //
  // // WHEELS

  // // float connectionHeight = 0.5f;

  // // choose coordinate system
  // _bullet.vehicle->setCoordinateSystem(def.coordinateSystem.x, def.coordinateSystem.y, def.coordinateSystem.z);

  // for (const auto& wheelStats : def.allWheelStats) {
  //   btWheelInfo& wheelInfo = _bullet.vehicle->addWheel(
  //     btVector3(wheelStats.connectionPoint.x, wheelStats.connectionPoint.y, wheelStats.connectionPoint.z),
  //     btVector3(wheelStats.wheelDirectionCS0.x, wheelStats.wheelDirectionCS0.y, wheelStats.wheelDirectionCS0.z),
  //     btVector3(wheelStats.wheelAxleCS.x, wheelStats.wheelAxleCS.y, wheelStats.wheelAxleCS.z),
  //     wheelStats.suspensionRestLength,
  //     wheelStats.wheelRadius,
  //     tuning,
  //     wheelStats.isFrontWheel);

  //   wheelInfo.m_suspensionStiffness = wheelStats.suspensionStiffness;
  //   wheelInfo.m_maxSuspensionTravelCm = wheelStats.maxSuspensionTravelCm;
  //   wheelInfo.m_wheelsDampingRelaxation = wheelStats.wheelsDampingRelaxation;
  //   wheelInfo.m_wheelsDampingCompression = wheelStats.wheelsDampingCompression;
  //   wheelInfo.m_frictionSlip = wheelStats.wheelFriction;
  //   wheelInfo.m_rollInfluence = wheelStats.rollInfluence;
  // }
}

PhysicHingeConstraint::~PhysicHingeConstraint() { delete _bullet.constraint; }

PhysicHingeConstraint::PhysicHingeConstraint(PhysicHingeConstraint&& other) {
  if (&other == this)
    return;

  std::swap(_bullet.constraint, other._bullet.constraint);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);
}

PhysicHingeConstraint& PhysicHingeConstraint::operator=(PhysicHingeConstraint&& other) {
  if (&other == this)
    return *this;

  std::swap(_bullet.constraint, other._bullet.constraint);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);

  return *this;
}

//
//
//
//
//

// void PhysicHingeConstraint::applyEngineForce(int32_t index, float force) { _bullet.vehicle->applyEngineForce(force,
// index); }

// void PhysicHingeConstraint::applyBrake(int32_t index, float force) { _bullet.vehicle->setBrake(force, index); }

// void PhysicHingeConstraint::setSteeringValue(int32_t index, float steering) {
//   _bullet.vehicle->setSteeringValue(steering, index);
// }

// void PhysicHingeConstraint::reset() {
//   _body->clearForces();
//   _body->setLinearVelocity(0, 0, 0);
//   _body->setAngularVelocity(0, 0, 0);
//   _body->forceActivate();

//   for (int32_t ii = 0; ii < getNumWheels(); ++ii)
//     applyBrake(ii, 1000);

//   _bullet.vehicle->resetSuspension();
//   _bullet.vehicle->updateVehicle(0);

//   for (int32_t ii = 0; ii < getNumWheels(); ++ii)
//     applyBrake(ii, 0);
// }

// int32_t PhysicHingeConstraint::getNumWheels() const { return _bullet.vehicle->getNumWheels(); }

// const glm::mat4& PhysicHingeConstraint::getWheelTransform(int32_t index, glm::mat4& mat4x4) const {

//   // _bullet.vehicle->getWheelTransformWS(index).getOpenGLMatrix(glm::value_ptr(mat4x4));

//   btScalar tmpMat[16];
//   _bullet.vehicle->getWheelTransformWS(index).getOpenGLMatrix(tmpMat);
//   getGlmFromBullet(tmpMat, mat4x4);
//   return mat4x4;
// }

// glm::vec3 PhysicHingeConstraint::getWheelPosition(int32_t index) const {
//   auto transform = _bullet.vehicle->getWheelTransformWS(index);
//   auto origin = transform.getOrigin();
//   return glm::vec3(origin[0], origin[1], origin[2]);
// }

// glm::quat PhysicHingeConstraint::getWheelOrientation(int32_t index) const {
//   auto transform = _bullet.vehicle->getWheelTransformWS(index);
//   auto rotation = transform.getRotation();
//   return glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
// }

// float PhysicHingeConstraint::getCurrentSpeedKmHour() const { return float(_bullet.vehicle->getCurrentSpeedKmHour());
// }

// BodyWeakRef PhysicHingeConstraint::getPhysicBody() { return _body; }
// const BodyWeakRef PhysicHingeConstraint::getPhysicBody() const { return _body; }

} // namespace physics
} // namespace gero
