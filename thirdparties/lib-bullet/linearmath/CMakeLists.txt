cmake_minimum_required(VERSION 3.24)
project(lib-bullet-linearmath)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    ../../dependencies/bullet3/src/LinearMath/btConvexHull.cpp
    ../../dependencies/bullet3/src/LinearMath/btAlignedAllocator.cpp
    ../../dependencies/bullet3/src/LinearMath/btThreads.cpp
    ../../dependencies/bullet3/src/LinearMath/btSerializer.cpp
    ../../dependencies/bullet3/src/LinearMath/btGeometryUtil.cpp
    ../../dependencies/bullet3/src/LinearMath/btPolarDecomposition.cpp
    ../../dependencies/bullet3/src/LinearMath/btQuickprof.cpp
    ../../dependencies/bullet3/src/LinearMath/btVector3.cpp
    ../../dependencies/bullet3/src/LinearMath/btConvexHullComputer.cpp
    ../../dependencies/bullet3/src/LinearMath/btSerializer64.cpp
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

# target_link_libraries(${PROJECT_NAME} PRIVATE lib-geronimo-system)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../../dependencies/bullet3/src
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


