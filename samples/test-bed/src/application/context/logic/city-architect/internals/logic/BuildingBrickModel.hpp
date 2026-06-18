
#pragma once

#include "FloorQuad.hpp"
#include "WallQuad.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

// forward declaration
class FloorBuilder;

//
//
//

class BuildingBrickModel
{
  friend FloorBuilder;

public:
  BuildingBrickModel();
  ~BuildingBrickModel() = default;

public:
  bool collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const;
  bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const;
  bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const;
  bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const FloorQuad*>& outQuads) const;
  bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<const FloorQuad*>& outQuads) const;

public:
  const std::vector<FloorQuad>& getFloorQuads() const { return this->_floorQuads; }

public:
  void render() const;

private:
  std::vector<FloorQuad> _floorQuads;

public:
  std::vector<WallQuad> _wallQuads;

};
