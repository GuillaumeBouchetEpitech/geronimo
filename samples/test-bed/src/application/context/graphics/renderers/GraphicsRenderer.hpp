
#pragma once

#include "hud/GraphicHudRenderer.hpp"
#include "scene/GraphicSceneRenderer.hpp"

class GraphicsRenderer {
public:
  GraphicsRenderer() = default;
  ~GraphicsRenderer() = default;

public:
  void initialize(uint32_t width, uint32_t height);

public:
  void resize(uint32_t width, uint32_t height);
  void computeMatrices();

public:
  IGraphicSceneRenderer& getSceneRenderer();
  IGraphicHudRenderer& getHudRenderer();

private:
  GraphicSceneRenderer _sceneRenderer;
  GraphicHudRenderer _hudRenderer;
};
