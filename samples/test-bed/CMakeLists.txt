
cmake_minimum_required(VERSION 3.24)

project(test-bed)

add_executable(${PROJECT_NAME}
    ./src/main.cpp
    ./src/application/context/logic/controllers/FreeFlyCameraController.cpp
    ./src/application/context/logic/flocking/internals/Boid.cpp
    ./src/application/context/logic/flocking/internals/FlockingManager.cpp
    ./src/application/context/logic/flocking/AbstractFlockingManager.cpp
    ./src/application/context/logic/HeightField.cpp
    ./src/application/context/Context_experimentalPhysicVehicle.cpp
    ./src/application/context/Context.cpp
    ./src/application/context/graphics/renderers/hud/GraphicHudRenderer.cpp
    ./src/application/context/graphics/renderers/scene/helpers/renderPhysicVehicle.cpp
    ./src/application/context/graphics/renderers/scene/helpers/renderPhysicBody.cpp
    ./src/application/context/graphics/renderers/scene/scene-stack-renderers/SceneStackRenderers.cpp
    ./src/application/context/graphics/renderers/scene/GraphicSceneRenderer.cpp
    ./src/application/context/graphics/renderers/scene/geometries-stack-renderer/GeometriesStackRenderer.cpp
    ./src/application/context/graphics/renderers/GraphicsRenderer.cpp
    ./src/application/context/graphics/Scene.cpp
    ./src/application/context/Context_initializePhysicResources.cpp
    ./src/application/Application.cpp
    ./src/application/states/State_Running.cpp
    ./src/application/states/StateManager.cpp
    ./src/application/states/State_Paused.cpp
)

# target_include_directories(${PROJECT_NAME} PRIVATE
#     ../../../network-wrapper/src/
# )

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ../../src/geronimo/system/src
    ../../src/geronimo/graphics/src
    ../../src/geronimo/physics/src
    ../../src/geronimo/audio/src
    ../../src/geronimo/helpers/src
    ../../thirdparties
    ../../thirdparties/dependencies
    ../../thirdparties/dependencies/bullet3/src
    ../../thirdparties/dependencies/entt/src/entt/entity
    ../../thirdparties/dependencies/json/include
)

add_compile_definitions(BT_NO_PROFILE)
add_compile_definitions(SIMD_FORCE_INLINE=inline)

target_link_libraries(${PROJECT_NAME} PRIVATE
    lib-bullet-dynamics
    lib-bullet-collision
    lib-bullet-linearmath
    lib-geronimo-audio
    lib-geronimo-physics
    lib-geronimo-graphics
    lib-geronimo-system
)





# # sdl2 glesv2 openal
# target_link_libraries(${PROJECT_NAME} PRIVATE -lSDL2 -lGLESv2 -lopenal)

# find_package(PkgConfig REQUIRED)

# pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET GLOBAL sdl2)
# # pkg_check_modules(YOUR_PKG REQUIRED IMPORTED_TARGET GLOBAL ya-package)

# target_link_libraries(${PROJECT_NAME} PUBLIC
#     PkgConfig::SDL2
#     # PkgConfig::YOUR_PKG
# )

# pkg_check_modules(SDL2 REQUIRED sdl2)
# target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
# target_compile_options(${PROJECT_NAME} PUBLIC ${SDL2_CFLAGS_OTHER})

# pkg_check_modules(GLES REQUIRED glesv2)
# target_link_libraries(${PROJECT_NAME} ${GLES_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PUBLIC ${GLES_INCLUDE_DIRS})
# target_compile_options(${PROJECT_NAME} PUBLIC ${GLES_CFLAGS_OTHER})

# pkg_check_modules(OPEN_AL REQUIRED openal)
# target_link_libraries(${PROJECT_NAME} ${OPEN_AL_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PUBLIC ${OPEN_AL__INCLUDE_DIRS})
# target_compile_options(${PROJECT_NAME} PUBLIC ${OPEN_AL__CFLAGS_OTHER})





if (DEFINED EMSCRIPTEN)

    # this makes sure the --preload-file flag find the assets folder
    add_custom_command(
        TARGET ${PROJECT_NAME}  # Tie the command to the "MyApp" target
        PRE_BUILD    # Run before the target is built
        COMMAND ${CMAKE_COMMAND} -E copy_directory  # CMake's cross-platform copy command
            ${CMAKE_SOURCE_DIR}/samples/test-bed/assets     # Source directory
            ${CMAKE_CURRENT_BINARY_DIR}/assets              # Destination directory
        COMMENT "Copying assets files to build directory"  # Optional: Log message
    )

    # emscripten will generate both .js and .wasm files
	set(CMAKE_EXECUTABLE_SUFFIX ".js")

	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3 -s USE_SDL=2 -s USE_PTHREADS=0")

    set(EM_LDFLAGS "")
    set(EM_LDFLAGS "${EM_LDFLAGS} -O3")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s WASM=1")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s USE_SDL=2")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s USE_PTHREADS=0")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s TOTAL_MEMORY=128Mb")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s GL_PREINITIALIZED_CONTEXT=1")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s USE_WEBGL2=1")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s FULL_ES3=1")
    set(EM_LDFLAGS "${EM_LDFLAGS} --preload-file ./assets/")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s EXPORT_NAME=\"testBedApp\"")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s MODULARIZE=1")
    set(EM_LDFLAGS "${EM_LDFLAGS} -s EXPORTED_RUNTIME_METHODS=\"['cwrap']\"")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${EM_LDFLAGS})

    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/dist/wasm")

    # ensure the web-wasm-loader page/script
    execute_process(COMMAND sh ./scripts/sh_ensure_web_wasm_loader.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE ENSURE_WEB_WASM_LOADER_RESULT)

    if(NOT ENSURE_WEB_WASM_LOADER_RESULT EQUAL "0")
        message(FATAL_ERROR "sh ./scripts/sh_ensure_web_wasm_loader.sh failed with ${ENSURE_WEB_WASM_LOADER_RESULT}")
    endif()

else()

    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET GLOBAL sdl2 glesv2 openal)
    # pkg_check_modules(YOUR_PKG REQUIRED IMPORTED_TARGET GLOBAL ya-package)

    target_link_libraries(${PROJECT_NAME} PUBLIC
        PkgConfig::SDL2
        # PkgConfig::YOUR_PKG
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-O3")

    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

endif()


