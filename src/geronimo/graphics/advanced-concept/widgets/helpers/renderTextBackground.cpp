
#include "renderTextBackground.hpp"

namespace gero {
namespace graphics {

namespace helpers {

void renderTextBackground(float textDepth,
                          const glm::vec4& colorA,
                          const glm::vec4& colorB,
                          float extraSizeA,
                          float extraSizeB,
                          gero::graphics::StackRenderers& inStackRenderers,
                          gero::graphics::TextRenderer& inTextRenderer) {

  auto& triangles = inStackRenderers.getTrianglesStack();

  const auto& outRectangles = inTextRenderer.getLatestTextRectangles();

  {
    // const glm::vec4 bgColor = glm::vec4(0.0f, 0.0f, 0.0f, _alpha * 0.75f);
    const float bgDepth = textDepth - 0.1f;

    for (const auto& rec : outRectangles)
      // if (rec.size.x > 0.0f)
      triangles.pushQuad(rec.pos + rec.size * 0.5f, rec.size + extraSizeA, colorA, bgDepth);
  }

  {
    // const glm::vec4 bgColor = glm::vec4(0.3f, 0.3f, 0.3f, _alpha * 0.75f);
    const float bgDepth = textDepth - 0.2f;

    for (const auto& rec : outRectangles)
      // if (rec.size.x > 0.0f)
      triangles.pushQuad(rec.pos + rec.size * 0.5f, rec.size + extraSizeB, colorB, bgDepth);
  }
}

} // namespace helpers

} // namespace graphics
} // namespace gero
