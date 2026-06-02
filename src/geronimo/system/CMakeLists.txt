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
    ./src/geronimo/system/parser-utils/jsonUtils.cpp
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
    ${THIRD_PARTIES_DIR}/dependencies/json/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

if (DEFINED EMSCRIPTEN)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " -O3 -s WASM=1 -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 ")

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/wasm")

else()

    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${GERONIMO_DIR}/lib/native")


    # UNIT TESTS
    # UNIT TESTS
    # UNIT TESTS

    add_executable(unit_test__system
        ./src/geronimo/system/rng/DeterministicRng.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get2UI.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get2F.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get3F.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get1UI.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_forEachArgs.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get4F.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get1F.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_get3UI.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_getFileName.tests.cpp
        ./src/geronimo/system/parser-utils/parser-utils_tests/test_getName.tests.cpp
        ./src/geronimo/system/math/rayIntersectTriangle.tests.cpp
        ./src/geronimo/system/math/rayIntersectSphere.tests.cpp
        ./src/geronimo/system/math/angles.tests.cpp
        ./src/geronimo/system/math/2d/collisions/collisions.tests.cpp
        ./src/geronimo/system/math/2d/convex-polygon/ConvexPolygon.tests.cpp
        ./src/geronimo/system/math/circleCircleIntersectionPoints.tests.cpp
        ./src/geronimo/system/compression/AdaptiveHuffmanAlgorithm.tests.cpp
        ./src/geronimo/system/containers/weak_ref_data_pool_tests/test_features_with_callbacks.tests.cpp
        ./src/geronimo/system/containers/weak_ref_data_pool_tests/test_weak_ref_features.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/swap_two_links_from_same_list.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/reset_link.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/loop_list_links_and_reset.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/remove_link_from_list.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/reset_list.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/list_is_empty_initially.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/add_link_to_list.tests.cpp
        ./src/geronimo/system/containers/basic_double_linked_list_tests/loop_list_links.tests.cpp
        ./src/geronimo/system/containers/dynamic_heap_array/test_remove_data.tests.cpp
        ./src/geronimo/system/containers/dynamic_heap_array/test_std_sort.tests.cpp
        ./src/geronimo/system/containers/dynamic_heap_array/test_push_data.tests.cpp
        ./src/geronimo/system/containers/dynamic_heap_array/test_allocating_features.tests.cpp
        ./src/geronimo/system/containers/dynamic_heap_array/test_get_index.tests.cpp
        ./src/geronimo/system/containers/generic_array_container_tests/test_sortable.tests.cpp
        ./src/geronimo/system/containers/generic_array_container_tests/test_iterator.tests.cpp
        ./src/geronimo/system/containers/generic_array_container_tests/test_data_get_and_set.tests.cpp
        ./src/geronimo/system/containers/generic_array_container_tests/test_constructors.tests.cpp
        ./src/geronimo/system/containers/fixed_size_array_tests/test_remove_data.tests.cpp
        ./src/geronimo/system/containers/fixed_size_array_tests/test_std_sort.tests.cpp
        ./src/geronimo/system/containers/fixed_size_array_tests/test_push_data.tests.cpp
        ./src/geronimo/system/containers/generic_array_container_commons_tests/common.tests.cpp
        ./src/geronimo/system/containers/static_heap_grid_array_tests/can_set_and_get_values.tests.cpp
        ./src/geronimo/system/containers/static_heap_grid_array_tests/can_create.tests.cpp
        ./src/geronimo/system/containers/static_heap_grid_array_tests/iterators_features.tests.cpp
        ./src/geronimo/system/filtering/filtering.tests.cpp
        ./src/geronimo/system/multithreading/multithreading_tests/test_multithreading_avoid_blocking.tests.cpp
        ./src/geronimo/system/multithreading/multithreading_tests/test_multithreading.tests.cpp
        ./src/geronimo/system/trees/bvh/BvhTree.tests.cpp
        ./src/geronimo/system/trees/kd/KdTree-as3d.tests.cpp
        ./src/geronimo/system/trees/kd/KdTree-as2d.tests.cpp
    )
    add_dependencies(unit_test__system ${PROJECT_NAME})
    target_link_libraries(unit_test__system ${PROJECT_NAME} gtest_main gtest pthread)

    target_include_directories(unit_test__system PRIVATE
        ./src
        ../helpers/src
        ${THIRD_PARTIES_DIR}
    )

    set_target_properties(unit_test__system PROPERTIES COMPILE_FLAGS "-O3")
    set_target_properties(unit_test__system PROPERTIES LINK_FLAGS "-O3")
    set_target_properties(unit_test__system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

    # UNIT TESTS
    # UNIT TESTS
    # UNIT TESTS


endif()

