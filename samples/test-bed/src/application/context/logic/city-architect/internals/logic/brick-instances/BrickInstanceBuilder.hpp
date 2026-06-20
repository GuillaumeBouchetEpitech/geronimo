
#pragma once

#include "BrickInstance.hpp"
#include "../BrickModel.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

class BrickInstanceBuilder
{
public:

  // struct ConnectOpts {
  //   float center;
  //   float width;

  //   ConnectOpts(float inCenter, float inWidth): center(inCenter), width(inWidth) {}
  // };

  // using MaybeGenericQuadRef = std::optional<std::reference_wrapper<WallQuad>>;

  enum class BrickCreateError {
    unknown //,
    // is_blocked,
    // not_aligned,
    // out_of_range
  };

  using ExpectBrickInstanceRef = std::expected<std::reference_wrapper<const BrickInstance>, BrickCreateError>;

  //
  //
  //

public:
  BrickInstanceBuilder() = default;
  ~BrickInstanceBuilder() = default;

public:
  ExpectBrickInstanceRef addInstance(
    const glm::vec3& inPos,
    const glm::quat& inQuat,
    AbstractBrickModelWeakRef inRef);

public:
  void setBrickModel(AbstractBrickModel* inBrickModel) { this->_model = inBrickModel; }
  void resetBrickModel() { this->_model = nullptr; }

private:
  AbstractBrickModel* _model = nullptr;

};
