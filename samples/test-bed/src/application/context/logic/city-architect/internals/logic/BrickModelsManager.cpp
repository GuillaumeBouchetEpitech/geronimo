
#include "BrickModelsManager.hpp"

#include <functional>

ExpectBrickRef BrickModelsManager::createNewBrick(const std::string& inName)
{
  if (this->_brickModelsMap.count(inName)) {
    return std::unexpected(BrickCreateError::duplicated_name);
  }

  AbstractBrickModelWeakRef currModelRef = this->_brickModelsPool.acquire(inName);
  this->_brickModelsMap[inName] = currModelRef;
  return currModelRef;
}

MaybeBrickRef BrickModelsManager::getBrickByName(const std::string& inName)
{
  auto it = this->_brickModelsMap.find(inName);
  if (it == this->_brickModelsMap.end()) {
    return std::nullopt;
  }
  return it->second;
}

const MaybeBrickRef BrickModelsManager::getBrickByName(const std::string& inName) const {
  auto it = this->_brickModelsMap.find(inName);
  if (it == this->_brickModelsMap.end()) {
    return std::nullopt;
  }
  return it->second;
}

// void BrickModelsManager::render(const glm::mat4& transform) const
// {
//   this->_brickModelsPool.for_each([&transform](const AbstractBrickModel& currModel) {
//     currModel.render(transform);
//   });
// }
