
#include "TouchManager.hpp"

#include "geronimo/system/ErrorHandler.hpp"

TouchManager* TouchManager::_instance = nullptr;

void TouchManager::create() {
  if (_instance)
    D_THROW(std::runtime_error, "already created");

  _instance = new TouchManager();
}

void TouchManager::destroy() {
  if (!_instance)
    D_THROW(std::runtime_error, "no allocated");

  delete _instance, _instance = nullptr;
}

TouchManager& TouchManager::get() {
  if (!_instance)
    D_THROW(std::runtime_error, "no allocated");

  return *_instance;
}

//

void TouchManager::updateAsTouchedDown(int32_t inFingerId, const glm::vec2& inPos)
{
  TouchData newData;
  newData.fingerId = inFingerId;
  newData.position = inPos;
  newData.delta = {0, 0};
  _touchDataMap[inFingerId] = newData;
}
void TouchManager::updateAsTouchedUp(int32_t inFingerId)
{
  _touchDataMap.erase(inFingerId);
}
void TouchManager::updateAsTouchedMotion(int32_t inFingerId, const glm::vec2& inPos, const glm::vec2& inDelta)
{
  auto it = _touchDataMap.find(inFingerId);
  if (it == _touchDataMap.end())
    return;

  TouchData& existingData = it->second;
  existingData.position = inPos;
  existingData.delta = inDelta;
}

//

TouchManager::MaybeTouchDataRef TouchManager::getTouchData(int32_t inFingerId)
{
  auto it = _touchDataMap.find(inFingerId);
  if (it == _touchDataMap.end())
    return {};
  return it->second;
}

//

void TouchManager::reset() {
  _touchDataMap.clear();
}

