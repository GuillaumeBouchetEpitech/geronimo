
#include "ArtificialStupiditySimImpl.hpp"

#include "application/context/Context.hpp"

#include "geronimo/graphics/input-managers/KeyboardManager.hpp"

ArtificialStupiditySimImpl::ArtificialStupiditySimImpl()
  : _platformerDuel(this->_keyframesManager)
{
}

void ArtificialStupiditySimImpl::update(float deltaTimeSec) {

  this->_platformerDuel.update(deltaTimeSec);

}

void ArtificialStupiditySimImpl::renderScene()
{
  const glm::vec3 k_origin = glm::vec3(5,50,0);

  {
    // grey background
    auto& context = Context::get();

    auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

    // auto& wireFramesStack = stackRenderer.getWireFramesStack();
    auto& trianglesStack = stackRenderer.getTrianglesStack();

    const glm::vec2 k_size = glm::vec2(40,20);
    trianglesStack.pushQuad(glm::vec2(k_origin) + k_size * 0.45f, k_size * 1.05f, glm::vec4(0.3f,0.3f,0.3f, 1), k_origin.z - 1.0f);
  }

  this->_keyframesManager.renderSpriteSheet(k_origin);

  std::array<std::string, 12> allKeyFramesKeys = {{
    "1000",
    "1001",
    "1011",
    "1002",
    "1003",
    "2000",
    "2100",
    "2101",
    "2001",
    "2002",
    "2003",
    "3000"
  }};

  for (std::size_t ii = 0; ii < allKeyFramesKeys.size(); ++ii)
  {
    const std::string& currAlias = allKeyFramesKeys.at(ii);

    this->_keyframesManager.renderFrame(
      currAlias,
      k_origin + glm::vec3(2.0f + 2.4f * float(ii),12+0,2),
      glm::vec3(1.0f,1.0f,0.5f), // <- color
      6.0f, // <- scale
      false, // <- flipX
      true // <- debugMode
    );

    // flipped on the X axis
    this->_keyframesManager.renderFrame(
      currAlias,
      k_origin + glm::vec3(2.0f + 2.4f * float(ii),12+2,2),
      glm::vec3(0.5f,1.0f,1.0f), // <- color
      6.0f, // <- scale
      true, // <- flipX
      true // <- debugMode
    );
  }

  this->_platformerDuel.renderScene();

}

void ArtificialStupiditySimImpl::renderHUD()
{
  this->_platformerDuel.renderHUD();
}
