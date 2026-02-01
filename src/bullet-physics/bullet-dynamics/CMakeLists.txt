cmake_minimum_required(VERSION 3.24)

project(bullet-dynamics)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(BULLET_DIR ${GERONIMO_DIR}/thirdparties/dependencies/bullet3/)

set(SOURCE_FILES
    # ${BULLET_DIR}/src/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
    # ${BULLET_DIR}/src/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
    # ${BULLET_DIR}/src/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Character/btKinematicCharacterController.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
    ${BULLET_DIR}/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp
    ${BULLET_DIR}/src/BulletDynamics/Vehicle/btWheelInfo.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyGearConstraint.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodySliderConstraint.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBody.cpp
    # ${BULLET_DIR}/src/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.cpp
    ${BULLET_DIR}/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
    ${BULLET_DIR}/src/BulletDynamics/Dynamics/btRigidBody.cpp
    ${BULLET_DIR}/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp
    ${BULLET_DIR}/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
    ${BULLET_DIR}/src/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp
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


