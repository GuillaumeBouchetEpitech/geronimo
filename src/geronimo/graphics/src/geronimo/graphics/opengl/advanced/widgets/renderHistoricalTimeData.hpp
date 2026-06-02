
#pragma once

#include "geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/opengl/advanced/textRenderer/TextRenderer.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/metrics/HistoricalTimeData.hpp"

#include <string>

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {
namespace widgets {

void renderHistoricalTimeData(const glm::vec3& inPos,
                              const glm::vec2& inSize,
                              bool inShowFps,
                              const gero::metrics::HistoricalTimeData& inTimeData,
                              gero::graphics::opengl::advanced::IStackRenderers& inStackRenderers,
                              gero::graphics::opengl::advanced::ITextRenderer& inTextRenderer);


} // namespace widgets
} // namespace advanced
} // namespace opengl
} // namespace graphics
} // namespace gero
