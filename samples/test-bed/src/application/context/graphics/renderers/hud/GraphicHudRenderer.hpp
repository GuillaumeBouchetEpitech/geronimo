
#pragma once

#include "geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/opengl/advanced/textRenderer/TextRenderer.hpp"

class IGraphicHudRenderer {
public:
  virtual gero::graphics::camera::ICamera& getCamera() = 0;
  virtual gero::graphics::opengl::advanced::IStackRenderers& getStackRenderers() = 0;
  virtual gero::graphics::opengl::advanced::ITextRenderer& getTextRenderer() = 0;
};

class GraphicHudRenderer : public IGraphicHudRenderer {
public:
  GraphicHudRenderer() = default;
  ~GraphicHudRenderer() = default;

public:
  void initialize(uint32_t width, uint32_t height);
  void resize(uint32_t width, uint32_t height);
  void computeMatrices();

public:
  gero::graphics::camera::ICamera& getCamera() override;
  gero::graphics::opengl::advanced::IStackRenderers& getStackRenderers() override;
  gero::graphics::opengl::advanced::ITextRenderer& getTextRenderer() override;

private:
  gero::graphics::camera::Camera _camera;

  gero::graphics::opengl::advanced::StackRenderers _stackRenderers;
  gero::graphics::opengl::advanced::TextRenderer _textRenderer;
};
