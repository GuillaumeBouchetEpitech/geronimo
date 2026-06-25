
#pragma once

#include "floors/FloorsManager.hpp"
#include "walls/WallsManager.hpp"
#include "brick-instances/BrickInstancesManager.hpp"

#include "utilities/GenericAABB.hpp"

#include "../graphics/instanced-brick-models/WireFramesAccumulator.hpp"
#include "../graphics/instanced-brick-models/TrianglesAccumulator.hpp"

#include "AbstractBrickModel.hpp"

#include "geronimo/system/math/RayCastResult.hpp"

#include <string>

class BrickModel : public AbstractBrickModel
{
public:
  BrickModel(const std::string& inName);
  ~BrickModel() = default;

public:
  const FloorsManager& getFloorsManager() const override { return this->_floorsManager; }
  const WallsManager& getWallsManager() const override { return this->_wallsManager; }
  const BrickInstancesManager& getBrickInstancesManager() const override { return this->_brickInstancesManager; }

public:
  // bool collideFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize) const override;
  bool findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<FloorQuad>& outQuads) const override;
  bool findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<FloorQuad>& outQuads) const override;

private:
  void _findFloorQuads(
    const glm::mat4& inTransform,
    const BrickInstance& inInstance,
    const glm::vec3& inCenter,
    const glm::vec3& inSize,
    std::vector<FloorQuad>& outQuads
  ) const;

public:
  void computeAABB(const glm::mat4& inTransform) const override;

  // bool intersect(
  //   const glm::mat4& inTransform,
  //   const glm::vec3& inRayFrom,
  //   const glm::vec3& inRayTo,
  //   gero::math::RayCastResult& outData
  // ) const override;

private:
  FloorsManager& getFloorsManager() override { return this->_floorsManager; }
  WallsManager& getWallsManager() override { return this->_wallsManager; }
  BrickInstancesManager& getBrickInstancesManager() override { return this->_brickInstancesManager; }
  const std::string& getName() const override {return _name; }
  const GenericAABB& getAABB() const override { return this->_aabb; }

public:
  void buildVertices(
    IWireFramesAccumulator& inWireFrames,
    ITrianglesAccumulator& inTriangles
  ) const override;
  void buildInstances(
    IWireFramesAccumulator& inWireFrames,
    ITrianglesAccumulator& inTriangles,
    InstancedBrickModels& inInstancedBrickModels
  ) const override;
  void pushNewInstances(
    const glm::vec3& inOrigin,
    const glm::quat& inQuat,
    InstancedBrickModels& inInstancedBrickModels
  ) const override;

private:
  std::string _name;
  FloorsManager _floorsManager;
  WallsManager _wallsManager;
  BrickInstancesManager _brickInstancesManager;

  mutable GenericAABB _aabb;

};
