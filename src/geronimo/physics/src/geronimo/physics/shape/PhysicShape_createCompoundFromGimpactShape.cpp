
#include "PhysicShape.hpp"

// #include "geronimo/physics/helpers/transformConversion.hpp"

#include "geronimo/helpers/internals/BulletPhysics.hpp"
// // #include "geronimo/system/ErrorHandler.hpp"
// // #include "geronimo/system/TraceLogger.hpp"

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wall"
// #pragma GCC diagnostic ignored "-Wextra"

// #undef BT_USE_DOUBLE_PRECISION

// it compile when included here
#include <BulletCollision/Gimpact/btCompoundFromGimpact.h>
// /it compile when included here

// #include "../../../../thirdparties/dependencies/bullet3/Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.h"
#include "../Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.h"

// #pragma GCC diagnostic pop

namespace gero {
namespace physics {

btCompoundShape* StaticMeshShape::createCompoundFromGimpactShape(const btGImpactMeshShape* gimpactMesh) {
  constexpr btScalar depth = 0;
  return btCreateCompoundFromGimpactShape(gimpactMesh, depth);
}

btCollisionShape*
StaticMeshShape::createConvexDecompositionFromGimpactShape(btTriangleIndexVertexArray* indexVertexArrays) {
  btStridingMeshInterface* meshInterface = indexVertexArrays;
  return new btGImpactConvexDecompositionShape(
    meshInterface, btVector3(1.0f, 1.0f, 1.0f), 0.00999999977648258209F, false);
}

} // namespace physics
} // namespace gero
