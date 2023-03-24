
#pragma once

#include "application/context/Context.hpp"

#include "geronimo/helpers/GLMath.hpp"

class Scene {
public:
  static void initialize();
  static void renderAll();

  static void updateMatrices();

private:
  static void _clear();

  static void _renderScene();
  static void _renderHud();
};
