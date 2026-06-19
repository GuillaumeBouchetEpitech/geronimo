
#include "WallsManager.hpp"

WallsManager::WallsManager()
{
  this->_wallQuads.reserve(256);
}

bool WallsManager::collideWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const
{
  for (std::size_t ii = 0; ii < this->_wallQuads.size(); ++ii) {
    if (this->_wallQuads.at(ii).isColliding(inCenter, inSize)) {
      return true;
    }
  }

  return false;
}

bool WallsManager::findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const
{
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_wallQuads.size(); ++ii) {
    if (this->_wallQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(ii);
    }
  }

  return !outQuads.empty();
}

bool WallsManager::findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const
{
  return findWallQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

bool WallsManager::findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const WallQuad*>& outQuads) const {
  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_wallQuads.size(); ++ii) {
    if (this->_wallQuads.at(ii).isColliding(inCenter, inSize)) {
      outQuads.push_back(&this->_wallQuads.at(ii));
    }
  }

  return !outQuads.empty();
}

bool WallsManager::findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<const WallQuad*>& outQuads) const {
  return findWallQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}
