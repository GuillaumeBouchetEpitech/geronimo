
cmake_minimum_required(VERSION 3.24)

set(CHECK_CLANG_TIDY FALSE CACHE BOOL "Some user-specified option")

project(geronimo-wrappers)

#
# ensure dependencies
#

execute_process(
    COMMAND sh ./scripts/sh_ensure_dependencies.sh
    WORKING_DIRECTORY .
)

#
# ensure wasm compiler (if needed)
#

if (DEFINED EMSCRIPTEN)

    execute_process(
        COMMAND sh ./scripts/sh_ensure_wasm_compiler.sh
        WORKING_DIRECTORY .
    )

endif()

#
# ensure bullet physics
#

add_subdirectory(./src/bullet-physics)

#
# check for clang-tidy linter
# -> this is setup here on purpose
# ---> we want to avoid linting bullet physics code
#

message(STATUS "CHECK_CLANG_TIDY -> ${CHECK_CLANG_TIDY}")

if (NOT DEFINED EMSCRIPTEN AND CHECK_CLANG_TIDY)

    message(STATUS "clang-tidy will be attempted")

    find_program(CLANG_TIDY_EXE "clang-tidy")

    if (CLANG_TIDY_EXE)

        # file generated at ./build.release.native/compile_commands.json

        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})

    else()

        message(WARNING "clang-tidy not found!")

    endif()

else()
    message(STATUS "clang-tidy will be ignored")
endif()

#
# ensure geronimo wrappers
#

add_subdirectory(./src/geronimo)

#
# build the test-bed
#

# add_subdirectory(./samples/test-bed)


