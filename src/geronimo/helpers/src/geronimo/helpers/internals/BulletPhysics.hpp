
#pragma once

// shutdown the warnings
#pragma GCC system_header
#pragma clang system_header
// shutdown the warnings

#include "dependencies/bullet3/src/btBulletDynamicsCommon.h"

// #include
// "dependencies/bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h"

// to expose "btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE"
#include "dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "dependencies/bullet3/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.h"

#include "dependencies/bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "dependencies/bullet3/src/BulletCollision/CollisionDispatch/btGhostObject.h"

#include "dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

#include "dependencies/bullet3/src/BulletCollision/Gimpact/btGImpactShape.h"
