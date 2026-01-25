cmake_minimum_required(VERSION 3.24)
project(lib-geronimo-graphics)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    ./src/geronimo/graphics/input-managers/KeyboardManager.cpp
    ./src/geronimo/graphics/input-managers/MouseManager.cpp
    ./src/geronimo/graphics/input-managers/TouchManager.cpp
    ./src/geronimo/graphics/vertexBuffers/VertexBufferObject.cpp
    ./src/geronimo/graphics/vertexBuffers/VertexArrayObject.cpp
    ./src/geronimo/graphics/GeometryBuilder.cpp
    ./src/geronimo/graphics/advanced-concept/stackRenderers/internals/WireFramesStackRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/stackRenderers/internals/TrianglesStackRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred/internals/LightStackRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred/internals/ResultQuadRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred/internals/ScreenRecorder.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred/internals/FrameMerger.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred/DepthDeferred.cpp
    ./src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/LightManager.cpp
    ./src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/PerspectiveClustering.cpp
    ./src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/ResultQuadRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/ScreenRecorder.cpp
    ./src/geronimo/graphics/advanced-concept/clusteredDeferred/ClusteredDeferred.cpp
    ./src/geronimo/graphics/advanced-concept/widgets/helpers/renderTextBackground.cpp
    ./src/geronimo/graphics/advanced-concept/widgets/helpers/writeTime.cpp
    ./src/geronimo/graphics/advanced-concept/widgets/renderHistoricalTimeData.cpp
    ./src/geronimo/graphics/advanced-concept/textRenderer/TextRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred-noSpecular/DepthDeferredNoSPecular.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred-noSpecular/internals/LightStackRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred-noSpecular/internals/ResultQuadRenderer.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred-noSpecular/internals/ScreenRecorder.cpp
    ./src/geronimo/graphics/advanced-concept/depthDeferred-noSpecular/internals/FrameMerger.cpp
    ./src/geronimo/graphics/ShaderProgramBuilder.cpp
    ./src/geronimo/graphics/Texture.cpp
    ./src/geronimo/graphics/RenderBuffer.cpp
    ./src/geronimo/graphics/DataTexture.cpp
    ./src/geronimo/graphics/ShaderProgram.cpp
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
    ./src/geronimo/graphics/sdl/SDLWindowWrapper.cpp
    ./src/geronimo/graphics/FrameBuffer.cpp
    ./src/geronimo/graphics/Geometry.cpp
    ./src/geronimo/graphics/wrappers/GlContext_VertexBufferObject.cpp
    ./src/geronimo/graphics/wrappers/GlContext_Texture.cpp
    ./src/geronimo/graphics/wrappers/GlContext.cpp
    ./src/geronimo/graphics/wrappers/glCheckError.cpp
    ./src/geronimo/graphics/wrappers/GlContext_Shader.cpp
    ./src/geronimo/graphics/wrappers/GlContext_Framebuffers.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE lib-geronimo-system)

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ../helpers/src
    ../system/src
    ../../../thirdparties
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


