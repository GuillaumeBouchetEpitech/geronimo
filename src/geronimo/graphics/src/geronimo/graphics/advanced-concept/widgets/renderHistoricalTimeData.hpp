
#pragma once

#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/advanced-concept/textRenderer/TextRenderer.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/metrics/HistoricalTimeData.hpp"

#include <string>

namespace gero {
namespace graphics {

namespace widgets {

void renderHistoricalTimeData(const glm::vec3& inPos,
                              const glm::vec2& inSize,
                              bool inShowFps,
                              const gero::metrics::HistoricalTimeData& inTimeData,
                              gero::graphics::IStackRenderers& inStackRenderers,
                              gero::graphics::ITextRenderer& inTextRenderer);

};

} // namespace graphics
} // namespace gero
