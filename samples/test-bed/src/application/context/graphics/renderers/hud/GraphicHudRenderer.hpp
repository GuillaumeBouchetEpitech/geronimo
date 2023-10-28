
#pragma once

#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/advanced-concept/textRenderer/TextRenderer.hpp"


class IGraphicHudRenderer
{
public:
  virtual gero::graphics::ICamera& getCamera() = 0;
  virtual gero::graphics::IStackRenderers& getStackRenderers() = 0;
  virtual gero::graphics::ITextRenderer& getTextRenderer() = 0;
};

class GraphicHudRenderer : public IGraphicHudRenderer
{
public:
  GraphicHudRenderer() = default;
  ~GraphicHudRenderer() = default;

public:
  void initialize(uint32_t width, uint32_t height);
  void resize(uint32_t width, uint32_t height);
  void computeMatrices();

public:
  gero::graphics::ICamera& getCamera() override;
  gero::graphics::IStackRenderers& getStackRenderers() override;
  gero::graphics::ITextRenderer& getTextRenderer() override;

private:
  gero::graphics::Camera _camera;

  gero::graphics::StackRenderers _stackRenderers;
  gero::graphics::TextRenderer _textRenderer;
};
