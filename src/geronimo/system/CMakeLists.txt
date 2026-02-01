cmake_minimum_required(VERSION 3.24)

project(geronimo-system)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(THIRD_PARTIES_DIR ${GERONIMO_DIR}/thirdparties)

set(SOURCE_FILES
    ./src/geronimo/system/Timer.cpp
    ./src/geronimo/system/rng/DeterministicRng.cpp
    ./src/geronimo/system/rng/RandomNumberGenerator.cpp
    ./src/geronimo/system/rng/proceduralGeneration/SimplexNoise.cpp
    ./src/geronimo/system/TraceLogger.cpp
    ./src/geronimo/system/parser-utils/BasicRegexParser.cpp
    ./src/geronimo/system/parser-utils/string_view_regexp.cpp
    ./src/geronimo/system/parser-utils/ValuesParsers.cpp
    ./src/geronimo/system/getStreamStr.cpp
    ./src/geronimo/system/math/lerp.cpp
    ./src/geronimo/system/math/BSpline.cpp
    ./src/geronimo/system/math/compute-normal.cpp
    ./src/geronimo/system/math/rotateVec2.cpp
    ./src/geronimo/system/math/2d/collisions/collisions.cpp
    ./src/geronimo/system/math/2d/convex-polygon/ConvexPolygon.cpp
    ./src/geronimo/system/math/angles.cpp
    ./src/geronimo/system/math/circleCircleIntersectionPoints.cpp
    ./src/geronimo/system/math/safe-normalize.cpp
    ./src/geronimo/system/math/validation-helpers.cpp
    ./src/geronimo/system/math/clamp.cpp
    ./src/geronimo/system/math/float-vec-helpers.cpp
    ./src/geronimo/system/math/rayIntersectSphere.cpp
    ./src/geronimo/system/math/rayIntersectTriangle.cpp
    ./src/geronimo/system/compression/AdaptiveHuffmanAlgorithm.cpp
    ./src/geronimo/system/messaging/MessageBuffer.cpp
    ./src/geronimo/system/messaging/MessageView.cpp
    ./src/geronimo/system/messaging/AbstractMessage.cpp
    ./src/geronimo/system/file-utils/FileManager.cpp
    ./src/geronimo/system/file-utils/getFileContent.cpp
    ./src/geronimo/system/file-utils/FileLoadFileCallback.cpp
    ./src/geronimo/system/string-utils/stream-formatter.cpp
    ./src/geronimo/system/string-utils/trim.cpp
    ./src/geronimo/system/easing/easingFunctions.cpp
    ./src/geronimo/system/metrics/HistoricalTimeData.cpp
    ./src/geronimo/system/metrics/Clock.cpp
    ./src/geronimo/system/metrics/PerformanceProfiler.cpp
    ./src/geronimo/system/multithreading/internals/ThreadSynchronizer.cpp
    ./src/geronimo/system/multithreading/internals/Consumer.cpp
    ./src/geronimo/system/multithreading/Producer.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ../helpers/src
    ${THIRD_PARTIES_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

if (DEFINED EMSCRIPTEN)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 -s WASM=1 -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 ")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/wasm")

else()

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/native")

endif()

