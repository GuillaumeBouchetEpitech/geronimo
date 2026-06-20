
#include "BrickModelsManager.hpp"

#include <functional>

void BrickModelsManager::render(const glm::mat4& transform) const
{
  this->_brickModelsPool.for_each([&transform](const AbstractBrickModel& currModel) {
    currModel.render(transform);
  });
}
