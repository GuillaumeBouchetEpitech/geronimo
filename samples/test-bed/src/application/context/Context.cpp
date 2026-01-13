
#include "Context.hpp"

#include "application/defines.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/physics/body/PhysicBodyDef.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include "geronimo/helpers/Json.hpp"

#include <iomanip>
#include <sstream>
#include <fstream>

//
//
// singleton

Context* Context::_instance = nullptr;

Context::~Context() {
  delete audio.soundManager;
}

void Context::initialize(uint32_t width, uint32_t height) {

  graphic.renderer.initialize(width, height);

  physic.world = gero::physics::AbstractPhysicWorld::create();
  physic.world->setDebuggerPushLine([this](const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) {
    graphic.renderer.getSceneRenderer().getStackRenderers().getWireFramesStack().pushLine(posA, posB, color);
  });

  audio.soundManager = new gero::audio::OpenALSoundManager();
  audio.soundManager->setVolume(0.5f);
  audio.soundManager->loadOggFromFile(666, "./assets/audio/Cannon_3.ogg");

  logic.controllers.freeFly.setPosition(glm::vec3(-30, -20, 20));

  initializePhysicResources();
  experimentalPhysicVehicle();

  logic.flockingManager = AbstractFlockingManager::create();
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
