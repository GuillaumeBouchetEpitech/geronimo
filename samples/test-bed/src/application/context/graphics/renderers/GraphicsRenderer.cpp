
#include "GraphicsRenderer.hpp"

#include "geronimo/graphics/opengl/GlContext.hpp"

void GraphicsRenderer::initialize(uint32_t width, uint32_t height) {
  D_MYLOG("MaxTextureSize: " << gero::graphics::opengl::GlContext::getMaxTextureSize());

  _sceneRenderer.initialize(width, height);
  _hudRenderer.initialize(width, height);
}

void GraphicsRenderer::resize(uint32_t width, uint32_t height) {
  _sceneRenderer.resize(width, height);
  _hudRenderer.resize(width, height);
}

void GraphicsRenderer::computeMatrices() {
  _sceneRenderer.computeMatrices();
  _hudRenderer.computeMatrices();
}

IGraphicSceneRenderer& GraphicsRenderer::getSceneRenderer() { return _sceneRenderer; }

IGraphicHudRenderer& GraphicsRenderer::getHudRenderer() { return _hudRenderer; }
