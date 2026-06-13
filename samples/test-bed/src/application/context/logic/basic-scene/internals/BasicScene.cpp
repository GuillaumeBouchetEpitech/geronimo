
#include "BasicScene.hpp"

#include "helpers/renderPhysicBody.hpp"
#include "helpers/renderPhysicVehicle.hpp"

#include "application/context/Context.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/system/asValue.hpp"
#include "geronimo/system/easing/easingFunctions.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/math/constants.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/math/safe-normalize.hpp"
// #include "geronimo/system/rng/RandomNumberGenerator.hpp"
#include "geronimo/system/rng/DeterministicRng.hpp"

BasicScene::BasicScene() {

  physic.world = gero::physics::AbstractPhysicWorld::create();
  physic.world->setDebuggerPushLine([this](const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) {
    Context::get().graphic.renderer.getSceneRenderer().getStackRenderers().getWireFramesStack().pushLine(posA, posB, color);
  });

  this->initializePhysicResources();
  this->experimentalPhysicVehicle();
}

void BasicScene::update(float deltaTimeSec) {

  auto& performanceProfiler = Context::get().logic.performanceProfiler;

  performanceProfiler.start("1 update physic");

  constexpr uint32_t k_maxSubSteps = 3;
  constexpr float k_fixedStep = 1.0f / 60.0f;
  this->physic.world->step(deltaTimeSec, k_maxSubSteps, k_fixedStep);

  performanceProfiler.stop("1 update physic");

}

void BasicScene::render() {

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  if (Context::get().logic.debugMode) {

    scene.getStackRenderers().safeMode([this]() { this->physic.world->renderDebug(); });
  }

  {
    //
    // physic bodies
    //

    auto& physicBodyManager = this->physic.world->getPhysicBodyManager();
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
    // instance.color = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f);
    instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    instance.light = 0.5f;

    scene.getGeometriesStackRenderer().pushAlias(100, instance);
  }

  {
    //
    // physic vehicles
    //

    auto& geometriesStackRenderer = scene.getGeometriesStackRenderer();
    const auto& frustumCulling = scene.getCamera().getFrustumCulling();

    auto& vehicleManager = this->physic.world->getPhysicVehicleManager();
    for (std::size_t ii = 0; ii < vehicleManager.vehicleSize(); ++ii) {
      auto vehicle = vehicleManager.getVehicle(uint32_t(ii));

      renderPhysicVehicle(*vehicle);

      {
        const auto body = vehicle->getPhysicBody();

        const glm::mat3 rotMat3 = glm::mat3_cast(body->getOrientation());

        glm::vec3 lightPos1 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 3);
        glm::vec3 lightPos2 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 4);
        glm::vec3 lightPos3 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 5);

        if (frustumCulling.sphereInFrustum(lightPos3, 5)) {
          scene.getDeferred().pushSpotLight(lightPos3, glm::vec3(1), 10);
        }

        {
          const float radius = 0.5f;

          GeometriesStackRenderer::GeometryInstance instance;
          instance.position = lightPos3;
          instance.orientation = glm::quat(1, 0, 0, 0);
          instance.scale = glm::vec3(radius);
          instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
          instance.light = 0.0f;

          if (frustumCulling.sphereInFrustum(instance.position, radius)) {
            geometriesStackRenderer.pushAlias(1112, instance);
          }

          instance.scale = glm::vec3(radius * 0.5f);

          instance.position = lightPos1;
          if (frustumCulling.sphereInFrustum(instance.position, radius)) {
            geometriesStackRenderer.pushAlias(1112, instance);
          }

          instance.position = lightPos2;
          if (frustumCulling.sphereInFrustum(instance.position, radius)) {
            geometriesStackRenderer.pushAlias(1112, instance);
          }
        }
      }
    }
  }


  {

    auto& stackRenderers = scene.getStackRenderers();

    {

      // std::array<glm::vec3, 5> allPos = {{
      //   glm::vec3(10, 10, 10),
      //   glm::vec3(20, 20, 20),
      //   glm::vec3(10, 20, 30),
      //   glm::vec3(20, 10, 40),
      //   glm::vec3(0, 0, 50),
      // }};

      // for (std::size_t ii = 0; ii + 1 < allPos.size(); ++ii) {
      //   stackRenderers.getTrianglesStack().pushThickTriangle3dLine(
      //     allPos.at(ii + 0), allPos.at(ii + 1), 0.5f, glm::vec4(1, 1, 1, 1));
      // }

    }

    {
      auto& wireFrames = stackRenderers.getWireFramesStack();

      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(1000, 0, 0), glm::vec3(1, 0, 0));
      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0));
      wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), glm::vec3(0, 0, 1));

      const auto getGroundPos = [this](const glm::vec3& inOrigin) {
        gero::physics::RayCaster::RayCastParams params(inOrigin + glm::vec3(0, 0, +100),
                                                       inOrigin + glm::vec3(0, 0, -100));
        params.collisionGroup = -1;
        params.collisionMask = -1;
        params.radius = 0.0f;

        gero::physics::RayCaster::RayCastParams::ResultArray<1> result;
        this->physic.world->getRayCaster().rayCast(params, result);

        if (result.hasHit && result.allImpactsTotal > 0) {
          const auto& impact = result.allImpactsData.front();
          return impact.impactPoint.z;
        }
        return 0.0f;
      };

      auto& frustumCulling = camInstance.getFrustumCulling();

      std::array<glm::vec3, 7> k_colors = {{
        {1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f},
      }};

      for (int xx = -2; xx <= 1; ++xx)
        for (int yy = -2; yy <= 2; ++yy) {

          int currIndex = xx + yy;

          const float angle = context.logic.time * 1.0f + float(currIndex);
          const glm::vec3 dir = glm::vec3(gero::math::getDirection(angle), 0);

          glm::vec3 lightPos = {xx * 10, yy * 10, 7};
          lightPos += dir * 10.0f;
          lightPos.z = getGroundPos(lightPos) + 2;

          if (!frustumCulling.sphereInFrustum(lightPos, 5))
            continue;

          glm::vec3 lightColor = k_colors[std::size_t(currIndex) % k_colors.size()];
          // glm::vec3 lightColor = glm::vec3(0,0,0);
          // lightColor.x = (currIndex % 2) == 0;
          // lightColor.y = (currIndex % 3) == 0;
          // lightColor.z = (lightColor.x + lightColor.y) == 0.0f ? 1.0f : 0.0f;

          scene.getDeferred().pushSpotLight(lightPos, lightColor, 5);

          {
            const float radius = 0.5f;

            GeometriesStackRenderer::GeometryInstance instance;
            instance.position = lightPos;
            instance.orientation = glm::quat(1, 0, 0, 0);
            instance.scale = glm::vec3(radius);
            instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            instance.light = 0.0f;

            scene.getGeometriesStackRenderer().pushAlias(1112, instance);
          }

          // wireFrames.pushCross(lightPos, glm::vec3(1,1,1), 1.0f);

          // xx = 100; // TODO: debug
          // yy = 100; // TODO: debug
          // break; // TODO: debug
        }

      stackRenderers.flush();

      // if (context.logic.debugMode) {

      //   scene.getStackRenderers().safeMode([&context]() { context.physic.world->renderDebug(); });
      // }
    }
  }

}
