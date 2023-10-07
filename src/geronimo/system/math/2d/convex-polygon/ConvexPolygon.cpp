
#include "ConvexPolygon.hpp"

#include "geronimo/system/math/2d/collisions/collisions.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/TraceLogger.hpp"

namespace gero {
namespace math {


void ConvexPolygon::preAllocate(std::size_t size)
{
  _vertices.reserve(size);
}

void ConvexPolygon::addVertex(const glm::vec2& pos)
{
  _vertices.push_back(pos);
}

void ConvexPolygon::validate()
{
  _center = glm::vec2(0,0);
  for (const glm::vec2& vertex : _vertices)
    _center += vertex;
  _center /= float(_vertices.size());

  if (!contains(_center)) {
    for (std::size_t ii = 0; ii < _vertices.size() / 2; ++ii)
    {
      std::size_t jj = _vertices.size() - 1 - ii;
      std::swap(_vertices.at(ii), _vertices.at(jj));
    }
  }
}

bool ConvexPolygon::contains(const glm::vec2& pos)
{

  // D_MYLOG("-> pos " << pos);

  for (std::size_t ii = 0; ii < _vertices.size(); ++ii)
  {
    std::size_t jj = (ii + 1) % _vertices.size();;

    // D_MYLOG("  -> p1 " << _vertices.at(ii) << ", p2 " << _vertices.at(jj));

    if (pointIsLeftOfLine(_vertices.at(ii), _vertices.at(jj), pos))
      return false;
  }
  return true;
}

float ConvexPolygon::intersect(const glm::vec2& origin, const glm::vec2& dest)
{
  float finalValue = -1.0f;

  for (std::size_t ii = 0; ii < _vertices.size(); ++ii)
  {
    const std::size_t jj = (ii + 1) % _vertices.size();;

    // D_MYLOG("  -> p1 " << _vertices.at(ii) << ", p2 " << _vertices.at(jj));

    const float tmpValue = rawCollisionSegmentSegment(origin, dest, _vertices.at(ii), _vertices.at(jj));

    if (tmpValue > 0.0f && (finalValue < 0.0f || finalValue > tmpValue)) {
      finalValue = tmpValue;
    }
  }
  return finalValue;
}

// glm::vec2 ConvexPolygon::limitate(const glm::vec2& pos, float offset /*= 0.0f*/)
// {
//   const float lerpCoef = intersect(_center, pos);
//   if (lerpCoef < 0.0f) {
//     return _center;
//   }
//   return gero::math::lerp(_center, pos, lerpCoef + offset);
// }

const std::vector<glm::vec2>& ConvexPolygon::getVertices() const
{
  return _vertices;
}


}
}

