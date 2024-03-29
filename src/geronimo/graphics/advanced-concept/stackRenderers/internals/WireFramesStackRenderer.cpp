
#include "WireFramesStackRenderer.hpp"

#include "geronimo/system/asValue.hpp"
#include "geronimo/system/math/constants.hpp"

namespace gero {
namespace graphics {

void WireFramesStackRenderer::initialize(gero::graphics::IUnboundShaderProgram& shader,
                                         const gero::graphics::Geometry::Definition& geoDef) {

  _geometry.initialize(shader, geoDef);
  _geometry.setPrimitiveCount(0);

  constexpr std::size_t preAllocatedSize = 1024 * 32; // 32Ko * 28(<- 7 * float4)
  // constexpr std::size_t preAllocatedSize = 1024 * 1024 * 4; // 4Mo
  _vertices.reserve(preAllocatedSize);
  // _geometry.preAllocateBufferFromCapacity(0, _vertices);
}

//

void WireFramesStackRenderer::pushLine(const glm::vec3& posA,
                                       const glm::vec3& posB,
                                       const glm::vec4& colorA,
                                       const glm::vec4& colorB) {
  if (_vertices.size() + 2 >= _vertices.capacity()) {
    if (_safeMode) {
      flush();
    } else {
      // D_THROW(std::runtime_error, "out of memory");
      return;
    }
  }

  _vertices.emplace_back(posA, colorA);
  _vertices.emplace_back(posB, colorB);
}

void WireFramesStackRenderer::pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) {

  pushLine(posA, posB, color, color);
}

void WireFramesStackRenderer::pushLine(const glm::vec3& posA,
                                       const glm::vec3& posB,
                                       const glm::vec3& colorA,
                                       const glm::vec3& colorB) {
  pushLine(posA, posB, glm::vec4(colorA, 1.0f), glm::vec4(colorB, 1.0f));
}

void WireFramesStackRenderer::pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) {
  pushLine(posA, posB, glm::vec4(color, 1.0f));
}

//

void WireFramesStackRenderer::pushCross(const glm::vec3& pos, const glm::vec3& color, float halfExtent) {
  if (halfExtent <= 0)
    return;

  std::array<std::array<glm::vec3, 2>, 3> vertices{
    {{{{pos.x - halfExtent, pos.y, pos.z}, {pos.x + halfExtent, pos.y, pos.z}}},
     {{{pos.x, pos.y - halfExtent, pos.z}, {pos.x, pos.y + halfExtent, pos.z}}},
     {{{pos.x, pos.y, pos.z - halfExtent}, {pos.x, pos.y, pos.z + halfExtent}}}}};

  for (const auto& elem : vertices)
    pushLine(elem.at(0), elem.at(1), color);
}

void WireFramesStackRenderer::pushRectangle(const glm::vec2& pos,
                                            const glm::vec2& size,
                                            const glm::vec3& color,
                                            float depth /*= 0.0f*/) {
  const glm::vec2 farPos = pos + size;

  std::array<glm::vec3, 4> vertices = {{
    glm::vec3(pos, depth),
    glm::vec3(farPos.x, pos.y, depth),
    glm::vec3(farPos, depth),
    glm::vec3(pos.x, farPos.y, depth),
  }};

  WireFramesStackRenderer::pushLine(vertices.at(0), vertices.at(1), color);
  WireFramesStackRenderer::pushLine(vertices.at(1), vertices.at(2), color);
  WireFramesStackRenderer::pushLine(vertices.at(2), vertices.at(3), color);
  WireFramesStackRenderer::pushLine(vertices.at(3), vertices.at(0), color);
}

void WireFramesStackRenderer::flush() {
  if (!canRender())
    return;

  // _geometry.updateBuffer(0, _vertices);
  _geometry.allocateBuffer(0, _vertices);
  _geometry.setPrimitiveCount(uint32_t(_vertices.size()));
  _geometry.render();

  _vertices.clear();
}

bool WireFramesStackRenderer::canRender() const { return !_vertices.empty(); }

void WireFramesStackRenderer::startSafeMode() { _safeMode = true; }

void WireFramesStackRenderer::stopSafeMode() { _safeMode = false; }

} // namespace graphics
} // namespace gero
