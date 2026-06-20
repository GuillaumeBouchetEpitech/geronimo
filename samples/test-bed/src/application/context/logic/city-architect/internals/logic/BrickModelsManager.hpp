
#pragma once

#include "AbstractBrickModel.hpp"
#include "BrickModel.hpp"

#include "floors/FloorBuilder.hpp"
#include "walls/WallBuilder.hpp"
#include "brick-instances/BrickInstanceBuilder.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

//
//
//

using BrickModelsPool = gero::weak_ref_data_pool<BrickModel, AbstractBrickModel, 64, true>;

class BrickModelsManager
{
public:
  BrickModelsManager() = default;
  ~BrickModelsManager() = default;

public:
  void render(const glm::mat4& transform) const;

public:
  BrickModelsPool  _brickModelsPool;

  FloorBuilder _floorBuilder;
  WallBuilder _wallBuilder;
  BrickInstanceBuilder _brickInstanceBuilder;


};
