
#pragma once

#include <cstdint>

enum class ShadersAliases : int32_t {
  stackRendererHud = 0,
  stackRendererScene,
  geometriesStackRenderer,
  textRenderer,
};

enum class GeometriesAliases : int32_t {
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
