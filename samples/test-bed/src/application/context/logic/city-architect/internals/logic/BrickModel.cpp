
#include "BrickModel.hpp"

#include "application/context/Context.hpp"

// #include "geronimo/system/TraceLogger.hpp"


void BrickModel::buildVertices(IWireFramesStackRenderer& inWireFrames) const
{
  // axis
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    currQuad.buildVertices(inWireFrames);
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    currQuad.buildVertices(inWireFrames);
  }

}

//MARK:render
void BrickModel::render(const glm::mat4& transform) const
{

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  // auto& wireFrames = stackRenderers.getWireFramesStack();

  {
    auto& wireFrames = stackRenderers.getWireFramesStack();

    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

    stackRenderers.flush();
  }

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    currQuad.render();
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    currQuad.render();
  }

  stackRenderers.flush();

}

