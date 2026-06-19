
#pragma once

// #include "floors/FloorQuad.hpp"
// #include "walls/WallQuad.hpp"
#include "floors/FloorsManager.hpp"
#include "walls/WallsManager.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

// forward declaration
class FloorBuilder;
class WallBuilder;

//
//
//

class BuildingBrickModel
{
  friend FloorBuilder;
  friend WallBuilder;

public:
  BuildingBrickModel() = default;
  ~BuildingBrickModel() = default;

// public:
//   bool collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const;
//   bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const;
//   bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const;
//   bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const FloorQuad*>& outQuads) const;
//   bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<const FloorQuad*>& outQuads) const;

// public:
//   bool collideWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const;
//   bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const WallQuad*>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<const WallQuad*>& outQuads) const;

public:
  // const std::vector<FloorQuad>& getFloorQuads() const { return this->_floorQuads; }
  const FloorsManager& getFloorsManager() const { return this->_floorsManager; }
  const WallsManager& getWallsManager() const { return this->_wallsManager; }
  // const std::vector<WallQuad>& getWallQuads() const { return this->_wallQuads; }

public:
  void render() const;

private:
  FloorsManager _floorsManager;
  WallsManager _wallsManager;
  // std::vector<FloorQuad> _floorQuads;
  // std::vector<WallQuad> _wallQuads;

};
