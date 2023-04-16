
#include "renderPerformanceProfilerMetrics.hpp"

#include "./helpers/renderTextBackground.hpp"

#include <iomanip>
#include <sstream>

namespace gero {
namespace graphics {

namespace {

constexpr int32_t k_slowdownDelta = 33;
constexpr float k_divider = 5.0f; // 5ms

constexpr float k_textScale = 14.0f;

} // namespace

namespace widgets {

void renderPerformanceProfilerMetrics(const glm::vec3& inPos,
                                      const glm::vec2& inSize,
                                      const gero::metrics::PerformanceProfiler::TimeData& inTimeData,
                                      gero::graphics::StackRenderers& inStackRenderers,
                                      gero::graphics::TextRenderer& inTextRenderer) {

  const glm::vec3 whiteColor = glm::vec3(1.0f, 1.0f, 1.0f) * 0.8f;
  const glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f) * 0.8f;

  const glm::vec3 borderPos = glm::vec3(inPos.x, inPos.y + k_textScale * 1.1f, inPos.z);
  const glm::vec2 borderSize = glm::vec2(inSize.x, inSize.y - k_textScale * 1.1f);

  const int32_t maxDuration = inTimeData.getMaxDuration();

  const float verticalSize = (std::ceil(float(maxDuration) / k_divider)) * k_divider;

  { // background

    glm::vec4 bgColor = glm::vec4(0.0f, 0.25f, 0.0f, 1.0f);
    if (maxDuration > k_slowdownDelta * 2)
      bgColor = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);
    else if (maxDuration > k_slowdownDelta)
      bgColor = glm::vec4(0.25f, 0.25f, 0.0f, 1.0f);

    const glm::vec3 center = borderPos + glm::vec3(borderSize * 0.5f, 0);

    inStackRenderers.getTrianglesStack().pushQuad(center, borderSize, bgColor, inPos.z - 0.03f);
    inStackRenderers.getWireFramesStack().pushRectangle(borderPos, borderSize, whiteColor, inPos.z - 0.02f);

  } // background

  //
  //

  { // dividers

    if (maxDuration < k_slowdownDelta * 2) {

      for (float currDivider = k_divider; currDivider < verticalSize; currDivider += k_divider) {
        const float ratio = currDivider / verticalSize;

        inStackRenderers.getWireFramesStack().pushLine(borderPos + glm::vec3(0, borderSize.y * ratio, 0.0f),
                                                       borderPos +
                                                         glm::vec3(borderSize.x, borderSize.y * ratio, -0.01f),
                                                       whiteColor * 0.5f);
      }
    }

  } // dividers

  //
  //

  {

    const float widthStep = borderSize.x / float(inTimeData.getTotalDurations());

    float prevDelta = float(inTimeData.getDurationByIndex(0));
    float prevHeight = borderSize.y * prevDelta / verticalSize;
    glm::vec3 prevCoord = borderPos + glm::vec3(0.0f, prevHeight, 0.0f);

    for (uint32_t ii = 1; ii < inTimeData.getTotalDurations(); ++ii) {

      const float currDelta = float(inTimeData.getDurationByIndex(ii));
      const float currHeight = borderSize.y * currDelta / verticalSize;
      const glm::vec3 currCoord = borderPos + glm::vec3(float(ii) * widthStep, currHeight, 0.0f);

      inStackRenderers.getWireFramesStack().pushLine(prevCoord,
                                                     currCoord,
                                                     prevDelta < k_slowdownDelta ? whiteColor : redColor,
                                                     currDelta < k_slowdownDelta ? whiteColor : redColor);

      prevDelta = currDelta;
      prevHeight = currHeight;
      prevCoord = currCoord;
    }
  }

  {

    const float averageDuration = float(inTimeData.getAverageDuration());
    const float minFDuration = float(inTimeData.getMinDuration());
    const float maxFDuration = float(inTimeData.getMaxDuration());
    const float fpsValue = averageDuration > 0.0f ? 1000.0f / averageDuration : 0.0f;
    const float minValue = minFDuration > 0.0f ? 1000.0f / minFDuration : 0.0f;
    const float maxValue = maxFDuration > 0.0f ? 1000.0f / maxFDuration : 0.0f;

    std::stringstream sstr;
    sstr << std::setw(3);
    sstr << std::fixed;
    sstr << std::setprecision(0);
    sstr << fpsValue << "fps" << std::endl;
    sstr << maxValue << "_" << minValue << std::endl;

    std::string str = sstr.str();

    inTextRenderer.setMainColor(glm::vec4(1, 1, 1, 1))
      .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
      .setScale(k_textScale)
      .setDepth(inPos.z)
      .setTextAlign(TextRenderer::TextAlign::left);

    inTextRenderer.pushText(inPos, str);

    helpers::renderTextBackground(inPos.z,
                                  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                  glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                                  3.0f,
                                  6.0f,
                                  inStackRenderers,
                                  inTextRenderer);
  }
}

} // namespace widgets

} // namespace graphics
} // namespace gero
