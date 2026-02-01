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

endif()


