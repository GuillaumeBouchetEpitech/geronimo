
#pragma once

#include "WallQuad.hpp"
#include "../BuildingBrickModel.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

class WallBuilder
{
public:

  struct ConnectOpts {
    float center;
    float width;

    ConnectOpts(float inCenter, float inWidth): center(inCenter), width(inWidth) {}
  };

  // using MaybeGenericQuadRef = std::optional<std::reference_wrapper<WallQuad>>;

  enum class QuadCreateError {
    is_blocked,
    not_aligned,
    out_of_range
  };

  using ExpectGenericQuadRef = std::expected<std::reference_wrapper<const WallQuad>, QuadCreateError>;

  //
  //
  //

public:
  WallBuilder() = default;
  ~WallBuilder() = default;

public:
  ExpectGenericQuadRef addWallFromOrigin(
    const glm::vec3& inOrigin,
    const glm::vec2& inSize,
    WallOrientation inWallOrientation);

  ExpectGenericQuadRef makeWallAdjacentToFloor(
    const FloorQuad& inFloorQuad,
    WallOrientation inWallOrientation,
    float inHeight
  );
  ExpectGenericQuadRef makeWallAdjacentToFloor(
    const glm::vec3& inCenter,
    const glm::vec3& inSize,
    WallOrientation inWallOrientation,
    float inHeight
  );
  ExpectGenericQuadRef makeWallAdjacentToFloor(
    const glm::vec3& inCenter,
    float inRadius,
    WallOrientation inWallOrientation,
    float inHeight
  );

  bool removeWallFromOrigin(const glm::vec3& inOrigin, const glm::vec3& inSize);

public:
  void mergeAllAdjacentQuads();

public:
  void setBuildingBrickModel(BuildingBrickModel* inBuildingBrickModel) { this->_model = inBuildingBrickModel; }
  void resetBuildingBrickModel() { this->_model = nullptr; }

private:
  BuildingBrickModel* _model = nullptr;

};
