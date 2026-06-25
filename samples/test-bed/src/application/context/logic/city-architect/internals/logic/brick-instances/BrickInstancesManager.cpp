
#include "BrickInstancesManager.hpp"

BrickInstancesManager::BrickInstancesManager()
{
  this->_brickInstances.reserve(256);
}


BrickInstance& LiveBrickInstancesManager::addInstance(
  const glm::vec3& inPos,
  const glm::quat& inQuat,
  AbstractBrickModelWeakRef inRef
) {
  this->_brickInstances.emplace_back(inPos, inQuat, inRef);
  return this->_brickInstances.back();
}

