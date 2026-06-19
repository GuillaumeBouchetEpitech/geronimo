
#pragma once

// #include "floors/FloorQuad.hpp"
// #include "floors/FloorsManager.hpp"
#include "WallQuad.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
// #include <optional>
// #include <expected>

//
//
//

// forward declaration
class FloorBuilder;
class WallBuilder;

//
//
//

class WallsManager
{
  friend FloorBuilder;
  friend WallBuilder;

public:
  WallsManager();
  ~WallsManager() = default;

public:
  bool collideWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const;
  bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const;
  bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const;
  bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const WallQuad*>& outQuads) const;
  bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<const WallQuad*>& outQuads) const;

public:
  const std::vector<WallQuad>& getWallQuads() const { return this->_wallQuads; }

private:
  std::vector<WallQuad> _wallQuads;

};
