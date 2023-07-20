
#include "PhysicUniversalConstraint.hpp"

#include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/physics/body/internals/PhysicBody.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

PhysicUniversalConstraint::PhysicUniversalConstraint(const PhysicUniversalConstraintDef& def) {
  if (!def.body_a)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_a)");
  if (!def.body_b)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_b)");

  const btVector3 anchor = btVector3(def.anchor.x, def.anchor.y, def.anchor.z);
  const btVector3 axis1 = btVector3(def.axis1.x, def.axis1.y, def.axis1.z);
  const btVector3 axis2 = btVector3(def.axis2.x, def.axis2.y, def.axis2.z);
  // const bool useReferenceFrameA = true;

  // btUniversalConstraint(btRigidBody& rbA, btRigidBody& rbB, const btVector3& anchor, const btVector3& axis1, const
  // btVector3& axis2);

  _bullet.constraint =
    new btUniversalConstraint(*(def.body_a->getRawRigidBody()), *(def.body_b->getRawRigidBody()), anchor, axis1, axis2);

  _bullet.constraint->setDbgDrawSize(1.0f);

  // const btVector3& getAnchor() { return m_calculatedTransformA.getOrigin(); }
  // const btVector3& getAnchor2() { return m_calculatedTransformB.getOrigin(); }
  // const btVector3& getAxis1() { return m_axis1; }
  // const btVector3& getAxis2() { return m_axis2; }
  // btScalar getAngle1() { return getAngle(2); }
  // btScalar getAngle2() { return getAngle(1); }
  // // limits
  // void setUpperLimit(btScalar ang1max, btScalar ang2max) { setAngularUpperLimit(btVector3(0.f, ang1max, ang2max)); }
  // void setLowerLimit(btScalar ang1min, btScalar ang2min) { setAngularLowerLimit(btVector3(0.f, ang1min, ang2min)); }
}

PhysicUniversalConstraint::~PhysicUniversalConstraint() { delete _bullet.constraint; }

PhysicUniversalConstraint::PhysicUniversalConstraint(PhysicUniversalConstraint&& other) {
  if (&other == this)
    return;

  std::swap(_bullet.constraint, other._bullet.constraint);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);
}

PhysicUniversalConstraint& PhysicUniversalConstraint::operator=(PhysicUniversalConstraint&& other) {
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

glm::vec3 PhysicUniversalConstraint::getAnchor1() const {
  const btVector3& value = _bullet.constraint->getAnchor();
  return glm::vec3(value[0], value[1], value[2]);
}
glm::vec3 PhysicUniversalConstraint::getAnchor2() const {
  const btVector3& value = _bullet.constraint->getAnchor2();
  return glm::vec3(value[0], value[1], value[2]);
}
glm::vec3 PhysicUniversalConstraint::getAxis1() const {
  const btVector3& value = _bullet.constraint->getAxis1();
  return glm::vec3(value[0], value[1], value[2]);
}
glm::vec3 PhysicUniversalConstraint::getAxis2() const {
  const btVector3& value = _bullet.constraint->getAxis2();
  return glm::vec3(value[0], value[1], value[2]);
}
float PhysicUniversalConstraint::getAngle1() const { return _bullet.constraint->getAngle1(); }
float PhysicUniversalConstraint::getAngle2() const { return _bullet.constraint->getAngle2(); }
void PhysicUniversalConstraint::setUpperLimit(float angle1Max, float angle2Max) {
  _bullet.constraint->setUpperLimit(angle1Max, angle2Max);
}
void PhysicUniversalConstraint::setLowerLimit(float angle1Min, float angle2Min) {
  _bullet.constraint->setLowerLimit(angle1Min, angle2Min);
}

// const btVector3& getAnchor() { return m_calculatedTransformA.getOrigin(); }
// const btVector3& getAnchor2() { return m_calculatedTransformB.getOrigin(); }
// const btVector3& getAxis1() { return m_axis1; }
// const btVector3& getAxis2() { return m_axis2; }
// btScalar getAngle1() { return getAngle(2); }
// btScalar getAngle2() { return getAngle(1); }
// // limits
// void setUpperLimit(btScalar ang1max, btScalar ang2max) { setAngularUpperLimit(btVector3(0.f, ang1max, ang2max)); }
// void setLowerLimit(btScalar ang1min, btScalar ang2min) { setAngularLowerLimit(btVector3(0.f, ang1min, ang2min)); }

} // namespace physics
} // namespace gero
