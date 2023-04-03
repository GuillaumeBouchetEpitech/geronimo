
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include <string>

namespace widgets {

  void renderPerformanceProfilerMetrics(
    const glm::vec3& inPos,
    const glm::vec2& inSize,
    const std::string& inMetricsName
  );

};