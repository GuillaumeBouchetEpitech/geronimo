
#pragma once

#include "application/states/StateManager.hpp"

#include "graphics/renderers/GraphicsRenderer.hpp"

#include "logic/HeightField.hpp"
#include "logic/controllers/FreeFlyCameraController.hpp"

#include "geronimo/audio/OpenALSoundManager.hpp"
#include "geronimo/graphics/FrameBuffer.hpp"
#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/RenderBuffer.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/PhysicWorld.hpp"
#include "geronimo/system/NonCopyable.hpp"
#include "geronimo/system/metrics/PerformanceProfiler.hpp"

#include "geronimo/graphics/advanced-concept/clusteredDeferred/ClusteredDeferred.hpp"
#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/advanced-concept/textRenderer/TextRenderer.hpp"

#include <array>
#include <cstdint>
#include <list>
#include <memory> // <= unique_ptr / make_unique
#include <string>
#include <unordered_map>

enum class Groups : short {
  all = -1,
  player = (1 << 0),
  ground = (1 << 1),
  dynamic = (1 << 2),
  projectile = (1 << 3),
  sensor = (1 << 4),
};

enum class Masks : short {
  all = -1,
  player = gero::asValue(Groups::ground) | gero::asValue(Groups::projectile) | gero::asValue(Groups::sensor),
  ground = gero::asValue(Groups::player) | gero::asValue(Groups::dynamic) | gero::asValue(Groups::projectile),
  dynamic = (1 << 2),
  projectile = gero::asValue(Groups::ground) | gero::asValue(Groups::player),
  sensor = (1 << 4),
};

class Context : public gero::NonCopyable {

  //
  //
  // singleton

private:
  static Context* _instance;

  Context() = default;
  ~Context();

private:
  void initialize(uint32_t width, uint32_t height);

public:
  static void create(uint32_t width, uint32_t height);
  static void destroy();
  static Context& get();

  // singleton
  //
  //

private:
  void initializePhysicResources();

#if 1
  void experimentalPhysicVehicle();
#endif

public:
  struct Graphic {
    GraphicsRenderer renderer;
  } graphic;

  //

  struct t_physic {
    gero::physics::PhysicWorld* world;
  } physic;

  struct t_audio {
    gero::audio::OpenALSoundManager* soundManager;
  } audio;

  struct Logic {

    gero::metrics::PerformanceProfiler performanceProfiler;

    struct Controllers {
      FreeFlyCameraController freeFly;
    } controllers;

    HeightField heightField;

    float time;

    bool debugMode = false;

  } logic;
};
