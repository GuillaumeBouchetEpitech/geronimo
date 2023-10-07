
#pragma once

#include "geronimo/helpers/SDL.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <cstdint>
#include <unordered_map>

namespace gero {
namespace graphics {

class SDLWindowWrapper : public NonCopyable {
public:
  enum class OpenGlEsVersion {
    v2 = 2, // OpenGl ES2 -> WebGL1
    v3 = 3, // OpenGl ES3 -> WebGL2
  };

private:
  SDL_Window* _window = nullptr;
  SDL_GLContext _glContextId = 0;

  uint32_t _framesPerSecond = 0;

  bool _running = false;
  uint32_t _startTime = 0;
  bool _visible = false;

public:
  SDLWindowWrapper(const char* name,
                   uint32_t width,
                   uint32_t height,
                   uint32_t framesPerSecond,
                   OpenGlEsVersion openGlEsVersion,
                   bool enableResize = true);
  virtual ~SDLWindowWrapper();

#ifdef __EMSCRIPTEN__

private:
  static void _webStep(void* data);

#endif

public:
  void run();
  void stop();

public:
  void update(uint32_t deltaTime);
  void render();

public:
  void setWindowPosition(int32_t x, int32_t y);
  void setWindowSize(uint32_t width, uint32_t height);
  void setMinimumWindowSize(uint32_t width, uint32_t height);
  void setMaximumWindowSize(uint32_t width, uint32_t height);

protected:
  virtual void _onEvent(const SDL_Event& event) = 0; // TODO: convert event?
  virtual void _onUpdate(uint32_t deltaTime) = 0;
  virtual void _onRender(const SDL_Window& screen) = 0; // TODO: remove window?
  virtual void _onResize(uint32_t width, uint32_t height) = 0;
  virtual void _onVisibilityChange(bool visible) = 0;

  //   // TODO
  //   // TODO
  //   // TODO

  // public:
  //   struct Input
  //   {
  //     struct Keyboard
  //     {
  //       std::unordered_map<int32_t, bool> keys;
  //     }
  //     keyboard;

  //     struct Mouse
  //     {
  //       glm::vec2 position = { 0, 0 };
  //       glm::vec2 delta = { 0, 0 };

  //       struct Buttons
  //       {
  //         bool left = false;
  //         bool middle = false;
  //         bool right = false;
  //       }
  //       buttons;

  //       // std::unordered_map<int32_t, bool> buttons;
  //       // std::unordered_map<int32_t, bool> prevButtons;
  //     }
  //     mouse;
  //   }
  //   _inputs;

  //   struct MouseEvent
  //   {
  //   };

  // protected:
  //   virtual void _onMouseEvent(const SDL_Event& event) = 0;
};

} // namespace graphics
} // namespace gero
