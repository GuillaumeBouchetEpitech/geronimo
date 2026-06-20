
#include "BrickInstanceBuilder.hpp"

BrickInstanceBuilder::ExpectBrickInstanceRef BrickInstanceBuilder::addInstance(
  const glm::vec3& inPos,
  const glm::quat& inQuat,
  AbstractBrickModelWeakRef inRef
) {

  _model->getBrickInstancesManager()._brickInstances.emplace_back(inPos, inQuat, inRef);

  return _model->getBrickInstancesManager()._brickInstances.back();
}
