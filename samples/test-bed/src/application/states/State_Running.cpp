
#include "State_Running.hpp"

#include "StateManager.hpp"

#include "geronimo/graphics/input-managers/KeyboardManager.hpp"
#include "geronimo/graphics/input-managers/MouseManager.hpp"
#include "geronimo/graphics/input-managers/TouchManager.hpp"

#include "application/context/Context.hpp"
#include "application/context/graphics/Scene.hpp"
#include "application/defines.hpp"

#include "geronimo/helpers/GLMath.hpp"
// #include "geronimo/physics/body/PhysicBody.hpp"
#include "geronimo/physics/queries/ray-caster/RayCaster.hpp"
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
  auto& touch = TouchManager::get();

  switch (event.type) {
  case SDL_KEYDOWN: {
    keyboard.updateAsPressed(event.key.keysym.sym);
    break;
  }
  case SDL_KEYUP: {

    if (keyboard.isPressed(SDLK_g))
      context.logic.debugMode = !context.logic.debugMode;

    if (keyboard.isPressed(SDLK_h)) {
      context.logic.pauseMode = !context.logic.pauseMode;
    }
    if (keyboard.isPressed(SDLK_k)) {
      context.logic.pauseMode = true;
      context.logic.framesLeftToPlay += 1;
    }

    keyboard.updateAsReleased(event.key.keysym.sym);

    break;
  }

  case SDL_MOUSEBUTTONDOWN: {
    // SDL_BUTTON_LEFT
    // SDL_BUTTON_MIDDLE
    // SDL_BUTTON_RIGHT

    mouse.updateAsPressed(event.button.button);

    if (event.button.button == SDL_BUTTON_LEFT) {
      context.audio.soundManager->playAbsoluteSound(666, 10, 1);
    }

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

  case SDL_FINGERDOWN: {
    const glm::vec2 vSize = glm::vec2(context.graphic.renderer.getHudRenderer().getCamera().getSize());
    const glm::vec2 currPos = glm::vec2(event.tfinger.x, event.tfinger.y) * vSize;
    touch.updateAsTouchedDown(int32_t(event.tfinger.fingerId), currPos);
    break;
  }
  case SDL_FINGERUP: {
    touch.updateAsTouchedUp(int32_t(event.tfinger.fingerId));
    break;
  }
  case SDL_FINGERMOTION: {
    const glm::vec2 vSize = glm::vec2(context.graphic.renderer.getHudRenderer().getCamera().getSize());
    const glm::vec2 currPos = glm::vec2(event.tfinger.x, event.tfinger.y) * vSize;
    const glm::vec2 currDelta = glm::vec2(event.tfinger.dx, event.tfinger.dy) * vSize;
    touch.updateAsTouchedMotion(int32_t(event.tfinger.fingerId), currPos, currDelta);
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
  performanceProfiler.stop("FRAME");
  performanceProfiler.start("FRAME");

  performanceProfiler.start("1 UPDATE");

  {
    context.logic.controllers.freeFly.update(elapsedTime);

    auto& currCamera = context.graphic.renderer.getSceneRenderer().getCamera();

    context.audio.soundManager->setListener(currCamera.getEye(), currCamera.getForwardAxis(), glm::vec3(0, 0, 1));

    // if (context.inputs.mouse.buttons[SDL_BUTTON_LEFT] == true) {
    // }

    if (context.logic.pauseMode == false || context.logic.framesLeftToPlay > 0) {
      if (context.logic.framesLeftToPlay > 0) {
        context.logic.framesLeftToPlay -= 1;
      }

      context.logic.flockingManager->update(elapsedTime);

      context.logic.time += elapsedTime;

      performanceProfiler.start("1 update physic");

      constexpr uint32_t k_maxSubSteps = 3;
      constexpr float k_fixedStep = 1.0f / 60.0f;
      context.physic.world->step(elapsedTime, k_maxSubSteps, k_fixedStep);

      performanceProfiler.stop("1 update physic");
    }
  }

  performanceProfiler.stop("1 UPDATE");
}

void State_Running::render(const SDL_Window&) {

  auto& context = Context::get();

  auto& performanceProfiler = context.logic.performanceProfiler;
  performanceProfiler.stop("2 render frame");
  performanceProfiler.start("2 render frame");

  performanceProfiler.start("2 RENDER");

  Scene::renderAll();

  performanceProfiler.stop("2 RENDER");
}

void State_Running::resize(uint32_t width, uint32_t height) { Context::get().graphic.renderer.resize(width, height); }

void State_Running::visibility(bool visible) {
  auto* stateManager = StateManager::get();
  StateManager::States currentState = stateManager->getState();

  if (currentState != StateManager::States::Paused && !visible) {
    stateManager->changeState(StateManager::States::Paused);
  }
}
