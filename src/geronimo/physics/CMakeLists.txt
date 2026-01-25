cmake_minimum_required(VERSION 3.24)
project(lib-geronimo-physics)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    ./src/geronimo/physics/body/AbstractPhysicBodyManager.cpp
    ./src/geronimo/physics/body/internals/PhysicBody.cpp
    ./src/geronimo/physics/body/internals/PhysicBodyManager.cpp
    ./src/geronimo/physics/shape/PhysicShape_createCompoundFromGimpactShape.cpp
    ./src/geronimo/physics/shape/PhysicShape.cpp
    ./src/geronimo/physics/vehicle/internals/PhysicVehicleManager.cpp
    ./src/geronimo/physics/vehicle/internals/PhysicVehicle.cpp
    ./src/geronimo/physics/vehicle/AbstractPhysicVehicleManager.cpp
    ./src/geronimo/physics/helpers/transformConversion.cpp
    ./src/geronimo/physics/world/PhysicWorld.cpp
    ./src/geronimo/physics/world/CollisionAlgorithm.cpp
    ./src/geronimo/physics/AbstractPhysicWorld.cpp
    ./src/geronimo/physics/queries/query-shape/QueryShape.cpp
    ./src/geronimo/physics/queries/ray-caster/internals/CustomConvexResultCallback.cpp
    ./src/geronimo/physics/queries/ray-caster/internals/CustomRayResultCallback.cpp
    ./src/geronimo/physics/queries/ray-caster/RayCaster.cpp
    ./src/geronimo/physics/constraints/hinge/AbstractPhysicHingeConstraintManager.cpp
    ./src/geronimo/physics/constraints/hinge/internals/PhysicHingeConstraint.cpp
    ./src/geronimo/physics/constraints/hinge/internals/PhysicHingeConstraintManager.cpp
    ./src/geronimo/physics/constraints/six-dof/internals/PhysicSixDofConstraint.cpp
    ./src/geronimo/physics/constraints/six-dof/internals/PhysicSixDofConstraintManager.cpp
    ./src/geronimo/physics/constraints/six-dof/AbstractPhysicSixDofConstraintManager.cpp
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ../helpers/src
    ../system/src
    ../../../thirdparties
    ../../../thirdparties/dependencies/bullet3/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    lib-bullet-dynamics
    lib-bullet-collision
    lib-bullet-linearmath
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

