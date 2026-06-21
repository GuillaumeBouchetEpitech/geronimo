
#pragma once

#include "AbstractBrickModel.hpp"
#include "BrickModel.hpp"

#include "floors/FloorBuilder.hpp"
#include "walls/WallBuilder.hpp"
#include "brick-instances/BrickInstanceBuilder.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

#include <unordered_map>
#include <expected>
#include <optional>

//
//
//

using BrickModelsPool = gero::weak_ref_data_pool<BrickModel, AbstractBrickModel, 64, true>;

enum class BrickCreateError {
  duplicated_name
};

using ExpectBrickRef = std::expected<AbstractBrickModelWeakRef, BrickCreateError>;
using MaybeBrickRef = std::optional<AbstractBrickModelWeakRef>;

class BrickModelsManager
{
public:
  BrickModelsManager() = default;
  ~BrickModelsManager() = default;

public:
  ExpectBrickRef createNewBrick(const std::string& inName);

  MaybeBrickRef getBrickByName(const std::string& inName);
  const MaybeBrickRef getBrickByName(const std::string& inName) const;

// public:
//   void render(const glm::mat4& transform) const;

public:
  BrickModelsPool  _brickModelsPool;
  std::unordered_map<std::string, AbstractBrickModelWeakRef> _brickModelsMap;

  FloorBuilder _floorBuilder;
  WallBuilder _wallBuilder;
  BrickInstanceBuilder _brickInstanceBuilder;


};
