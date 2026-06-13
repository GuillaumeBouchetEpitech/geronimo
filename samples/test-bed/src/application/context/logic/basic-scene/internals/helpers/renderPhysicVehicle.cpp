
#include "renderPhysicVehicle.hpp"

#include "renderPhysicBody.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/math/constants.hpp"

void renderPhysicVehicle(const gero::physics::AbstractPhysicVehicle& inVehicle) {

  auto& context = Context::get();
  auto& scene = context.graphic.renderer.getSceneRenderer();
  auto& geometriesStackRenderer = scene.getGeometriesStackRenderer();
  const auto& frustumCulling = scene.getCamera().getFrustumCulling();

  {
    const auto body = inVehicle.getPhysicBody();

    renderPhysicBody(body->getShapeDefinition(), body->getPosition(), body->getOrientation());
  }

  // {
  //   const auto body = inVehicle.getPhysicBody();

  //   const glm::mat3 rotMat3 = glm::mat3_cast(body->getOrientation());

  //   glm::vec3 lightPos1 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 3);
  //   glm::vec3 lightPos2 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 4);
  //   glm::vec3 lightPos3 = body->getPosition() + rotMat3 * glm::vec3(0, 0, 5);

  //   if (frustumCulling.sphereInFrustum(lightPos3, 5)) {
  //     scene.getDeferred().pushSpotLight(lightPos3, 10);
  //   }

  //   {
  //     const float radius = 0.5f;

  //     GeometriesStackRenderer::GeometryInstance instance;
  //     instance.position = lightPos3;
  //     instance.orientation = glm::quat(1, 0, 0, 0);
  //     instance.scale = glm::vec3(radius);
  //     instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  //     instance.light = 0.0f;

  //     if (frustumCulling.sphereInFrustum(instance.position, radius)) {
  //       geometriesStackRenderer.pushAlias(1112, instance);
  //     }

  //     instance.scale = glm::vec3(radius * 0.5f);

  //     instance.position = lightPos1;
  //     if (frustumCulling.sphereInFrustum(instance.position, radius)) {
  //       geometriesStackRenderer.pushAlias(1112, instance);
  //     }

  //     instance.position = lightPos2;
  //     if (frustumCulling.sphereInFrustum(instance.position, radius)) {
  //       geometriesStackRenderer.pushAlias(1112, instance);
  //     }
  //   }
  // }

  {

    GeometriesStackRenderer::GeometryInstance instance;
    instance.scale = glm::vec3(1, 1, 1);
    instance.color = glm::vec4(1, 1, 1, 1);
    instance.light = 1.0f;

    for (int32_t wheelIndex = 0; wheelIndex < inVehicle.getNumWheels(); ++wheelIndex) {

      instance.position = inVehicle.getWheelPosition(wheelIndex);

      if (!frustumCulling.sphereInFrustum(instance.position, 2.0f))
        continue;

      instance.orientation = inVehicle.getWheelOrientation(wheelIndex);
      instance.orientation = glm::rotate(instance.orientation, gero::math::hpi, glm::vec3(0, 1, 0));

      geometriesStackRenderer.pushAlias(3333, instance);
    }
  }
}
