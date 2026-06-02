
#pragma once

#include "geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/opengl/advanced/textRenderer/TextRenderer.hpp"
#include "geronimo/helpers/GLMath.hpp"

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {
namespace widgets {

namespace helpers {

void renderTextBackground(float textDepth,
                          const glm::vec4& colorA,
                          const glm::vec4& colorB,
                          float extraSizeA,
                          float extraSizeB,
                          gero::graphics::opengl::advanced::IStackRenderers& inStackRenderers,
                          gero::graphics::opengl::advanced::ITextRenderer& inTextRenderer);

} // namespace helpers

} // namespace widgets
} // namespace advanced
} // namespace opengl
} // namespace graphics
} // namespace gero
