
#pragma once

#include "geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.hpp"

class SceneStackRenderers : public gero::graphics::opengl::advanced::StackRenderers {
public:
  SceneStackRenderers() = default;
  ~SceneStackRenderers() = default;

public:
  void initialize();
};
