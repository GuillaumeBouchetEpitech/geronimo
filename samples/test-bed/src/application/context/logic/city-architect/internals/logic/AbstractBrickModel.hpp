
#pragma once

#include "floors/FloorsManager.hpp"
#include "walls/WallsManager.hpp"
// #include "brick-instances/BrickInstancesManager.hpp"

#include "../graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

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
class BrickInstanceBuilder;
class BrickInstancesManager;

//
//
//

class AbstractBrickModel
{
  friend FloorBuilder;
  friend WallBuilder;
  friend BrickInstanceBuilder;

public:
  virtual ~AbstractBrickModel() = default;

public:
  virtual const FloorsManager& getFloorsManager() const = 0;
  virtual const WallsManager& getWallsManager() const = 0;
  virtual const BrickInstancesManager& getBrickInstancesManager() const = 0;
  virtual const std::string& getName() const = 0;

public:
  // virtual bool collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const = 0;
  virtual bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<FloorQuad>& outQuads) const = 0;
  virtual bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<FloorQuad>& outQuads) const = 0;

protected:
  virtual FloorsManager& getFloorsManager() = 0;
  virtual WallsManager& getWallsManager() = 0;
  virtual BrickInstancesManager& getBrickInstancesManager() = 0;

public:
  virtual void buildVertices(IWireFramesStackRenderer& inWireFrames, ITrianglesAccumulator& inTriangles) const = 0;
  virtual void buildInstances(const glm::vec3& inOrigin, const glm::quat& inQuat, InstancedBrickModels& inInstancedBrickModels) const = 0;
  virtual void render(const glm::mat4& transform) const = 0;

};

using AbstractBrickModelWeakRef = gero::data_pool_weak_ref<AbstractBrickModel>;

