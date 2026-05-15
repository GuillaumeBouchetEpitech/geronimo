
#pragma once

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/camera/Camera.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

struct StackElement {
  glm::vec3 position;
  glm::vec2 texCoord;
  glm::vec3 color;
};

struct FrameElementRect {
  glm::vec2 bottomLeft;
  glm::vec2 topRight;
};

struct FrameElement {
  FrameElementRect pos;
  FrameElementRect texcoord;
  glm::vec3 color;
  float depth;
};

class SpriteStackRenderer : public gero::NonCopyable
{
public:
  SpriteStackRenderer();
  ~SpriteStackRenderer();

public:
  void initialize();
  void setMatricesData(const gero::graphics::Camera::MatricesData& matricesData);

public:
  void push(const StackElement& newElem);
  void pushFrame(const FrameElement& newElem);
  void render();
  void clear();

private:
  std::shared_ptr<gero::graphics::ShaderProgram> _shader = nullptr;
  gero::graphics::Geometry _geometry;
  gero::graphics::Camera::MatricesData _matricesData;

  std::vector<StackElement> _stack;

};
