
#include "State_Paused.hpp"

#include "StateManager.hpp"

#include "application/context/Context.hpp"
#include "application/context/graphics/Scene.hpp"

#include "geronimo/helpers/GLMath.hpp"

void State_Paused::enter() { D_MYLOG("step"); }

void State_Paused::leave() { D_MYLOG("step"); }

void State_Paused::handleEvent(const SDL_Event& event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP:
  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    StateManager::get()->returnToPreviousState();
    break;
  }
  }
}

void State_Paused::update(uint32_t delta) {
  static_cast<void>(delta); // <= unused
}

void State_Paused::render(const SDL_Window&) {
  // static_cast<void>(window); // <= unused

  Scene::renderAll();
}

void State_Paused::resize(uint32_t width, uint32_t height) { Context::get().graphic.renderer.resize(width, height); }

void State_Paused::visibility(bool visible) {
  static_cast<void>(visible); // <= unused
}
