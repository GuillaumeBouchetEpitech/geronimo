cmake_minimum_required(VERSION 3.24)
project(lib-bullet-collision)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    # ../../dependencies/bullet3/src/Bullet3Collision/BroadPhaseCollision/b3DynamicBvhBroadphase.cpp
    # ../../dependencies/bullet3/src/Bullet3Collision/BroadPhaseCollision/b3OverlappingPairCache.cpp
    # ../../dependencies/bullet3/src/Bullet3Collision/BroadPhaseCollision/b3DynamicBvh.cpp
    # ../../dependencies/bullet3/src/Bullet3Collision/NarrowPhaseCollision/b3CpuNarrowPhase.cpp
    # ../../dependencies/bullet3/src/Bullet3Collision/NarrowPhaseCollision/b3ConvexUtility.cpp

    ../../dependencies/bullet3/src/BulletCollision/Gimpact/gim_tri_collision.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/gim_memory.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/gim_contact.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btGImpactBvh.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btGImpactShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/gim_box_set.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btContactProcessing.cpp
    ../../dependencies/bullet3/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btGhostObject.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btUnionFind.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btEmptyShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btCylinderShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btCompoundShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConcaveShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btShapeHull.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btSphereShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConeShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btBox2dShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btBoxShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
    ../../dependencies/bullet3/src/BulletCollision/CollisionShapes/btCollisionShape.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btDbvt.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
    ../../dependencies/bullet3/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
    ../../dependencies/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp

    ../../dependencies/bullet3/Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.cpp

    ../../dependencies/bullet3/Extras/ConvexDecomposition/bestfit.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/cd_hull.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/meshvolume.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/raytri.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/ConvexBuilder.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/planetri.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/ConvexDecomposition.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/fitsphere.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/bestfitobb.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/cd_wavefront.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/concavity.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/splitplane.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/float_math.cpp
    ../../dependencies/bullet3/Extras/ConvexDecomposition/vlookup.cpp
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

# target_link_libraries(${PROJECT_NAME} PRIVATE lib-geronimo-system)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../../dependencies/bullet3/src
    ../../dependencies/bullet3/Extras/ConvexDecomposition
)

if (DEFINED EMSCRIPTEN)

    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 -s WASM=1 -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 ")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/wasm")

else()

    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/native")

endif()


