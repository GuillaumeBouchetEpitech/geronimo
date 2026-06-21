
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

enum class BrickInstanceError {
  unknown
};

using ExpectBrickInstanceRef = std::expected<std::reference_wrapper<const BrickInstance>, BrickInstanceError>;

class BrickInstanceBuilder
{
public:

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
