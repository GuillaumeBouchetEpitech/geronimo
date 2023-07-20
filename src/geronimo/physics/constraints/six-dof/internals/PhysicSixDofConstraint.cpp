
#include "PhysicSixDofConstraint.hpp"

#include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/physics/body/internals/PhysicBody.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

PhysicSixDofConstraint::PhysicSixDofConstraint(const PhysicSixDofConstraintDef& def) {
  if (!def.body_a)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_a)");
  if (!def.body_b)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_b)");

  // const btVector3 anchor = btVector3(def.anchor.x, def.anchor.y, def.anchor.z);
  // const btVector3 axis1 = btVector3(def.axis1.x, def.axis1.y, def.axis1.z);
  // const btVector3 axis2 = btVector3(def.axis2.x, def.axis2.y, def.axis2.z);
  // // const bool useReferenceFrameA = true;

  // btUniversalConstraint(btRigidBody& rbA, btRigidBody& rbB, const btVector3& anchor, const btVector3& axis1, const
  // btVector3& axis2);

  glm::mat4 transformA = glm::identity<glm::mat4>();
  glm::mat4 transformB = glm::identity<glm::mat4>();
  def.body_a->getTransform(transformA);
  def.body_b->getTransform(transformB);

  transformA = glm::inverse(transformA); // in A coordinate system
  transformB = glm::inverse(transformB); // in B coordinate system

  transformA = glm::translate(transformA, def.center); // centered
  transformB = glm::translate(transformB, def.center); // centered

  // constraint_def.transformA = transformA;
  // constraint_def.transformB = transformB;

  btTransform frameInA = btTransform::getIdentity();
  btTransform frameInB = btTransform::getIdentity();
  frameInA.setFromOpenGLMatrix(glm::value_ptr(transformA));
  frameInB.setFromOpenGLMatrix(glm::value_ptr(transformB));

  // btGeneric6DofSpring2Constraint(btRigidBody& rbA, btRigidBody& rbB, const btTransform& frameInA, const btTransform&
  // frameInB, RotateOrder rotOrder = RO_XYZ); btGeneric6DofSpring2Constraint(btRigidBody& rbB, const btTransform&
  // frameInB, RotateOrder rotOrder = RO_XYZ);

  _bullet.constraint = new btGeneric6DofSpring2Constraint(
    *(def.body_a->getRawRigidBody()), *(def.body_b->getRawRigidBody()), frameInA, frameInB);

  _bullet.constraint->setDbgDrawSize(1.0f);

  // void setAngularLowerLimit(const btVector3& angularLower);
  // void getAngularLowerLimit(btVector3& angularLower);
  // void setAngularUpperLimit(const btVector3& angularUpper);
  // void getAngularUpperLimit(btVector3& angularUpper);

  // //first 3 are linear, next 3 are angular

  // void setLimit(int axis, btScalar lo, btScalar hi);
  // void setLimitReversed(int axis, btScalar lo, btScalar hi);

  // bool isLimited(int limitIndex);

  // void setRotationOrder(RotateOrder order) { m_rotateOrder = order; }
  // RotateOrder getRotationOrder() { return m_rotateOrder; }

  // void setAxis( const btVector3& axis1, const btVector3& axis2);

  // void setBounce(int index, btScalar bounce);

  // void enableMotor(int index, bool onOff);
  // void setServo(int index, bool onOff); // set the type of the motor (servo or not) (the motor has to be turned on
  // for servo also) void setTargetVelocity(int index, btScalar velocity); void setServoTarget(int index, btScalar
  // target); void setMaxMotorForce(int index, btScalar force);

  // void enableSpring(int index, bool onOff);
  // void setStiffness(int index, btScalar stiffness, bool limitIfNeeded = true); // if limitIfNeeded is true the system
  // will automatically limit the stiffness in necessary situations where otherwise the spring would move
  // unrealistically too widely void setDamping(int index, btScalar damping, bool limitIfNeeded = true); // if
  // limitIfNeeded is true the system will automatically limit the damping in necessary situations where otherwise the
  // spring would blow up void setEquilibriumPoint(); // set the current constraint position/orientation as an
  // equilibrium point for all DOF void setEquilibriumPoint(int index);  // set the current constraint
  // position/orientation as an equilibrium point for given DOF void setEquilibriumPoint(int index, btScalar val);
}

PhysicSixDofConstraint::~PhysicSixDofConstraint() { delete _bullet.constraint; }

PhysicSixDofConstraint::PhysicSixDofConstraint(PhysicSixDofConstraint&& other) {
  if (&other == this)
    return;

  std::swap(_bullet.constraint, other._bullet.constraint);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);
}

PhysicSixDofConstraint& PhysicSixDofConstraint::operator=(PhysicSixDofConstraint&& other) {
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

void PhysicSixDofConstraint::setLinearLowerLimit(const glm::vec3& linearLower) {
  _bullet.constraint->setLinearLowerLimit(btVector3(linearLower.x, linearLower.y, linearLower.z));
}

void PhysicSixDofConstraint::setLinearUpperLimit(const glm::vec3& linearUpper) {
  _bullet.constraint->setLinearUpperLimit(btVector3(linearUpper.x, linearUpper.y, linearUpper.z));
}

glm::vec3 PhysicSixDofConstraint::getLinearLowerLimit() const {
  btVector3 value;
  _bullet.constraint->getLinearLowerLimit(value);
  return glm::vec3(value[0], value[1], value[2]);
}

glm::vec3 PhysicSixDofConstraint::getLinearUpperLimit() const {
  btVector3 value;
  _bullet.constraint->getLinearUpperLimit(value);
  return glm::vec3(value[0], value[1], value[2]);
}

void PhysicSixDofConstraint::setAngularLowerLimit(const glm::vec3& angularLower) {
  _bullet.constraint->setAngularLowerLimit(btVector3(angularLower.x, angularLower.y, angularLower.z));
}

void PhysicSixDofConstraint::setAngularUpperLimit(const glm::vec3& angularUpper) {
  _bullet.constraint->setAngularUpperLimit(btVector3(angularUpper.x, angularUpper.y, angularUpper.z));
}

glm::vec3 PhysicSixDofConstraint::getAngularLowerLimit() const {
  btVector3 value;
  _bullet.constraint->getAngularLowerLimit(value);
  return glm::vec3(value[0], value[1], value[2]);
}

glm::vec3 PhysicSixDofConstraint::getAngularUpperLimit() const {
  btVector3 value;
  _bullet.constraint->getAngularUpperLimit(value);
  return glm::vec3(value[0], value[1], value[2]);
}

// void setAngularLowerLimit(const btVector3& angularLower);
// void getAngularLowerLimit(btVector3& angularLower);
// void setAngularUpperLimit(const btVector3& angularUpper);
// void getAngularUpperLimit(btVector3& angularUpper);

// glm::vec3 PhysicSixDofConstraint::getAnchor1() const
// {
//   const btVector3& value = _bullet.constraint->getAnchor();
//   return glm::vec3(value[0], value[1], value[2]);
// }
// glm::vec3 PhysicSixDofConstraint::getAnchor2() const
// {
//   const btVector3& value = _bullet.constraint->getAnchor2();
//   return glm::vec3(value[0], value[1], value[2]);
// }
// glm::vec3 PhysicSixDofConstraint::getAxis1() const
// {
//   const btVector3& value = _bullet.constraint->getAxis1();
//   return glm::vec3(value[0], value[1], value[2]);
// }
// glm::vec3 PhysicSixDofConstraint::getAxis2() const
// {
//   const btVector3& value = _bullet.constraint->getAxis2();
//   return glm::vec3(value[0], value[1], value[2]);
// }
// float PhysicSixDofConstraint::getAngle1() const
// {
//   return _bullet.constraint->getAngle1();
// }
// float PhysicSixDofConstraint::getAngle2() const
// {
//   return _bullet.constraint->getAngle2();
// }
// void PhysicSixDofConstraint::setUpperLimit(float angle1Max, float angle2Max)
// {
//   _bullet.constraint->setUpperLimit(angle1Max, angle2Max);
// }
// void PhysicSixDofConstraint::setLowerLimit(float angle1Min, float angle2Min)
// {
//   _bullet.constraint->setLowerLimit(angle1Min, angle2Min);
// }

// 	// const btVector3& getAnchor() { return m_calculatedTransformA.getOrigin(); }
// 	// const btVector3& getAnchor2() { return m_calculatedTransformB.getOrigin(); }
// 	// const btVector3& getAxis1() { return m_axis1; }
// 	// const btVector3& getAxis2() { return m_axis2; }
// 	// btScalar getAngle1() { return getAngle(2); }
// 	// btScalar getAngle2() { return getAngle(1); }
// 	// // limits
// 	// void setUpperLimit(btScalar ang1max, btScalar ang2max) { setAngularUpperLimit(btVector3(0.f, ang1max, ang2max));
// }
// 	// void setLowerLimit(btScalar ang1min, btScalar ang2min) { setAngularLowerLimit(btVector3(0.f, ang1min, ang2min));
// }

} // namespace physics
} // namespace gero
