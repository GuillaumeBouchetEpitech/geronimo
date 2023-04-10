
#include "Scene.hpp"

#include "renderers/hud/helpers/renderTextBackground.hpp"
#include "renderers/hud/helpers/writeTime.hpp"
#include "renderers/hud/widgets/renderPerformanceProfilerMetrics.hpp"

#include "renderers/scene/helpers/renderPhysicBody.hpp"
#include "renderers/scene/helpers/renderPhysicVehicle.hpp"

#include "application/states/StateManager.hpp"

#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/math/constants.hpp"

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
    performanceProfiler.start("render scene");

    context.graphic.scene.deferred.startRecording();

    Scene::_renderScene();

    context.graphic.scene.deferred.stopRecording();

    context.graphic.scene.deferred.setEyePosition(context.graphic.camera.scene.getEye());
    context.graphic.scene.deferred.applySpotLights(context.graphic.camera.scene);

    performanceProfiler.stop("render scene");
  }

  {
    performanceProfiler.start("render hud");

    Scene::_renderHud();

    performanceProfiler.stop("render hud");
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

        helpers::renderPhysicBody(body->getShapeDefinition(), body->getPosition(), body->getOrientation());
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

        helpers::renderPhysicVehicle(*vehicle);
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
        gero::physics::Raycaster::RaycastParams params(inOrigin + glm::vec3(0, 0, +100),
                                                       inOrigin + glm::vec3(0, 0, -100));
        params.collisionGroup = -1;
        params.collisionMask = -1;
        params.radius = 0.0f;

        gero::physics::Raycaster::RaycastParams::ResultArray<1> result;
        context.physic.world->getRaycaster().raycast(params, result);

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

        graphic.scene.stackRenderers.safeMode([&context]() { context.physic.world->render(); });
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

      std::stringstream sstr;

      {

        // auto& timeDataMap = context.logic.performanceProfiler.getTimeDataMap();

        const auto& performanceProfiler = context.logic.performanceProfiler;
        const auto& allDataKeys = performanceProfiler.getAllDataKeys();
        for (const auto& keyName : allDataKeys) {
          if (const auto timeDataRef = performanceProfiler.tryGetTimeData(keyName)) {
            const auto& timeData = timeDataRef->get();

            const int32_t latestDuration = timeData.getLatestDuration();
            const int32_t averageDuration = timeData.getAverageDuration();

            sstr << keyName << ":";
            sstr << std::endl;

            writeTime(sstr, latestDuration, 0);
            sstr << std::endl;

            if (averageDuration > 0 && averageDuration < 999) {
              sstr << "~";
              writeTime(sstr, averageDuration, 0);
              sstr << std::endl;
            }
            sstr << std::endl;
          }
        }

        if (auto timeDataRef = performanceProfiler.tryGetTimeData("update frame")) {
          const auto& timeData = timeDataRef->get();

          const int32_t latestDuration = timeData.getLatestDuration();
          const int32_t averageDuration = timeData.getAverageDuration();
          const int32_t latestFpsValue = int32_t(1000.0f / float(latestDuration));

          sstr << "FPS (update):" << std::endl;
          sstr << latestFpsValue << "fps" << std::endl;

          if (averageDuration > 0) {
            const int32_t averageFpsValue = int32_t(1000.0f / float(averageDuration));
            if (averageFpsValue > 0 && averageFpsValue < 999) {
              sstr << "~" << averageFpsValue << "fps" << std::endl;
            }
          }
          sstr << std::endl;
        }

        if (auto timeDataRef = performanceProfiler.tryGetTimeData("render frame")) {
          const auto& timeData = timeDataRef->get();

          const int32_t latestDuration = timeData.getLatestDuration();
          const int32_t averageDuration = timeData.getAverageDuration();
          const int32_t latestFpsValue = int32_t(1000.0f / float(latestDuration));

          sstr << "FPS (render):" << std::endl;
          sstr << latestFpsValue << "fps" << std::endl;

          if (averageDuration > 0) {
            const int32_t averageFpsValue = int32_t(1000.0f / float(averageDuration));
            if (averageFpsValue > 0 && averageFpsValue < 999) {
              sstr << "~" << averageFpsValue << "fps" << std::endl;
            }
          }
          sstr << std::endl;
        }
      }

      const std::string str = sstr.str();

      const auto& vSize = graphic.camera.viewportSize;

      constexpr float k_textDepth = -0.5f;
      constexpr float k_scale = 16.0f;

      graphic.hud.textRenderer.setMainColor(glm::vec4(1, 1, 1, 1))
        .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
        .setScale(k_scale)
        .setDepth(k_textDepth)
        .setTextAlign(TextRenderer::TextAlign::left);

      graphic.hud.textRenderer.pushText(glm::vec2(k_scale * 0.5f, float(vSize.y) - k_scale * 1.5f), str);

      helpers::renderTextBackground(
        k_textDepth, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 3.0f, 6.0f);
    }

    graphic.hud.textRenderer.render();
  }

  {
    auto& stackRenderers = graphic.hud.stackRenderers;

    {
      auto& wireFrames = stackRenderers.getWireFramesStack();
      wireFrames.pushRectangle(glm::vec2(100, 20), glm::vec2(50, 20), glm::vec3(1, 1, 1));
    }

    {
      auto& triangles = stackRenderers.getTrianglesStack();
      triangles.pushCircle(glm::vec2(200, 30), 10.0f, glm::vec4(1, 1, 1, 0.5));
    }

    stackRenderers.flush();
  }

  {
    const glm::vec2 vSize = glm::vec2(graphic.camera.viewportSize);

    const glm::vec2 k_size = glm::vec2(150, 50);
    const glm::vec3 k_pos = glm::vec3(vSize.x - k_size.x - 10, vSize.y - k_size.y - 10, 0);

    widgets::renderPerformanceProfilerMetrics(k_pos, k_size, "complete frame");

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
