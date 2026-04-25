
#pragma once

#include <memory>

class AbstractVoxelSim {
public:
  virtual ~AbstractVoxelSim() = default;

public:
  virtual void update(float elapsedTime) = 0;
  virtual void render() = 0;

public:
  static std::unique_ptr<AbstractVoxelSim> create();

};
