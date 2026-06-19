
#include "FloorsManager.hpp"

FloorsManager::FloorsManager()
{
  this->_floorQuads.reserve(256);
}

bool FloorsManager::collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      return true;
    }
  }

  return false;
}

bool FloorsManager::findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const
{
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(ii);
    }
  }

  return !outQuads.empty();
}

bool FloorsManager::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const
{
  return findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

bool FloorsManager::findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const FloorQuad*>& outQuads) const {
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_floorQuads.size(); ++ii) {
    if (this->_floorQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(&this->_floorQuads.at(ii));
    }
  }

  return !outQuads.empty();
}

bool FloorsManager::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<const FloorQuad*>& outQuads) const {
  return findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

