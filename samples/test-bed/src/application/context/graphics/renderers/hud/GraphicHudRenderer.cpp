
#include "GraphicHudRenderer.hpp"

void GraphicHudRenderer::initialize(uint32_t width, uint32_t height) {
  _camera.setOrthographic(0.0f, float(width), 0.0f, float(height), -10.0f, +10.0f);
  _camera.setSize(width, height);

  const glm::vec3 eye = {0.0f, 0.0f, 1.0f};
  const glm::vec3 center = {0.0f, 0.0f, 0.0f};
  const glm::vec3 upAxis = {0.0f, 1.0f, 0.0f};
  _camera.lookAt(eye, center, upAxis);

  _stackRenderers.initialize("./assets/graphics/hud/advanced-concept-sub-assets/stackRenderers");
  _textRenderer.initialize("./assets/graphics/hud/advanced-concept-sub-assets/textRenderer");

  // samples/test-bed/assets/graphics/advanced-concept-sub-assets/depthDeferred

}

void GraphicHudRenderer::resize(uint32_t width, uint32_t height) { _camera.setSize(width, height); }

void GraphicHudRenderer::computeMatrices() {
  _camera.computeMatrices();
  _stackRenderers.setMatricesData(_camera.getMatricesData());
  _textRenderer.setMatricesData(_camera.getMatricesData());
}

gero::graphics::ICamera& GraphicHudRenderer::getCamera() { return _camera; }
gero::graphics::IStackRenderers& GraphicHudRenderer::getStackRenderers() { return _stackRenderers; }
gero::graphics::ITextRenderer& GraphicHudRenderer::getTextRenderer() { return _textRenderer; }
