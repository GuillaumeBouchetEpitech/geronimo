cmake_minimum_required(VERSION 3.24)

project(bullet-linearmath)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(BULLET_DIR ${GERONIMO_DIR}/thirdparties/dependencies/bullet3/)

set(SOURCE_FILES
    ${BULLET_DIR}/src/LinearMath/btConvexHull.cpp
    ${BULLET_DIR}/src/LinearMath/btAlignedAllocator.cpp
    ${BULLET_DIR}/src/LinearMath/btThreads.cpp
    ${BULLET_DIR}/src/LinearMath/btSerializer.cpp
    ${BULLET_DIR}/src/LinearMath/btGeometryUtil.cpp
    ${BULLET_DIR}/src/LinearMath/btPolarDecomposition.cpp
    ${BULLET_DIR}/src/LinearMath/btQuickprof.cpp
    ${BULLET_DIR}/src/LinearMath/btVector3.cpp
    ${BULLET_DIR}/src/LinearMath/btConvexHullComputer.cpp
    ${BULLET_DIR}/src/LinearMath/btSerializer64.cpp
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BULLET_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

if (DEFINED EMSCRIPTEN)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 -s WASM=1 -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 ")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/wasm")

else()

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/native")

endif()


