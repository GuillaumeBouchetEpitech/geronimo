
#include "Application.hpp"

#include "states/StateManager.hpp"

#include "context/Context.hpp"

#include "geronimo/graphics/inputs/KeyboardManager.hpp"
#include "geronimo/graphics/inputs/MouseManager.hpp"
#include "geronimo/graphics/inputs/TouchManager.hpp"

#include "context/graphics/Scene.hpp"

#include "geronimo/system/TraceLogger.hpp"

#include <chrono>

namespace {

#if defined D_NATIVE_BUILD
constexpr bool k_canResize = true;
constexpr uint32_t k_frameRate = 120;
#else
constexpr bool k_canResize = false;
constexpr uint32_t k_frameRate = 0;
#endif
} // namespace

Application::Application(const Definition& def)
  : SDLWindowWrapper("Test Bed", def.width, def.height, k_frameRate, OpenGlEsVersion::v3, k_canResize) {

  gero::graphics::inputs::KeyboardManager::create();
  gero::graphics::inputs::MouseManager::create();
  gero::graphics::inputs::TouchManager::create();
  Context::create(def.width, def.height);
  StateManager::create();

  Scene::initialize();
}

Application::~Application() {
  StateManager::destroy();
  Context::destroy();
  gero::graphics::inputs::TouchManager::destroy();
  gero::graphics::inputs::MouseManager::destroy();
  gero::graphics::inputs::KeyboardManager::destroy();
}

//

void Application::setMouseLockStatus(bool isLocked) {

  auto& mouse = gero::graphics::inputs::MouseManager::get();
  if (mouse.setLock(isLocked))
    mouse.reset();
}

//

void Application::_onEvent(const SDL_Event& event) { StateManager::get()->handleEvent(event); }

void Application::_onUpdate(uint32_t deltaTime) {

  constexpr uint32_t k_maxDelta = 16;
  ;
  if (deltaTime > k_maxDelta)
    deltaTime = k_maxDelta; // cap it

  StateManager::get()->update(deltaTime);
}

void Application::_onRender(const SDL_Window& screen) { StateManager::get()->render(screen); }

void Application::_onResize(uint32_t width, uint32_t height) { StateManager::get()->resize(width, height); }

void Application::_onVisibilityChange(bool visible) { StateManager::get()->visibility(visible); }
