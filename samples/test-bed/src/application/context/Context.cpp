
#include "Context.hpp"

#include "application/defines.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/physics/body/PhysicBodyDef.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

//
//
// singleton

Context* Context::_instance = nullptr;

Context::~Context() { delete audio.soundManager; }

void Context::initialize(uint32_t width, uint32_t height) {

  graphic.renderer.initialize(width, height);

  audio.soundManager = new gero::audio::OpenALSoundManager();
  audio.soundManager->setVolume(0.5f);
  audio.soundManager->loadOggFromFile(666, "./assets/audio/Cannon_3.ogg");

  logic.controllers.freeFly.setPosition(glm::vec3(-30, -20, 20));

  logic.basicScene = AbstractBasicScene::create();
  logic.flockingManager = AbstractFlockingManager::create();
  logic.voxelSim = AbstractVoxelSim::create();
  logic.artificialStupiditySim = AbstractArtificialStupiditySim::create();

}

//

void Context::create(uint32_t width, uint32_t height) {
  if (_instance)
    D_THROW(std::runtime_error, "Context singleton already initialized");

  _instance = new Context();
  _instance->initialize(width, height);
}

void Context::destroy() {
  if (!_instance)
    D_THROW(std::runtime_error, "Context singleton already destroyed");

  delete _instance, _instance = nullptr;
}

Context& Context::get() {
  if (!_instance)
    D_THROW(std::runtime_error, "Context singleton not initialized");

  return *_instance;
}

// singleton
//
//
