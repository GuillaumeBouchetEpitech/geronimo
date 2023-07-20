
#include "Context.hpp"

#include "application/defines.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/physics/body/PhysicBodyDef.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <iomanip>
#include <sstream>

//
//
// singleton

Context* Context::_instance = nullptr;

Context::~Context() {

  delete physic.world;
  delete audio.soundManager;
}

void Context::initialize(uint32_t width, uint32_t height) {
  graphic.camera.viewportSize = {width, height};

  initializeGraphicResources();

  physic.world = new gero::physics::PhysicWorld();
  physic.world->setDebuggerPushLine([this](const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) {
    graphic.scene.stackRenderers.getWireFramesStack().pushLine(posA, posB, color);
  });

  audio.soundManager = new gero::audio::OpenALSoundManager();
  audio.soundManager->setVolume(0.5f);
  audio.soundManager->loadOggFromFile(666, "./assets/audio/Cannon_3.ogg");

  // this->logic.controllers.freeFly.setPosition(glm::vec3(10, 10, 10));

  initializePhysicResources();
  experimentalPhysicVehicle();
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
