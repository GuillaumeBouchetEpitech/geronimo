
#pragma once

#include "FloorQuad.hpp"
#include "BuildingBrickModel.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

class FloorBuilder
{
public:

  struct ConnectOpts {
    float center;
    float width;

    ConnectOpts(float inCenter, float inWidth): center(inCenter), width(inWidth) {}
  };

  // using MaybeGenericQuadRef = std::optional<std::reference_wrapper<FloorQuad>>;

  enum class QuadCreateError {
    is_blocked,
    not_aligned,
    out_of_range
  };

  using ExpectGenericQuadRef = std::expected<std::reference_wrapper<const FloorQuad>, QuadCreateError>;

  //
  //
  //

public:
  FloorBuilder() = default;
  ~FloorBuilder() = default;

public:
  ExpectGenericQuadRef addFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize);
  bool removeFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec3& inSize);

public:
  bool connectFloors(
    const FloorQuad& inFloorA,
    const FloorQuad& inFloorB,
    const ConnectOpts& inOpts = ConnectOpts(0.0, 0.0f)
  );
  bool connectFloors(
    const glm::vec3& inCenterA, const glm::vec3& inSizeA,
    const glm::vec3& inCenterB, const glm::vec3& inSizeB,
    const ConnectOpts& inOpts = ConnectOpts(0.0, 0.0f)
  );
  bool connectFloors(
    const glm::vec3& inCenterA, float inRadiusA,
    const glm::vec3& inCenterB, float inRadiusB,
    const ConnectOpts& inOpts = ConnectOpts(0.0, 0.0f)
  );

public:
  void mergeAllAdjacentQuads();

public:
  void setBuildingBrickModel(BuildingBrickModel* inBuildingBrickModel) { this->_model = inBuildingBrickModel; }
  void resetBuildingBrickModel() { this->_model = nullptr; }

private:
  // std::vector<FloorQuad> _floorQuads;
  BuildingBrickModel* _model = nullptr;

};
