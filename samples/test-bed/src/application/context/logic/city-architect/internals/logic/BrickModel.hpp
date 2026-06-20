
#pragma once

#include "floors/FloorsManager.hpp"
#include "walls/WallsManager.hpp"
#include "brick-instances/BrickInstancesManager.hpp"

#include "../graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "AbstractBrickModel.hpp"

class BrickModel : public AbstractBrickModel
{
public:
  BrickModel() = default;
  ~BrickModel() = default;

public:
  const FloorsManager& getFloorsManager() const override { return this->_floorsManager; }
  const WallsManager& getWallsManager() const override { return this->_wallsManager; }
  const BrickInstancesManager& getBrickInstancesManager() const override { return this->_brickInstancesManager; }

private:
  FloorsManager& getFloorsManager() override { return this->_floorsManager; }
  WallsManager& getWallsManager() override { return this->_wallsManager; }
  BrickInstancesManager& getBrickInstancesManager() override { return this->_brickInstancesManager; }

public:
  void buildVertices(IWireFramesStackRenderer& inWireFrames) const;
  void render(const glm::mat4& transform) const override;

private:
  FloorsManager _floorsManager;
  WallsManager _wallsManager;
  BrickInstancesManager _brickInstancesManager;

};
