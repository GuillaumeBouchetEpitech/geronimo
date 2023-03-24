
#pragma once

#include <cstdint>

enum class ShaderAliases : int32_t {
  stackRendererHud = 0,
  stackRendererScene,
  geometriesStackRenderer,
  textRenderer,
};

enum class GeometryIds : int32_t {
  stackRendererWireFramesHud = 0,
  stackRendererTrianglesHud,
  stackRendererWireFramesScene,
  stackRendererTrianglesScene,
  geometriesStackRenderer,
  textRenderer,
};

enum class TextureIds : int32_t {
  asciiFont = 0,
};
