cmake_minimum_required(VERSION 3.24)
project(lib-bullet-dynamics)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    # ../../dependencies/bullet3/src/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Character/btKinematicCharacterController.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Vehicle/btWheelInfo.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyGearConstraint.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodySliderConstraint.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBody.cpp
    # ../../dependencies/bullet3/src/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Dynamics/btRigidBody.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
    ../../dependencies/bullet3/src/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp
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


