cmake_minimum_required(VERSION 3.24)

project(geronimo-graphics)

set(CMAKE_CXX_STANDARD 20)

set(GERONIMO_DIR "${PROJECT_SOURCE_DIR}/../../..")
set(THIRD_PARTIES_DIR ${GERONIMO_DIR}/thirdparties)

set(SOURCE_FILES
    ./src/geronimo/graphics/inputs/KeyboardManager.cpp
    ./src/geronimo/graphics/inputs/MouseManager.cpp
    ./src/geronimo/graphics/inputs/TouchManager.cpp
    ./src/geronimo/graphics/opengl/vertexBuffers/VertexBufferObject.cpp
    ./src/geronimo/graphics/opengl/vertexBuffers/VertexArrayObject.cpp
    ./src/geronimo/graphics/opengl/GeometryBuilder.cpp
    ./src/geronimo/graphics/opengl/advanced/stackRenderers/internals/WireFramesStackRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/stackRenderers/internals/TrianglesStackRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred/internals/LightStackRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred/internals/ResultQuadRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred/internals/ScreenRecorder.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred/internals/FrameMerger.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred/DepthDeferred.cpp
    # ./src/geronimo/graphics/opengl/advanced/clusteredDeferred/internals/LightManager.cpp
    # ./src/geronimo/graphics/opengl/advanced/clusteredDeferred/internals/PerspectiveClustering.cpp
    # ./src/geronimo/graphics/opengl/advanced/clusteredDeferred/internals/ResultQuadRenderer.cpp
    # ./src/geronimo/graphics/opengl/advanced/clusteredDeferred/internals/ScreenRecorder.cpp
    # ./src/geronimo/graphics/opengl/advanced/clusteredDeferred/ClusteredDeferred.cpp
    ./src/geronimo/graphics/opengl/advanced/widgets/helpers/renderTextBackground.cpp
    ./src/geronimo/graphics/opengl/advanced/widgets/helpers/writeTime.cpp
    ./src/geronimo/graphics/opengl/advanced/widgets/renderHistoricalTimeData.cpp
    ./src/geronimo/graphics/opengl/advanced/textRenderer/TextRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred-noSpecular/DepthDeferredNoSPecular.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred-noSpecular/internals/LightStackRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred-noSpecular/internals/ResultQuadRenderer.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred-noSpecular/internals/ScreenRecorder.cpp
    ./src/geronimo/graphics/opengl/advanced/depthDeferred-noSpecular/internals/FrameMerger.cpp
    ./src/geronimo/graphics/opengl/ShaderProgramBuilder.cpp
    ./src/geronimo/graphics/opengl/Texture.cpp
    ./src/geronimo/graphics/opengl/RenderBuffer.cpp
    ./src/geronimo/graphics/opengl/DataTexture.cpp
    ./src/geronimo/graphics/opengl/ShaderProgram.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_makeBox.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_makeCylinder.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_makeSphere.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_makeDisk.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_utilities.cpp
    ./src/geronimo/graphics/make-geometries/MakeGeometries_makeTorus.cpp
    ./src/geronimo/graphics/camera/Camera.cpp
    ./src/geronimo/graphics/camera/sceneToScreen.cpp
    ./src/geronimo/graphics/camera/FrustumCulling.cpp
    ./src/geronimo/graphics/ResourceManager.cpp
    ./src/geronimo/graphics/Image.cpp
    ./src/geronimo/graphics/loaders/loadObjModel.cpp
    ./src/geronimo/graphics/opengl/sdl2/SDLWindowWrapper.cpp
    ./src/geronimo/graphics/opengl/FrameBuffer.cpp
    ./src/geronimo/graphics/opengl/Geometry.cpp
    ./src/geronimo/graphics/opengl/wrappers/GlContext_VertexBufferObject.cpp
    ./src/geronimo/graphics/opengl/wrappers/GlContext_Texture.cpp
    ./src/geronimo/graphics/opengl/wrappers/GlContext.cpp
    ./src/geronimo/graphics/opengl/wrappers/glCheckError.cpp
    ./src/geronimo/graphics/opengl/wrappers/GlContext_Shader.cpp
    ./src/geronimo/graphics/opengl/wrappers/GlContext_Framebuffers.cpp
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

    add_executable(unit_test__graphics
        ./src/geronimo/graphics/camera/FrustumCulling.tests.cpp
    )
    add_dependencies(unit_test__graphics ${PROJECT_NAME})

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GRAPHICS_UNIT_TESTS REQUIRED IMPORTED_TARGET GLOBAL sdl2 glesv2)
    target_link_libraries(unit_test__graphics
        ${PROJECT_NAME}
        geronimo-system
        # bullet-dynamics
        # bullet-collision
        # bullet-linearmath
        gtest_main
        gtest
        PkgConfig::GRAPHICS_UNIT_TESTS
        pthread
    )

    target_include_directories(unit_test__graphics PRIVATE
        ./src
        ../helpers/src
        ../system/src
        ${THIRD_PARTIES_DIR}
    )

    set_target_properties(unit_test__graphics PROPERTIES COMPILE_FLAGS "-O3")
    set_target_properties(unit_test__graphics PROPERTIES LINK_FLAGS "-O3")
    set_target_properties(unit_test__graphics PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

    # UNIT TESTS
    # UNIT TESTS
    # UNIT TESTS


endif()


