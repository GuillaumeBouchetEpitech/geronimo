
#pragma once

#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/graphics/advanced-concept/textRenderer/TextRenderer.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/metrics/PerformanceProfiler.hpp"

#include <string>

namespace gero {
namespace graphics {

namespace widgets {

void renderPerformanceProfilerMetrics(const glm::vec3& inPos,
                                      const glm::vec2& inSize,
                                      const gero::metrics::PerformanceProfiler::TimeData& inTimeData,
                                      gero::graphics::StackRenderers& inStackRenderers,
                                      gero::graphics::TextRenderer& inTextRenderer);

};

} // namespace graphics
} // namespace gero
