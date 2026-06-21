
#pragma once

#include "../../graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/asValue.hpp"

#include <array>
#include <vector>
#include <optional>

class FloorQuad {
public:
  enum class FloorVertexType : std::size_t {
    negX_posY = 0,
    posX_posY,
    posX_negY,
    negX_negY,
  };

private:
  // must use the static and/or deducing method
  FloorQuad() = default;

public:
  virtual ~FloorQuad() = default;

public:

  // create new floor quad(s)

  [[nodiscard]]
  static FloorQuad makeFloorFromOrigin(const glm::vec3& origin, const glm::vec2& size);

  [[nodiscard]]
  static FloorQuad makeFloorConnection(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

  [[nodiscard]]
  static FloorQuad makeFloorFromMat4(const FloorQuad& inQuad, const glm::mat4& inTransform);

public:

  // deduce new floor quad(s)

  [[nodiscard]]
  bool divideFromCoords(const std::vector<glm::vec2>& inCutCoords, std::vector<FloorQuad>& outFloorQuads) const;

  [[nodiscard]]
  std::optional<FloorQuad> getMergedQuad(const FloorQuad& other) const;

public:

  // getter(s)

  [[nodiscard]]
  const glm::vec3& getFloorVertex(FloorVertexType type) const {
    return this->_vertices.at(gero::asValue(type));
  }

  [[nodiscard]]
  float getFloorZ(float inX, float inY) const;

  [[nodiscard]]
  bool isColliding(const glm::vec3& inCenter, const glm::vec3& inSize) const;

  [[nodiscard]]
  bool isColliding(const glm::vec3& inCenter, float inRadius) const;

  // [[nodiscard]]
  // bool isIntersecting(const FloorQuad& other, float epsilon = 0.1f) const;

  [[nodiscard]]
  const glm::vec3& getOrigin() const {
    return this->getFloorVertex(FloorVertexType::negX_negY);
  }

  [[nodiscard]]
  glm::vec3 getSize() const {
    return this->getFloorVertex(FloorVertexType::posX_posY) - this->getFloorVertex(FloorVertexType::negX_negY);
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

private:
  std::array<glm::vec3, 4> _vertices = {{
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0)
  }};



};
