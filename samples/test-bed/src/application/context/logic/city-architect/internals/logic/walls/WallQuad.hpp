
#pragma once

#include "../../graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/asValue.hpp"

#include <array>
#include <vector>
#include <optional>

// forward declaration
class FloorQuad;

enum class WallOrientation : uint32_t {
  posX = 0,
  negX,
  posY,
  negY
};

class WallQuad {
public:
  enum class VertexType : std::size_t {
    negAxis_posZ = 0,
    posAxis_posZ,
    posAxis_negZ,
    negAxis_negZ,
  };

private:
  // must use the static and/or deducing method
  WallQuad() = default;

public:
  virtual ~WallQuad() = default;

public:

  // create new wall quad(s)

  [[nodiscard]]
  static WallQuad makeWallFromOrigin(const glm::vec3& origin, const glm::vec2& size, WallOrientation inWallOrientation);

  [[nodiscard]]
  static WallQuad makeWallAdjacentToFloor(
    const FloorQuad& inFloorQuad,
    WallOrientation inWallOrientation,
    float inHeight
  );

public:

  // deduce new floor quad(s)

  [[nodiscard]]
  bool divideFromCoords(const std::vector<glm::vec2>& inCutCoords, std::vector<WallQuad>& outWallQuads) const;

  [[nodiscard]]
  std::optional<WallQuad> getMergedQuad(const WallQuad& other) const;

public:

  // getter(s)

  [[nodiscard]]
  const glm::vec3& getVertex(VertexType type) const {
    return this->_vertices.at(gero::asValue(type));
  }

  // [[nodiscard]]
  // float getWallZ(float inAxis) const;

  [[nodiscard]]
  bool isColliding(const glm::vec3& inCenter, const glm::vec3& inSize) const;

  [[nodiscard]]
  bool isColliding(const glm::vec3& inCenter, float inRadius) const;

  [[nodiscard]]
  const glm::vec3& getOrigin() const {
    return this->getVertex(VertexType::negAxis_negZ);
  }

  [[nodiscard]]
  glm::vec3 getSize() const {
    return this->getVertex(VertexType::posAxis_posZ) - this->getOrigin();
  }

  [[nodiscard]]
  glm::vec3 getCenter() const {
    return this->getOrigin() + this->getSize() * 0.5f;
  }

  [[nodiscard]]
  glm::vec3 getNormal() const;

public:

  // render

  void buildVertices(IWireFramesStackRenderer& inWireFrames) const;
  void render() const;

protected:
  std::array<glm::vec3, 4> _vertices = {{
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0)
  }};

};
