
#pragma once

#include "BrickInstance.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

//
//
//

// forward declaration
class BrickInstanceBuilder;

//
//
//

// BrickInstancesManager -> BrickInstance -> AbstractBrickModel -> BrickInstancesManager...

class BrickInstancesManager
{
  friend BrickInstanceBuilder;

public:
  BrickInstancesManager();
  virtual ~BrickInstancesManager() = default;

// public:
//   bool collideWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const;
//   bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<std::size_t>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<std::size_t>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<const WallQuad*>& outQuads) const;
//   bool findWallQuads(const glm::vec3& inCenter, float inRadius, std::vector<const WallQuad*>& outQuads) const;

public:
  const std::vector<BrickInstance>& getBrickInstances() const { return this->_brickInstances; }

protected:
  std::vector<BrickInstance> _brickInstances;

};

class LiveBrickInstancesManager : public BrickInstancesManager
{
public:
  LiveBrickInstancesManager() = default;

public:
  BrickInstance& addInstance(
    const glm::vec3& inPos,
    const glm::quat& inQuat,
    AbstractBrickModelWeakRef inRef
  );

};
