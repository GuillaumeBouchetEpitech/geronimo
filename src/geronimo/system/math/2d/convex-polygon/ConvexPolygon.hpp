
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

namespace gero {
namespace math {

class ConvexPolygon
{
public:
  ConvexPolygon() = default;
  ~ConvexPolygon() = default;

public:
  void preAllocate(std::size_t size);
  void addVertex(const glm::vec2& pos);
  void validate();

public:
  bool contains(const glm::vec2& pos);

public:
  float intersect(const glm::vec2& origin, const glm::vec2& dest);
  // glm::vec2 limitate(const glm::vec2& pos, float offset = 0.0f);

public:
  const std::vector<glm::vec2>& getVertices() const;

private:
  std::vector<glm::vec2> _vertices;
  glm::vec2 _center;

};

}
}
