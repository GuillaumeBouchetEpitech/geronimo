
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

  btTransform frameInA = btTransform::getIdentity();
  btTransform frameInB = btTransform::getIdentity();
  frameInA.setFromOpenGLMatrix(glm::value_ptr(def.transform_a));
  frameInB.setFromOpenGLMatrix(glm::value_ptr(def.transform_b));

  btRigidBody& rbA = *(def.body_a->getRawRigidBody());
  btRigidBody& rbB = *(def.body_b->getRawRigidBody());

  RotateOrder rotOrder = RO_XYZ;
  switch (def.order) {
    case PhysicSixDofConstraintDef::RotationOrder::XYZ : rotOrder = RO_XYZ; break;
    case PhysicSixDofConstraintDef::RotationOrder::XZY : rotOrder = RO_XZY; break;
    case PhysicSixDofConstraintDef::RotationOrder::YXZ : rotOrder = RO_YXZ; break;
    case PhysicSixDofConstraintDef::RotationOrder::YZX : rotOrder = RO_YZX; break;
    case PhysicSixDofConstraintDef::RotationOrder::ZXY : rotOrder = RO_ZXY; break;
    case PhysicSixDofConstraintDef::RotationOrder::ZYX : rotOrder = RO_ZYX; break;
  }

  _bullet.constraint = new btGeneric6DofSpring2Constraint(rbA, rbB, frameInA, frameInB, rotOrder);

  _bullet.constraint->setDbgDrawSize(1.0f);
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

//
//
//
//
//

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

//
//
//
//
//

void PhysicSixDofConstraint::setLinearBounce(float x, float y, float z) {
  _bullet.constraint->setBounce(0, x);
  _bullet.constraint->setBounce(1, y);
  _bullet.constraint->setBounce(2, z);
}

void PhysicSixDofConstraint::setAngularBounce(float x, float y, float z) {
  _bullet.constraint->setBounce(3, x);
  _bullet.constraint->setBounce(4, y);
  _bullet.constraint->setBounce(5, z);
}

void PhysicSixDofConstraint::setLinearSpring(bool x, bool y, bool z) {
  _bullet.constraint->enableSpring(0, x);
  _bullet.constraint->enableSpring(1, y);
  _bullet.constraint->enableSpring(2, z);
}

void PhysicSixDofConstraint::setAngularSpring(bool x, bool y, bool z) {
  _bullet.constraint->enableSpring(3, x);
  _bullet.constraint->enableSpring(4, y);
  _bullet.constraint->enableSpring(5, z);
}

// ERP, error reduction parameter, is a rate of how fast the error is reduced
void PhysicSixDofConstraint::setLinearStopERP(float x, float y, float z) {
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, x, 0);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, y, 1);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, z, 2);
}

// ERP, error reduction parameter, is a rate of how fast the error is reduced
void PhysicSixDofConstraint::setAngularStopERP(float x, float y, float z) {
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, x, 3);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, y, 4);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_ERP, z, 5);
}

// CFM, constraint force mixing, adds some small value to the main diagonal of the constraint matrix, to avoid degenerate matrices
void PhysicSixDofConstraint::setLinearStopCFM(float x, float y, float z) {
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, x, 0);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, y, 1);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, z, 2);
}

// CFM, constraint force mixing, adds some small value to the main diagonal of the constraint matrix, to avoid degenerate matrices
void PhysicSixDofConstraint::setAngularStopCFM(float x, float y, float z) {
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, x, 3);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, y, 4);
  _bullet.constraint->setParam(BT_CONSTRAINT_STOP_CFM, z, 5);
}

} // namespace physics
} // namespace gero
