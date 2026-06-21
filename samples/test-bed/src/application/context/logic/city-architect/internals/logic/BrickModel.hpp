
#pragma once

#include "floors/FloorsManager.hpp"
#include "walls/WallsManager.hpp"
#include "brick-instances/BrickInstancesManager.hpp"

#include "../graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "AbstractBrickModel.hpp"

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
  FloorsManager& getFloorsManager() override { return this->_floorsManager; }
  WallsManager& getWallsManager() override { return this->_wallsManager; }
  BrickInstancesManager& getBrickInstancesManager() override { return this->_brickInstancesManager; }
  const std::string& getName() const override {return _name; }

public:
  void buildVertices(IWireFramesStackRenderer& inWireFrames) const override;
  void buildInstances(const glm::vec3& inOrigin, const glm::quat& inQuat, InstancedBrickModels& inInstancedBrickModels) const override;
  void render(const glm::mat4& transform) const override;

private:
  std::string _name;
  FloorsManager _floorsManager;
  WallsManager _wallsManager;
  BrickInstancesManager _brickInstancesManager;

};
