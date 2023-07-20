
#include "PhysicConeTwistConstraint.hpp"

#include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/physics/body/internals/PhysicBody.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace physics {

PhysicConeTwistConstraint::PhysicConeTwistConstraint(const PhysicConeTwistConstraintDef& def) {
  if (!def.body_a)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_a)");
  if (!def.body_b)
    D_THROW(std::runtime_error, "physic hinge constraint need a physic body (body_b)");

  // const btVector3 anchor = btVector3(def.anchor.x, def.anchor.y, def.anchor.z);
  // const btVector3 axis1 = btVector3(def.axis1.x, def.axis1.y, def.axis1.z);
  // const btVector3 axis2 = btVector3(def.axis2.x, def.axis2.y, def.axis2.z);
  // // const bool useReferenceFrameA = true;

    // btUniversalConstraint(btRigidBody& rbA, btRigidBody& rbB, const btVector3& anchor, const btVector3& axis1, const btVector3& axis2);

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

  // btGeneric6DofSpring2Constraint(btRigidBody& rbA, btRigidBody& rbB, const btTransform& frameInA, const btTransform& frameInB, RotateOrder rotOrder = RO_XYZ);
  // btGeneric6DofSpring2Constraint(btRigidBody& rbB, const btTransform& frameInB, RotateOrder rotOrder = RO_XYZ);

  _bullet.constraint = new btConeTwistConstraint(
    *(def.body_a->getRawRigidBody()),
    *(def.body_b->getRawRigidBody()),
    frameInA,
    frameInB
  );

  _bullet.constraint->setDbgDrawSize(1.0f);




}

PhysicConeTwistConstraint::~PhysicConeTwistConstraint() {
  delete _bullet.constraint;
}

PhysicConeTwistConstraint::PhysicConeTwistConstraint(PhysicConeTwistConstraint&& other) {
  if (&other == this)
    return;

  std::swap(_bullet.constraint, other._bullet.constraint);
  std::swap(_body, other._body);
  std::swap(_isAdded, other._isAdded);
}

PhysicConeTwistConstraint& PhysicConeTwistConstraint::operator=(PhysicConeTwistConstraint&& other) {
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


void PhysicConeTwistConstraint::setAngularOnly(bool angularOnly)
{
  _bullet.constraint->setAngularOnly(angularOnly);
}

bool PhysicConeTwistConstraint::getAngularOnly() const
{
  return _bullet.constraint->getAngularOnly();
}

void PhysicConeTwistConstraint::setLimit(float _swingSpan1,float _swingSpan2,float _twistSpan, float _softness /*= 1.f*/, float _biasFactor /*= 0.3f*/, float _relaxationFactor /*= 1.0f*/)
{
  _bullet.constraint->setLimit(_swingSpan1,_swingSpan2,_twistSpan, _softness, _biasFactor, _relaxationFactor);
}

float PhysicConeTwistConstraint::getDamping() const
{
  return _bullet.constraint->getDamping();
}

void PhysicConeTwistConstraint::setDamping(float damping)
{
  _bullet.constraint->setDamping(damping);
}



	// void	setAngularOnly(bool angularOnly);
	// bool    getAngularOnly() const;
	// void	setLimit(btScalar _swingSpan1,btScalar _swingSpan2,btScalar _twistSpan, btScalar _softness = 1.f, btScalar _biasFactor = 0.3f, btScalar _relaxationFactor = 1.0f)

	// btScalar getDamping() const { return m_damping; }
	// void setDamping(btScalar damping) { m_damping = damping; }






} // namespace physics
} // namespace gero
