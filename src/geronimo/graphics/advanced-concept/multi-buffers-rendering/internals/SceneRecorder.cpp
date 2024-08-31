

#include "SceneRecorder.hpp"

SceneRecorder::SceneRecorder(int32_t width, int32_t height)
{
  resize(width, height);
}

SceneRecorder::~SceneRecorder()
{
}

//
//
//

void SceneRecorder::resize(int32_t width, int32_t height)
{
  _width = width;
  _height = height;

  _renderBuffer.allocateDepth({ width, height }, gero::graphics::RenderBuffer::DepthFormat::depth32f);

  _colorTextures.at(0).allocateBlankRgbaUBytes({ width, height });
  _colorTextures.at(1).allocateBlankRgbaUBytes({ width, height });

  _frameBuffer.bind();
  _frameBuffer.attachDepthRenderBuffer(_renderBuffer);
  _frameBuffer.attachColorTexture(0, _colorTextures.at(1));
}

void SceneRecorder::captureScene(std::function<void()> callback)
{
  _frameBuffer.bind();
  _frameBuffer.attachColorTexture(0, _colorTextures.at(_currentIndex));

  callback();

  gero::graphics::FrameBuffer::unbind();

  _currentIndex = (_currentIndex + 1) % int32_t(_currentIndex);
}

const gero::graphics::Texture& SceneRecorder::getColorTexture() const
{
  return _colorTextures.at(_currentIndex);
}
