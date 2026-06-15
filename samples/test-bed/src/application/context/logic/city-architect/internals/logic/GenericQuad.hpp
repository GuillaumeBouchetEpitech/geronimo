
#pragma once

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/asValue.hpp"

#include <array>

class GenericQuad {
public:
  enum class FloorVertexType : std::size_t {
    negX_posY = 0,
    posX_posY,
    posX_negY,
    negX_negY,
  };

private:
  GenericQuad() = default;

public:
  virtual ~GenericQuad() = default;

public:

  [[nodiscard]]
  static GenericQuad makeFloorFromOrigin(const glm::vec3& origin, const glm::vec2& size);

  [[nodiscard]]
  static GenericQuad makeFloorConnection(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

public:
  [[nodiscard]]
  const glm::vec3& getFloorVertex(FloorVertexType type) const {
    return this->_vertices.at(gero::asValue(type));
  }

  [[nodiscard]]
  float getFloorZ(float inX, float inY) const;

  [[nodiscard]]
  bool collide(const glm::vec3& inCenter, const glm::vec3& inSize) const;

  [[nodiscard]]
  bool collide(const glm::vec3& inCenter, float inRadius) const;

  [[nodiscard]]
  const glm::vec3& getOrigin() const {
    return this->getFloorVertex(FloorVertexType::negX_negY);
  }

  [[nodiscard]]
  glm::vec3 getSize() const {
    return this->getFloorVertex(FloorVertexType::posX_posY) - this->getFloorVertex(FloorVertexType::negX_negY);
  }

private:
  // glm::vec3 _origin;
  // glm::vec3 _size;
  // glm::vec3 _normal;
  // float _thickness = 0.1f;

  std::array<glm::vec3, 4> _vertices = {{
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0)
  }};



};
