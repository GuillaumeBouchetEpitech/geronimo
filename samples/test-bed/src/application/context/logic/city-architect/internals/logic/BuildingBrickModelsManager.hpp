
#pragma once

#include "BuildingBrickModel.hpp"

#include "floors/FloorBuilder.hpp"
#include "walls/WallBuilder.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

class BuildingBrickModelsManager
{
public:
  BuildingBrickModelsManager();
  ~BuildingBrickModelsManager() = default;

public:
  std::vector<BuildingBrickModel> _buildingBrickModels;
  FloorBuilder _floorBuilder;
  WallBuilder _wallBuilder;

};
