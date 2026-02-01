cmake_minimum_required(VERSION 3.24)

project(bullet-collision)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(BULLET_DIR ${GERONIMO_DIR}/thirdparties/dependencies/bullet3/)

set(SOURCE_FILES
    # ${BULLET_DIR}/src/Bullet3Collision/BroadPhaseCollision/b3DynamicBvhBroadphase.cpp
    # ${BULLET_DIR}/src/Bullet3Collision/BroadPhaseCollision/b3OverlappingPairCache.cpp
    # ${BULLET_DIR}/src/Bullet3Collision/BroadPhaseCollision/b3DynamicBvh.cpp
    # ${BULLET_DIR}/src/Bullet3Collision/NarrowPhaseCollision/b3CpuNarrowPhase.cpp
    # ${BULLET_DIR}/src/Bullet3Collision/NarrowPhaseCollision/b3ConvexUtility.cpp

    ${BULLET_DIR}/src/BulletCollision/Gimpact/gim_tri_collision.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/gim_memory.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/gim_contact.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btGImpactBvh.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btGImpactShape.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/gim_box_set.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btContactProcessing.cpp
    ${BULLET_DIR}/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btGhostObject.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btUnionFind.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btEmptyShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btCylinderShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btCompoundShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConcaveShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btShapeHull.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btSphereShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConeShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btBox2dShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btBoxShape.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
    ${BULLET_DIR}/src/BulletCollision/CollisionShapes/btCollisionShape.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btDbvt.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
    ${BULLET_DIR}/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
    ${BULLET_DIR}/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp

    ${BULLET_DIR}/Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.cpp

    ${BULLET_DIR}/Extras/ConvexDecomposition/bestfit.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/cd_hull.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/meshvolume.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/raytri.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/ConvexBuilder.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/planetri.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/ConvexDecomposition.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/fitsphere.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/bestfitobb.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/cd_wavefront.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/concavity.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/splitplane.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/float_math.cpp
    ${BULLET_DIR}/Extras/ConvexDecomposition/vlookup.cpp
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BULLET_DIR}/src
    ${BULLET_DIR}/Extras/ConvexDecomposition
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

if (DEFINED EMSCRIPTEN)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 -s WASM=1 -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 ")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/wasm")

else()

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/native")

endif()


