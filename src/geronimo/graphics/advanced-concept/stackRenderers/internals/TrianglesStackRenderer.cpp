
#include "TrianglesStackRenderer.hpp"

#include "geronimo/system/asValue.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/constants.hpp"

namespace gero {
namespace graphics {

void TrianglesStackRenderer::initialize(gero::graphics::IUnboundShaderProgram& shader,
                                        const gero::graphics::Geometry::Definition& geoDef) {

  _geometry.initialize(shader, geoDef);
  _geometry.setPrimitiveCount(0);

  constexpr std::size_t preAllocatedSize = 1024 * 32; // 32Ko
  _vertices.reserve(preAllocatedSize);
  // _geometry.preAllocateBufferFromCapacity(0, _vertices);
}

void TrianglesStackRenderer::pushTriangle(const glm::vec3& posA,
                                          const glm::vec3& posB,
                                          const glm::vec3& posC,
                                          const glm::vec4& colorA,
                                          const glm::vec4& colorB,
                                          const glm::vec4& colorC) {
  if (_vertices.size() + 3 >= _vertices.capacity()) {
    if (_safeMode) {
      flush();
    } else {
      // D_THROW(std::runtime_error, "out of memory");
      return;
    }
  }

  _vertices.emplace_back(posA, colorA);
  _vertices.emplace_back(posB, colorB);
  _vertices.emplace_back(posC, colorC);
}

void TrianglesStackRenderer::pushTriangle(const glm::vec3& posA,
                                          const glm::vec3& posB,
                                          const glm::vec3& posC,
                                          const glm::vec4& color) {
  pushTriangle(posA, posB, posC, color, color, color);
}

// namespace {

// glm::vec3 _xxxxx____sortingCall(
//   // const std::array<glm::vec3, 4>& inVertices,
//   const glm::vec3& posA,
//   const glm::vec3& posB,
//   const glm::vec3& posC,
//   const glm::vec3& posD,
//   bool inDirY,
//   bool inDirX
// ) {
//   const std::array<glm::vec3, 4> tmpVertices = {{ posA, posB, posC, posD }};

//   uint32_t bestIndex = 0;
//   // for (uint32_t ii = 1; ii < uint32_t(inVertices.size()); ++ii)
//   for (uint32_t ii = 1; ii < uint32_t(tmpVertices.size()); ++ii)
//   {
//     // const glm::vec3& best = inVertices.at(bestIndex);
//     // const glm::vec3& current = inVertices.at(ii);
//     const glm::vec3& best = tmpVertices.at(bestIndex);
//     const glm::vec3& current = tmpVertices.at(ii);

//     if (inDirX && best.x > current.x) {
//       continue;
//     }
//     if (!inDirX && best.x < current.x) {
//       continue;
//     }
//     if (inDirY && best.y > current.y) {
//       continue;
//     }
//     if (!inDirY && best.y < current.y) {
//       continue;
//     }

//     bestIndex = ii;
//   }

//   // return inVertices.at(bestIndex);
//   return tmpVertices.at(bestIndex);
// }

// };

// void
// TrianglesStackRenderer::push3dQuad(
//   const glm::vec3& posA,
//   const glm::vec3& posB,
//   const glm::vec3& posC,
//   const glm::vec3& posD,
//   const glm::vec4& color)
// {
//   const std::array<glm::vec3, 4> tmpVertices = {{ posA, posB, posC, posD }};

//   const auto _lol = [&tmpVertices](bool inDirY, bool inDirX) -> uint32_t {

//     uint32_t bestIndex = 0;
//     // for (uint32_t ii = 1; ii < uint32_t(inVertices.size()); ++ii)
//     for (uint32_t ii = 1; ii < uint32_t(tmpVertices.size()); ++ii)
//     {
//       // const glm::vec3& best = inVertices.at(bestIndex);
//       // const glm::vec3& current = inVertices.at(ii);
//       const glm::vec3& best = tmpVertices.at(bestIndex);
//       const glm::vec3& current = tmpVertices.at(ii);

//       if (inDirX && best.x > current.x) {
//         continue;
//       }
//       if (!inDirX && best.x < current.x) {
//         continue;
//       }
//       if (inDirY && best.y > current.y) {
//         continue;
//       }
//       if (!inDirY && best.y < current.y) {
//         continue;
//       }

//       bestIndex = ii;
//     }
//     return bestIndex;
//   };



//   // const glm::vec3 posX_posY = _xxxxx____sortingCall(tmpVertices, true, true);
//   // const glm::vec3 posX_negY = _xxxxx____sortingCall(tmpVertices, false, true);
//   // const glm::vec3 negX_posY = _xxxxx____sortingCall(tmpVertices, true, false);
//   // const glm::vec3 negX_negY = _xxxxx____sortingCall(tmpVertices, false, false);
//   const glm::vec3 posX_posY = tmpVertices.at(_lol(true, true));
//   const glm::vec3 posX_negY = tmpVertices.at(_lol(false, true));
//   const glm::vec3 negX_posY = tmpVertices.at(_lol(true, false));
//   const glm::vec3 negX_negY = tmpVertices.at(_lol(false, false));

//   pushTriangle(
//     negX_negY,
//     posX_negY,
//     negX_posY,
//     color
//   );

//   pushTriangle(
//     posX_posY,
//     negX_posY,
//     posX_negY,
//     color
//   );
// }

void TrianglesStackRenderer::pushThickTriangle2dLine(const glm::vec2& posA,
                                                     const glm::vec2& posB,
                                                     float thicknessA,
                                                     float thicknessB,
                                                     const glm::vec4& colorA,
                                                     const glm::vec4& colorB,
                                                     float z) {
  const float angle = std::atan2(posB.y - posA.y, posB.x - posA.x) + gero::math::hpi;

  const glm::vec2 direction = {std::cos(angle), std::sin(angle)};
  const glm::vec2 stepA = direction * thicknessA * 0.5f;
  const glm::vec2 stepB = direction * thicknessB * 0.5f;

  std::array<glm::vec3, 4> vertices{{
    {posA.x - stepA.x, posA.y - stepA.y, z},
    {posA.x + stepA.x, posA.y + stepA.y, z},
    {posB.x - stepB.x, posB.y - stepB.y, z},
    {posB.x + stepB.x, posB.y + stepB.y, z},
  }};

  pushTriangle(vertices.at(0), vertices.at(3), vertices.at(2), colorA, colorB, colorB);

  pushTriangle(vertices.at(0), vertices.at(1), vertices.at(3), colorA, colorA, colorB);
}

void TrianglesStackRenderer::pushQuad(const glm::vec2& center, const glm::vec2& size, const glm::vec4& color, float z) {
  const glm::vec2 hsize = size * 0.5f;

  const std::array<glm::vec3, 4> vertices = {{
    {center.x + hsize.x, center.y + hsize.y, z},
    {center.x - hsize.x, center.y + hsize.y, z},
    {center.x + hsize.x, center.y - hsize.y, z},
    {center.x - hsize.x, center.y - hsize.y, z},
  }};

  std::array<glm::uvec3, 2> indices = {{{1, 0, 2}, {2, 1, 3}}};

  for (const glm::uvec3& index : indices)
    pushTriangle(vertices.at(index[0]), vertices.at(index[1]), vertices.at(index[2]), color);
}

void TrianglesStackRenderer::pushQuad(const glm::vec2& center, const glm::vec2& size, const glm::vec4& color) {
  pushQuad(center, size, color, 0.0f);
}

void TrianglesStackRenderer::pushCircle(const glm::vec2& center, float radius, const glm::vec4& color, float z) {
  constexpr std::size_t quality = 16;

  std::array<glm::vec2, quality> cachedVertices;

  for (std::size_t ii = 0; ii < cachedVertices.size(); ++ii) {
    const float coef = float(ii) / quality;
    const float currAngle = coef * gero::math::pi2;

    // cachedVertices.at(ii) = { std::cos(currAngle), std::sin(currAngle) };
    cachedVertices.at(ii) = gero::math::getDirection(currAngle);
  }

  const glm::vec3 vertex0 = glm::vec3(center, z);

  for (std::size_t ii = 0; ii < cachedVertices.size(); ++ii) {
    const std::size_t index1 = ii;
    const std::size_t index2 = (ii + 1) % quality;

    const glm::vec2 tmpPos1 = cachedVertices.at(index1) * radius;
    const glm::vec2 tmpPos2 = cachedVertices.at(index2) * radius;

    const glm::vec3 vertex1(center.x + tmpPos1.x, center.y + tmpPos1.y, z);
    const glm::vec3 vertex2(center.x + tmpPos2.x, center.y + tmpPos2.y, z);

    pushTriangle(vertex0, vertex1, vertex2, color);
  }
}

void TrianglesStackRenderer::pushCircle(const glm::vec2& center, float radius, const glm::vec4& color) {
  pushCircle(center, radius, color, 0.0f);
}

void TrianglesStackRenderer::pushThickTriangle2dLine(const glm::vec2& posA,
                                                     const glm::vec2& posB,
                                                     float thicknessA,
                                                     float thicknessB,
                                                     const glm::vec4& colorA,
                                                     const glm::vec4& colorB) {
  pushThickTriangle2dLine(posA, posB, thicknessA, thicknessB, colorA, colorB, 0.0f);
}

void TrianglesStackRenderer::pushThickTriangle2dLine(
  const glm::vec2& posA, const glm::vec2& posB, float thickness, const glm::vec4& color, float z) {
  pushThickTriangle2dLine(posA, posB, thickness, thickness, color, color, z);
}

void TrianglesStackRenderer::pushThickTriangle2dLine(const glm::vec2& posA,
                                                     const glm::vec2& posB,
                                                     float thickness,
                                                     const glm::vec4& color) {
  pushThickTriangle2dLine(posA, posB, thickness, thickness, color, color, 0.0f);
}

void TrianglesStackRenderer::pushThickTriangle3dLine(const glm::vec3& posA,
                                                     const glm::vec3& posB,
                                                     float thicknessA,
                                                     float thicknessB,
                                                     const glm::vec4& colorA,
                                                     const glm::vec4& colorB) {
  const glm::vec3 diff = posB - posA;
  const float horizontalAngle = std::atan2(diff.y, diff.x);
  const float verticalAngle = std::atan2(diff.z, glm::length(glm::vec2(diff.x, diff.y)));

  glm::mat4 transform = glm::identity<glm::mat4>();
  transform = glm::rotate(transform, horizontalAngle, glm::vec3(0,0,1));
  transform = glm::rotate(transform, -verticalAngle, glm::vec3(0,1,0));

  // transform

  const float magnitude = glm::length(diff);

  std::array<glm::vec3, 4> sideA = {{
    glm::vec3(magnitude*0.0f,+thicknessA*0.5f,+thicknessA*0.5f),
    glm::vec3(magnitude*0.0f,-thicknessA*0.5f,+thicknessA*0.5f),
    glm::vec3(magnitude*0.0f,-thicknessA*0.5f,-thicknessA*0.5f),
    glm::vec3(magnitude*0.0f,+thicknessA*0.5f,-thicknessA*0.5f),
  }};
  std::array<glm::vec3, 4> sideB = {{
    glm::vec3(magnitude*1.0f,+thicknessB*0.5f,+thicknessB*0.5f),
    glm::vec3(magnitude*1.0f,-thicknessB*0.5f,+thicknessB*0.5f),
    glm::vec3(magnitude*1.0f,-thicknessB*0.5f,-thicknessB*0.5f),
    glm::vec3(magnitude*1.0f,+thicknessB*0.5f,-thicknessB*0.5f),
  }};

  for (glm::vec3& pos : sideA)
    pos = posA + glm::vec3(transform * glm::vec4(pos, 0.0f));
  for (glm::vec3& pos : sideB)
    pos = posA + glm::vec3(transform * glm::vec4(pos, 0.0f));

  struct Vertex {
    const glm::vec3& position;
    const glm::vec4& color;
  };

  std::array<std::array<Vertex, 4>, 4> allQuads{{
    {{{sideA.at(0), colorA}, {sideA.at(1), colorA}, {sideB.at(0), colorB}, {sideB.at(1), colorB}}},
    {{{sideA.at(1), colorA}, {sideA.at(2), colorA}, {sideB.at(1), colorB}, {sideB.at(2), colorB}}},
    {{{sideA.at(2), colorA}, {sideA.at(3), colorA}, {sideB.at(2), colorB}, {sideB.at(3), colorB}}},
    {{{sideA.at(3), colorA}, {sideA.at(0), colorA}, {sideB.at(3), colorB}, {sideB.at(0), colorB}}},
  }};

  std::array<glm::uvec3, 2> indices = {{{0, 3, 2}, {0, 1, 3}}};

  for (auto& quad : allQuads) {
    for (const glm::uvec3& index : indices) {
      pushTriangle(quad.at(index[0]).position,
                   quad.at(index[1]).position,
                   quad.at(index[2]).position,
                   quad.at(index[0]).color,
                   quad.at(index[1]).color,
                   quad.at(index[2]).color);
    }
  }
}

void TrianglesStackRenderer::pushThickTriangle3dLine(const glm::vec3& posA,
                                                     const glm::vec3& posB,
                                                     float thickness,
                                                     const glm::vec4& color) {
  pushThickTriangle3dLine(posA, posB, thickness, thickness, color, color);
}

// void TrianglesStackRenderer::pushThickTriangle3dCross(
//   const glm::vec3& center,
//   float halfExtent,
//   float thickness,
//   const glm::vec4& color)
// {

//   pushThickTriangle3dLine(
//     center + glm::vec3(halfExtent, 0, 0),
//     center - glm::vec3(halfExtent, 0, 0),
//     thickness,
//     color
//   );
//   pushThickTriangle3dLine(
//     center + glm::vec3(0, halfExtent, 0),
//     center - glm::vec3(0, halfExtent, 0),
//     thickness,
//     color
//   );
//   pushThickTriangle3dLine(
//     center + glm::vec3(0, 0, halfExtent),
//     center - glm::vec3(0, 0, halfExtent),
//     thickness,
//     color
//   );

// }

void TrianglesStackRenderer::flush() {
  if (!canRender())
    return;

  // _geometry.updateBuffer(0, _vertices);
  _geometry.allocateBuffer(0, _vertices);
  _geometry.setPrimitiveCount(uint32_t(_vertices.size()));
  _geometry.render();

  _vertices.clear();
}

bool TrianglesStackRenderer::canRender() const { return !_vertices.empty(); }

void TrianglesStackRenderer::startSafeMode() { _safeMode = true; }

void TrianglesStackRenderer::stopSafeMode() { _safeMode = false; }

} // namespace graphics
} // namespace gero
