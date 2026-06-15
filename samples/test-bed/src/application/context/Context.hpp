
#pragma once

#include "application/states/StateManager.hpp"

#include "graphics/renderers/GraphicsRenderer.hpp"

#include "logic/controllers/FreeFlyCameraController.hpp"

#include "logic/basic-scene/AbstractBasicScene.hpp"
#include "logic/flocking/AbstractFlockingManager.hpp"
#include "logic/voxel-sim/AbstractVoxelSim.hpp"
#include "logic/artificial-stupidity-sim/AbstractArtificialStupiditySim.hpp"
#include "logic/city-architect/AbstractCityArchitect.hpp"

#include "geronimo/audio/OpenALSoundManager.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/AbstractPhysicWorld.hpp"
#include "geronimo/system/NonCopyable.hpp"
#include "geronimo/system/metrics/PerformanceProfiler.hpp"

#include <array>
#include <cstdint>
#include <list>
#include <memory> // <= unique_ptr / make_unique
#include <string>
#include <unordered_map>

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

public:
  struct Graphic {
    GraphicsRenderer renderer;
  } graphic;

  //

  struct t_audio {
    gero::audio::OpenALSoundManager* soundManager;
  } audio;

  struct Logic {

    gero::metrics::PerformanceProfiler performanceProfiler;

    struct Controllers {
      FreeFlyCameraController freeFly;
    } controllers;

    std::unique_ptr<AbstractBasicScene> basicScene;
    std::unique_ptr<AbstractFlockingManager> flockingManager;
    std::unique_ptr<AbstractVoxelSim> voxelSim;
    std::unique_ptr<AbstractArtificialStupiditySim> artificialStupiditySim;
    std::unique_ptr<AbstractCityArchitect> cityArchitect;

    float time;

    bool debugMode = false;
    bool pauseMode = false;
    int32_t framesLeftToPlay = 0;

  } logic;
};
