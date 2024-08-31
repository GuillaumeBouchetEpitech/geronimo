
#pragma once

// #include "geronimo/helpers/GLMath.hpp"

#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/RenderBuffer.hpp"
#include "geronimo/graphics/Texture.hpp"

#include <functional>

#include <cstdint>

class SceneRecorder {

public:
  SceneRecorder(int32_t width, int32_t height);
  ~SceneRecorder();

public:
  void resize(int32_t width, int32_t height);

  void captureScene(std::function<void()> callback);

  const gero::graphics::Texture& getColorTexture() const;

private:

  int32_t _width = 0;
  int32_t _height = 0;

  gero::graphics::FrameBuffer _frameBuffer;
  gero::graphics::RenderBuffer _renderBuffer;
  std::array<gero::graphics::Texture, 2> _colorTextures;
  int32_t _currentIndex = 0;

};
