cmake_minimum_required(VERSION 3.24)

project(geronimo-audio)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(THIRD_PARTIES_DIR ${GERONIMO_DIR}/thirdparties)

set(SOURCE_FILES
    ./src/geronimo/audio/OpenALSoundManager.cpp
    ./src/geronimo/audio/decoders/ogg.cpp
    ./src/geronimo/audio/OpenAlContext.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE geronimo-system)

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ../helpers/src
    ../system/src
    ${THIRD_PARTIES_DIR}
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

    add_executable(unit_test__audio
        ./src/geronimo/audio/OpenALSoundManager_tests/load_ogg_file.tests.cpp
    )
    add_dependencies(unit_test__audio ${PROJECT_NAME})
    target_link_libraries(unit_test__audio
        ${PROJECT_NAME}
        gtest_main
        gtest
        openal
        pthread
    )

    target_include_directories(unit_test__audio PRIVATE
        ./src
        ../helpers/src
        ../system/src
        ${THIRD_PARTIES_DIR}
    )

    set_target_properties(unit_test__audio PROPERTIES COMPILE_FLAGS "-O3")
    set_target_properties(unit_test__audio PROPERTIES LINK_FLAGS "-O3")
    set_target_properties(unit_test__audio PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

    # UNIT TESTS
    # UNIT TESTS
    # UNIT TESTS


endif()


