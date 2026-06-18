
#pragma once

#include "FloorQuad.hpp"
#include "BuildingBrickModel.hpp"
#include "FloorBuilder.hpp"

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

};
