
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <array>
#include <cmath>

namespace gero {
namespace graphics {

class IFrustumCulling {
public:
  virtual bool pointInFrustum(const glm::vec3& inCenter) const = 0;
  virtual bool sphereInFrustum(const glm::vec3& inCenter, float inRadius) const = 0;
  virtual bool cubeInFrustum(const glm::vec3& inCenter, const glm::vec3& inCubeSize) const = 0;
};

//
//
//

class FrustumCulling : public IFrustumCulling {
public:
  void calculateFrustum(const glm::mat4& inProj, const glm::mat4& inView);
  void calculateFrustum(const glm::mat4& inComposed);

public:
  virtual bool pointInFrustum(const glm::vec3& inCenter) const override;
  virtual bool sphereInFrustum(const glm::vec3& inCenter, float inRadius) const override;
  virtual bool cubeInFrustum(const glm::vec3& inCenter, const glm::vec3& inCubeSize) const override;

private:
  enum class FrustumSide : std::size_t { eRight = 0, eLeft, eBottom, eTop, eBack, eFront };

private:
  void _setPlane(FrustumSide inSide, const glm::vec4& inValue);

private:
  std::array<glm::vec4, 6> _planes;
};

} // namespace graphics
} // namespace gero
