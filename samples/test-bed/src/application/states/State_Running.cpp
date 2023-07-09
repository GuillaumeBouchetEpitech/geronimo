
#include "State_Running.hpp"

#include "StateManager.hpp"

#include "application/context/logic/inputManagers/KeyboardManager.hpp"
#include "application/context/logic/inputManagers/MouseManager.hpp"

#include "application/context/Context.hpp"
#include "application/context/graphics/Scene.hpp"
#include "application/defines.hpp"

#include "geronimo/helpers/GLMath.hpp"
// #include "geronimo/physics/body/PhysicBody.hpp"
#include "geronimo/physics/queries/raycaster/Raycaster.hpp"
#include "geronimo/system/math/constants.hpp"

void State_Running::enter() {
  D_MYLOG("step");

  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void State_Running::leave() {
  D_MYLOG("step");

  SDL_SetRelativeMouseMode(SDL_FALSE);
}

void State_Running::handleEvent(const SDL_Event& event) {
  auto& context = Context::get();

  auto& keyboard = KeyboardManager::get();
  auto& mouse = MouseManager::get();

  switch (event.type) {
  case SDL_KEYDOWN: {
    keyboard.updateAsPressed(event.key.keysym.sym);
    break;
  }
  case SDL_KEYUP: {

    if (keyboard.isPressed(SDLK_g))
      context.logic.debugMode = !context.logic.debugMode;

    keyboard.updateAsReleased(event.key.keysym.sym);

    break;
  }

  case SDL_MOUSEBUTTONDOWN: {
    // SDL_BUTTON_LEFT
    // SDL_BUTTON_MIDDLE
    // SDL_BUTTON_RIGHT

    mouse.updateAsPressed(event.button.button);
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    mouse.updateAsReleased(event.button.button);
    break;
  }
  case SDL_MOUSEMOTION: {

    float deltaX = float(event.motion.xrel);
    float deltaY = float(event.motion.yrel);

#ifdef D_WEB_BUILD
    deltaX *= 0.5f;
    deltaY *= 0.5f;
#else
#endif

    mouse.updateDelta(deltaX, deltaY);

    break;
  }

  default:
    break;
  }
}

void State_Running::update(uint32_t delta) {

  const float elapsedTime = float(delta) / 1000.0f;

  auto& context = Context::get();

  auto& performanceProfiler = context.logic.performanceProfiler;
  performanceProfiler.stop("complete frame");
  performanceProfiler.start("complete frame");

  performanceProfiler.start("update");

  auto& mouse = MouseManager::get();

  if (mouse.isLocked()) {

    context.logic.controllers.freeFly.update(elapsedTime);

    context.logic.time += elapsedTime;

    context.audio.soundManager->setListener(
      context.graphic.camera.scene.getEye(), context.graphic.camera.scene.getForwardAxis(), glm::vec3(0, 0, 1));

    // if (context.inputs.mouse.buttons[SDL_BUTTON_LEFT] == true) {
    // }

    constexpr uint32_t k_maxSubSteps = 3;
    constexpr float k_fixedStep = 1.0f / 60.0f;
    context.physic.world->step(elapsedTime, k_maxSubSteps, k_fixedStep);
  }

  performanceProfiler.stop("update");
}

void State_Running::render(const SDL_Window&) {

  auto& context = Context::get();

  auto& performanceProfiler = context.logic.performanceProfiler;
  performanceProfiler.stop("render frame");
  performanceProfiler.start("render frame");

  performanceProfiler.start("render");

  Scene::updateMatrices();

  Scene::renderAll();

  performanceProfiler.stop("render");
}

void State_Running::resize(uint32_t width, uint32_t height) {
  auto& context = Context::get();
  auto& graphic = context.graphic;

  graphic.camera.viewportSize = {width, height};
  graphic.camera.scene.setSize(width, height);
  graphic.camera.hud.setSize(width, height);
}

void State_Running::visibility(bool visible) {
  auto* stateManager = StateManager::get();
  StateManager::States currentState = stateManager->getState();

  if (currentState != StateManager::States::Paused && !visible)
    stateManager->changeState(StateManager::States::Paused);
}
