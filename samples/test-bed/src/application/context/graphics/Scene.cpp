
#include "Scene.hpp"

// #include "renderers/hud/helpers/renderTextBackground.hpp"
// #include "renderers/hud/helpers/writeTime.hpp"
// #include "renderers/hud/widgets/renderPerformanceProfilerMetrics.hpp"

#include "renderers/scene/helpers/renderPhysicBody.hpp"
#include "renderers/scene/helpers/renderPhysicVehicle.hpp"

#include "application/states/StateManager.hpp"

#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/advanced-concept/widgets/helpers/renderTextBackground.hpp"
#include "geronimo/graphics/advanced-concept/widgets/helpers/writeTime.hpp"
#include "geronimo/graphics/advanced-concept/widgets/renderPerformanceProfilerMetrics.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/math/constants.hpp"

#include <iomanip>

using namespace gero::graphics;
using namespace gero::graphics::GlContext;

void Scene::initialize() {
  GlContext::enable(States::depthTest);
  GlContext::setDepthFunc(DepthFuncs::less);

  // GlContext::enable(States::blend);
  // GlContext::setBlendFunc(BlendFuncs::srcAlpha, BlendFuncs::oneMinusSrcAlpha);
  GlContext::disable(States::blend);

  GlContext::enable(States::cullFace);
}

void Scene::renderAll() {
  Scene::_clear();

  auto& context = Context::get();
  auto& performanceProfiler = context.logic.performanceProfiler;

  {
    performanceProfiler.start("2 render scene");

    context.graphic.scene.deferred.startRecording();

    Scene::_renderScene();

    context.graphic.scene.deferred.stopRecording();

    context.graphic.scene.deferred.setEyePosition(context.graphic.camera.scene.getEye());
    context.graphic.scene.deferred.applySpotLights(context.graphic.camera.scene);

    performanceProfiler.stop("2 render scene");
  }

  {
    performanceProfiler.start("2 render hud");

    Scene::_renderHud();

    performanceProfiler.stop("2 render hud");
  }
}

void Scene::_renderScene() {
  GlContext::enable(States::cullFace);

  auto& context = Context::get();
  auto& graphic = context.graphic;
  auto& camera = graphic.camera;
  gero::graphics::Camera& camInstance = camera.scene;

  const auto& matricesData = camInstance.getMatricesData();

  graphic.scene.stackRenderers.setMatricesData(matricesData);
  graphic.scene.geometriesStackRenderer.setMatricesData(matricesData);

  {

    {
      //
      // physic bodies
      //

      auto& physicBodyManager = context.physic.world->getPhysicBodyManager();
      for (std::size_t ii = 0; ii < physicBodyManager.size(); ++ii) {
        auto body = physicBodyManager.getBody(uint32_t(ii));

        renderPhysicBody(body->getShapeDefinition(), body->getPosition(), body->getOrientation());
      }
    }

    {
      //
      // heightfield
      //

      GeometriesStackRenderer::GeometryInstance instance;
      instance.position = glm::vec3(0, 0, 0);
      instance.orientation = glm::quat(1, 0, 0, 0);
      instance.scale = glm::vec3(1.0f, 1.0f, 1.0f);
      instance.color = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f);
      instance.light = 0.5f;

      graphic.scene.geometriesStackRenderer.pushAlias(100, instance);
    }

    {
      //
      // physic vehicles
      //

      auto& vehicleManager = context.physic.world->getPhysicVehicleManager();
      for (std::size_t ii = 0; ii < vehicleManager.vehicleSize(); ++ii) {
        auto vehicle = vehicleManager.getVehicle(uint32_t(ii));

        renderPhysicVehicle(*vehicle);
      }
    }

    graphic.scene.geometriesStackRenderer.renderAll();
  }

  {

    auto& stackRenderers = graphic.scene.stackRenderers;

    {
      auto& wireFrames = stackRenderers.getWireFramesStack();

      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(1000, 0, 0), glm::vec3(1, 0, 0));
      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0));
      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), glm::vec3(0, 0, 1));

      const auto getGroundPos = [&context](const glm::vec3& inOrigin) {
        gero::physics::RayCaster::RayCastParams params(inOrigin + glm::vec3(0, 0, +100),
                                                       inOrigin + glm::vec3(0, 0, -100));
        params.collisionGroup = -1;
        params.collisionMask = -1;
        params.radius = 0.0f;

        gero::physics::RayCaster::RayCastParams::ResultArray<1> result;
        context.physic.world->getRayCaster().rayCast(params, result);

        if (result.hasHit && result.allImpactsTotal > 0) {
          const auto& impact = result.allImpactsData.front();
          return impact.impactPoint.z;
        }
        return 0.0f;
      };

      auto& frustumCulling = context.graphic.camera.scene.getFrustumCulling();

      for (int xx = -2; xx <= 1; ++xx)
        for (int yy = -2; yy <= 2; ++yy) {

          const float angle = context.logic.time * 1.0f + float(xx + yy);
          const glm::vec3 dir = glm::vec3(gero::math::getDirection(angle), 0);

          glm::vec3 lightPos = {xx * 10, yy * 10, 7};
          lightPos += dir * 10.0f;
          lightPos.z = getGroundPos(lightPos) + 2;

          if (!frustumCulling.sphereInFrustum(lightPos, 5))
            continue;

          context.graphic.scene.deferred.pushSpotLight(lightPos, 5);

          {
            const float radius = 0.5f;

            GeometriesStackRenderer::GeometryInstance instance;
            instance.position = lightPos;
            instance.orientation = glm::quat(1, 0, 0, 0);
            instance.scale = glm::vec3(radius);
            instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            instance.light = 0.0f;

            Context::get().graphic.scene.geometriesStackRenderer.pushAlias(1112, instance);
          }

          // wireFrames.pushCross(lightPos, glm::vec3(1,1,1), 1.0f);

          // xx = 100; // TODO: debug
          // yy = 100; // TODO: debug
          // break; // TODO: debug
        }

      stackRenderers.flush();

      if (context.logic.debugMode) {

        graphic.scene.stackRenderers.safeMode([&context]() { context.physic.world->renderDebug(); });
      }
    }
  }

  gero::graphics::ShaderProgram::unbind();
}

void Scene::_renderHud() {
  GlContext::clears(Buffers::depth);
  GlContext::disable(States::cullFace);

  auto& context = Context::get();
  auto& graphic = context.graphic;
  auto& camera = graphic.camera;
  gero::graphics::Camera& camInstance = camera.hud;

  const auto& matricesData = camInstance.getMatricesData();

  graphic.hud.stackRenderers.setMatricesData(matricesData);
  graphic.hud.textRenderer.setMatricesData(matricesData);

  context.graphic.scene.deferred.setAmbiantLightCoef(0.1f);
  context.graphic.scene.deferred.setSunLightDirection(glm::vec3(-1.0f, -1.0f, -2.0f));
  // context.graphic.scene.deferred.renderHudQuad(matricesData.composed);
  context.graphic.scene.deferred.renderHudQuad(camera.scene, camera.hud);

  GlContext::clears(Buffers::depth);

  {

    // {
    //   constexpr float k_textDepth = -0.5f;

    //   graphic.hud.textRenderer.pushText(
    //     glm::vec2(100, 100),
    //     "test",
    //     glm::vec4(1,1,1,1),
    //     80.0f,
    //     k_textDepth,
    //     glm::vec4(0.3f,0.3f,0.3f,1),
    //     TextRenderer::TextAlign::left
    //     );

    //   helpers::renderTextBackground(
    //     k_textDepth,
    //     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
    //     glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
    //     3.0f,
    //     6.0f
    //   );
    // }

    {

      auto& textRenderer = graphic.hud.textRenderer;

      const auto& timeDataMap = context.logic.performanceProfiler.getHistoricalTimeDataMap();

      const gero::graphics::TextRenderer::State state0 = {glm::vec4(1.0f,1.0f,1.0f,1), glm::vec4(0,0,0,1)};
      const gero::graphics::TextRenderer::State state1 = {glm::vec4(1.0f,1.0f,0.5f,1), glm::vec4(0,0,0,1)};
      const gero::graphics::TextRenderer::State state2 = {glm::vec4(0.5f,1.0f,0.5f,1), glm::vec4(0,0,0,1)};
      const gero::graphics::TextRenderer::State state3 = {glm::vec4(1.0f,0.5f,0.5f,1), glm::vec4(0,0,0,1)};

      std::stringstream sstr;
      for (const auto& timeData : timeDataMap) {

        if (timeData.first[0] != '1')
          continue;

        const int32_t avgIndex = timeData.second.getAverageDuration() > 0 ? 1 : 0;
        const int32_t minIndex = timeData.second.getMinDuration() > 0 ? 2 : 0;
        const int32_t maxIndex = timeData.second.getMaxDuration() > 0 ? 3 : 0;

        sstr << "${0}" << std::left << std::setw(15) << timeData.first;
        sstr << " " << std::setw(2) << timeData.second.getLatestDuration();
        sstr << " ${" << avgIndex << "}" << std::setw(2) << timeData.second.getAverageDuration();
        sstr << " ${" << minIndex << "}" << std::setw(2) << timeData.second.getMinDuration();
        sstr << " ${" << maxIndex << "}" << std::setw(2) << timeData.second.getMaxDuration();
        sstr << std::endl;
      }

      sstr << std::endl;
      sstr << std::endl;

      for (const auto& timeData : timeDataMap) {

        if (timeData.first[0] != '2')
          continue;

        const int32_t avgIndex = timeData.second.getAverageDuration() > 0 ? 1 : 0;
        const int32_t minIndex = timeData.second.getMinDuration() > 0 ? 2 : 0;
        const int32_t maxIndex = timeData.second.getMaxDuration() > 0 ? 3 : 0;

        sstr << "${0}" << std::left << std::setw(15) << timeData.first;
        sstr << " " << std::setw(2) << timeData.second.getLatestDuration();
        sstr << " ${" << avgIndex << "}" << std::setw(2) << timeData.second.getAverageDuration();
        sstr << " ${" << minIndex << "}" << std::setw(2) << timeData.second.getMinDuration();
        sstr << " ${" << maxIndex << "}" << std::setw(2) << timeData.second.getMaxDuration();
        sstr << std::endl;
      }

      const glm::vec2 vSize = glm::vec2(graphic.camera.viewportSize);

      const glm::vec2 textPos = glm::vec2(5, vSize.y - 5);
      const std::string str = sstr.str();

      textRenderer.setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left);
      textRenderer.setVerticalTextAlign(TextRenderer::VerticalTextAlign::top);

      textRenderer.pushText(textPos, str,
        state0,
        state1,
        state2,
        state3
        );

    }

    graphic.hud.textRenderer.render();
  }


  {

#if 0
    {

      const glm::vec2 k_pos = glm::vec2(400,200);
      const glm::vec2 k_size = glm::vec2(300,200);


      {

        auto& stackRenderers = graphic.hud.stackRenderers;
        auto& wireFrames = stackRenderers.getWireFramesStack();
        wireFrames.pushRectangle(k_pos, k_size, glm::vec3(1, 1, 1));
        wireFrames.pushRectangle(glm::vec2(k_pos.x + k_size.x * 0.5f, k_pos.y), glm::vec2(0.1f, k_size.y), glm::vec3(1, 1, 1));
        wireFrames.pushRectangle(glm::vec2(k_pos.x, k_pos.y + k_size.y * 0.5f), glm::vec2(k_size.y, 0.1f), glm::vec3(1, 1, 1));
        wireFrames.pushRectangle(k_pos - glm::vec2(10), glm::vec2(20), glm::vec3(1, 0, 0));
        stackRenderers.flush();
      }

      {

        // const auto& vSize = graphic.camera.viewportSize;

        constexpr float k_textDepth = -0.5f;
        constexpr float k_scale = 16.0f;

        graphic.hud.textRenderer.setMainColor(glm::vec4(1, 1, 1, 1))
          .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
          .setScale(k_scale)
          .setDepth(k_textDepth)
          .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
          .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
          ;

        {

          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x, k_pos.y + k_size.y), "T-L\nT-L\nT-L");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::right)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x + k_size.x, k_pos.y + k_size.y), "T-R\nT-R\nT-R");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::bottom)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x, k_pos.y), "B-L\nB-L\nB-L");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::bottom)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::right)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x + k_size.x, k_pos.y), "B-R\nB-R\nB-R");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::center)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x, k_pos.y + k_size.y * 0.5f), "C-L\nC-L\nC-L");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::center)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::right)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x + k_size.x, k_pos.y + k_size.y * 0.5f), "C-R\nC-R\nC-R");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::center)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x + k_size.x * 0.5f, k_pos.y + k_size.y), "T-C\nT-C\nT-C");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::bottom)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::center)
            ;

          graphic.hud.textRenderer.pushText(glm::vec2(k_pos.x + k_size.x * 0.5f, k_pos.y), "B-C\nB-C\nB-C");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        {
          graphic.hud.textRenderer
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::center)
            .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::center)
            ;

          graphic.hud.textRenderer.pushText(k_pos + k_size * 0.5f, "C-C\nC-C\nC-C");

          gero::graphics::helpers::renderTextBackground(k_textDepth,
                                                        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                                        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                                        3.0f,
                                                        6.0f,
                                                        graphic.hud.stackRenderers,
                                                        graphic.hud.textRenderer);
        }

        graphic.hud.textRenderer.render();
        graphic.hud.stackRenderers.flush();

      }

    }
#endif

  }

  {
    auto& stackRenderers = graphic.hud.stackRenderers;

    {
      auto& wireFrames = stackRenderers.getWireFramesStack();
      wireFrames.pushRectangle(glm::vec2(100, 20), glm::vec2(50, 20), glm::vec3(1, 1, 1));
    }

    {
      auto& triangles = stackRenderers.getTrianglesStack();

      // triangles.pushCircle(glm::vec2(200, 550), 10.0f, glm::vec4(1, 1, 1, 0.5));

      const glm::vec2 vSize = glm::vec2(graphic.camera.viewportSize);
      const glm::vec3 k_pos = glm::vec3(vSize.x - 15, vSize.y * 0.5f, 0);
      triangles.pushCircle(k_pos, 10.0f, glm::vec4(1, 1, 1, 0.5));

    }

    stackRenderers.flush();
  }

  const auto& performanceProfiler = context.logic.performanceProfiler;
  if (auto timeDataRef = performanceProfiler.tryGetTimeData("FRAME")) {
    const auto& timeData = timeDataRef->get();

    const glm::vec2 vSize = glm::vec2(graphic.camera.viewportSize);

    const glm::vec2 k_size = glm::vec2(150, 50);
    const glm::vec3 k_pos = glm::vec3(vSize.x - k_size.x - 120, vSize.y - k_size.y - 10 - 30, 0);

    gero::graphics::widgets::renderHistoricalTimeData(
      k_pos, k_size, true, timeData, graphic.hud.stackRenderers, graphic.hud.textRenderer);

    graphic.hud.stackRenderers.flush();
    graphic.hud.textRenderer.render();
  }

  gero::graphics::ShaderProgram::unbind();
}

void Scene::updateMatrices() {

  auto& context = Context::get();
  auto& graphic = context.graphic;
  auto& camera = graphic.camera;

  camera.scene.setSize(camera.viewportSize.x, camera.viewportSize.y);
  camera.scene.computeMatrices();
  camera.hud.setSize(camera.viewportSize.x, camera.viewportSize.y);
  camera.hud.computeMatrices();
}

void Scene::_clear() {
  const auto& viewportSize = Context::get().graphic.camera.viewportSize;
  const glm::uvec2 uVSize(viewportSize);

  GlContext::setViewport(0, 0, uVSize.x, uVSize.y);

  GlContext::clearColor(0.3f, 0.3f, 0.3f, 1.0f);
  GlContext::clearDepth(1.0f);
  GlContext::clears(Buffers::color, Buffers::depth);
}
