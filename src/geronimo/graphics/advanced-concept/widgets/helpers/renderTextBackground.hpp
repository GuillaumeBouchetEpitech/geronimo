
#pragma once

#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/advanced-concept/textRenderer/TextRenderer.hpp"
#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace graphics {

namespace helpers {

void renderTextBackground(float textDepth,
                          const glm::vec4& colorA,
                          const glm::vec4& colorB,
                          float extraSizeA,
                          float extraSizeB,
                          gero::graphics::StackRenderers& inStackRenderers,
                          gero::graphics::TextRenderer& inTextRenderer);

}

} // namespace graphics
} // namespace gero
