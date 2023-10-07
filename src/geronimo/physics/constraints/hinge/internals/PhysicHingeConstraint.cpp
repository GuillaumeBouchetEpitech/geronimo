
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

  btRigidBody& rbA = *(def.body_a->getRawRigidBody());
  btRigidBody& rbB = *(def.body_b->getRawRigidBody());
  const btVector3 pivot_in_a = btVector3(def.coord_a.x, def.coord_a.y, def.coord_a.z);
  const btVector3 pivot_in_b = btVector3(def.coord_b.x, def.coord_b.y, def.coord_b.z);
  const btVector3 axis = btVector3(def.axis.x, def.axis.y, def.axis.z);
  const bool useReferenceFrameA = true;

  _bullet.constraint = new btHingeConstraint(rbA,
                                             rbB,
                                             pivot_in_a,
                                             pivot_in_b,
                                             // axisInA, axisInB,
                                             axis,
                                             axis,
                                             useReferenceFrameA);

  // _bullet.constraint = new btHinge2Constraint(rbA,
  //                                            rbB,
  //                                            pivot_in_a,
  //                                           //  pivot_in_b,
  //                                            axis,
  //                                            axis);

  // btHinge2Constraint(btRigidBody& rbA, btRigidBody& rbB, btVector3& anchor, btVector3& axis1, btVector3& axis2);


  _bullet.constraint->setDbgDrawSize(1.0f);

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

void PhysicHingeConstraint::setLimit(float low, float high)
{
  _bullet.constraint->setLimit(low, high);
}

} // namespace physics
} // namespace gero
