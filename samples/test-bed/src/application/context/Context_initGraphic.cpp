
#include "Context.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/Image.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/system/TraceLogger.hpp"

void Context::initializeGraphicResources() {

  D_MYLOG("MaxTextureSize: " << gero::graphics::GlContext::getMaxTextureSize());

  {
    const auto& vSize = graphic.camera.viewportSize;

    graphic.camera.scene.setPerspective(70.0f, 1.0f, 150.0f);
    graphic.camera.scene.setSize(vSize.x, vSize.y);

    graphic.camera.hud.setOrthographic(0.0f, float(vSize.x), 0.0f, float(vSize.y), -10.0f, +10.0f);
    graphic.camera.hud.setSize(vSize.x, vSize.y);

    const glm::vec3 eye = {0.0f, 0.0f, 1.0f};
    const glm::vec3 center = {0.0f, 0.0f, 0.0f};
    const glm::vec3 upAxis = {0.0f, 1.0f, 0.0f};
    graphic.camera.hud.lookAt(eye, center, upAxis);
  }

}
