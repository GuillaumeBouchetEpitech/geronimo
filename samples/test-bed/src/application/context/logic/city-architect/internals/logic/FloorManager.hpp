
#pragma once

#include "GenericQuad.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>
#include <optional>
#include <expected>

//
//
//

class FloorManager
{
public:

  struct ConnectOpts {
    float center;
    float width;

    ConnectOpts(float inCenter, float inWidth): center(inCenter), width(inWidth) {}
  };

  // using MaybeGenericQuadRef = std::optional<std::reference_wrapper<GenericQuad>>;

  enum class ConnectionError {
    not_aligned,
    out_of_range
  };

  using ExpectGenericQuadRef = std::expected<std::reference_wrapper<const GenericQuad>, ConnectionError>;

  //
  //
  //

public:
  FloorManager();
  ~FloorManager() = default;

public:
  ExpectGenericQuadRef addFloorFromOrigin(const glm::vec3& inOrigin, const glm::vec2& inSize);

public:
  ExpectGenericQuadRef connectFloors(
    const GenericQuad& inFloorA,
    const GenericQuad& inFloorB,
    const ConnectOpts& inOpts = ConnectOpts(0.0, 0.0f)
  );

public:
  bool findQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<GenericQuad>& outQuads) const;
  bool findQuads(const glm::vec3& inCenter, float inRadius, std::vector<GenericQuad>& outQuads) const;

public:
  const std::vector<GenericQuad>& getFloorQuads() const { return this->_floorQuads; }

public:
  void render();

private:
  std::vector<GenericQuad> _floorQuads;

};
