
#include "renderPhysicBody.hpp"

#include "application/context/Context.hpp"

namespace helpers {

void renderPhysicBody(const gero::physics::PhysicShapeDef& inShapeDef,
                      const glm::vec3& inPosition,
                      const glm::quat& inOrientation) {

  auto& graphic = Context::get().graphic;
  auto& geometriesStackRenderer = graphic.scene.geometriesStackRenderer;

  switch (inShapeDef.type) {
  case gero::physics::PhysicShapeDef::Type::sphere: {
    const float radius = inShapeDef.data.sphere.radius;

    GeometriesStackRenderer::GeometryInstance instance;
    instance.position = inPosition;
    instance.orientation = inOrientation;
    instance.scale = glm::vec3(radius);
    instance.color = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);

    geometriesStackRenderer.pushAlias(1111, instance);

    break;
  }
  case gero::physics::PhysicShapeDef::Type::box: {
    const glm::vec3& size = inShapeDef.data.box.size;

    GeometriesStackRenderer::GeometryInstance instance;
    instance.position = inPosition;
    instance.orientation = inOrientation;
    instance.scale = size;
    instance.color = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);

    geometriesStackRenderer.pushAlias(2222, instance);

    break;
  }
  case gero::physics::PhysicShapeDef::Type::capsule: {
    const float radius = inShapeDef.data.capsule.radius;
    const float height = inShapeDef.data.capsule.height;

    const glm::mat3 rotMat3 = glm::mat3_cast(inOrientation);

    {
      GeometriesStackRenderer::GeometryInstance instance;
      instance.position = inPosition;
      instance.orientation = inOrientation;
      instance.scale = glm::vec3(radius);
      instance.color = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f);

      instance.position += rotMat3 * glm::vec3(0, 0, height * +0.5f);
      geometriesStackRenderer.pushAlias(1111, instance);

      instance.position += rotMat3 * glm::vec3(0, 0, height * -1.0f);
      geometriesStackRenderer.pushAlias(1111, instance);
    }

    {
      GeometriesStackRenderer::GeometryInstance instance;
      instance.position = inPosition, instance.orientation = inOrientation;
      instance.scale = glm::vec3(radius, radius, height);
      instance.color = glm::vec4(1.0f, 0.6f, 1.0f, 1.0f);

      geometriesStackRenderer.pushAlias(4444, instance);
    }

    break;
  }
  case gero::physics::PhysicShapeDef::Type::compound: {
    const auto& childrendShapes = inShapeDef.data.compound.childShapes;

    const glm::mat3 rotMat3 = glm::mat3_cast(inOrientation);

    for (const auto& currChild : childrendShapes) {
      const glm::vec3 childPosition = currChild.transform[3];
      const glm::quat childOrientation = glm::quat_cast(currChild.transform);

      renderPhysicBody(*currChild.shape, inPosition + rotMat3 * childPosition, inOrientation * childOrientation);
    }

    break;
  }
  case gero::physics::PhysicShapeDef::Type::staticMesh:
    break;
  }
}

} // namespace helpers
