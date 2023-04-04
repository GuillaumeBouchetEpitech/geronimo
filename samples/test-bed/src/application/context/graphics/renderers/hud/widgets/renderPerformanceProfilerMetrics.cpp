
#include "renderPerformanceProfilerMetrics.hpp"

#include "application/context/Context.hpp"
#include "application/context/graphics/renderers/hud/helpers/renderTextBackground.hpp"

#include <iomanip>
#include <sstream>

namespace {

constexpr int32_t k_slowdownDelta = 33;
constexpr float k_divider = 5.0f; // 5ms

constexpr float k_textScale = 14.0f;

} // namespace

namespace widgets {

void renderPerformanceProfilerMetrics(const glm::vec3& inPos,
                                      const glm::vec2& inSize,
                                      const std::string& inMetricsName) {

  auto& context = Context::get();

  auto& perfProfiler = context.logic.performanceProfiler;
  const auto& timeDataMap = perfProfiler.getTimeDataMap();
  const auto it = timeDataMap.find(inMetricsName);
  if (it == timeDataMap.end())
    return;
  const auto& profileData = it->second;

  auto& graphic = context.graphic;
  auto& stackRenderers = graphic.hud.stackRenderers;

  const glm::vec3 whiteColor = glm::vec3(1.0f, 1.0f, 1.0f) * 0.8f;
  const glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f) * 0.8f;

  const glm::vec3 borderPos = glm::vec3(inPos.x, inPos.y + k_textScale * 1.1f, inPos.z);
  const glm::vec2 borderSize = glm::vec2(inSize.x, inSize.y - k_textScale * 1.1f);

  const int32_t maxDuration = profileData.getMaxDuration();

  const float verticalSize = (std::ceil(float(maxDuration) / k_divider)) * k_divider;

  { // background

    glm::vec4 bgColor = glm::vec4(0.0f, 0.25f, 0.0f, 1.0f);
    if (maxDuration > k_slowdownDelta * 2)
      bgColor = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);
    else if (maxDuration > k_slowdownDelta)
      bgColor = glm::vec4(0.25f, 0.25f, 0.0f, 1.0f);

    const glm::vec3 center = borderPos + glm::vec3(borderSize * 0.5f, 0);

    stackRenderers.getTrianglesStack().pushQuad(center, borderSize, bgColor, inPos.z - 0.03f);
    stackRenderers.getWireFramesStack().pushRectangle(borderPos, borderSize, whiteColor, inPos.z - 0.02f);

  } // background

  //
  //

  { // dividers

    if (maxDuration < k_slowdownDelta * 2) {

      for (float currDivider = k_divider; currDivider < verticalSize; currDivider += k_divider) {
        const float ratio = currDivider / verticalSize;

        stackRenderers.getWireFramesStack().pushLine(borderPos + glm::vec3(0, borderSize.y * ratio, 0.0f),
                                                     borderPos + glm::vec3(borderSize.x, borderSize.y * ratio, -0.01f),
                                                     whiteColor * 0.5f);
      }
    }

  } // dividers

  //
  //

  {

    const float widthStep = borderSize.x / float(profileData.getTotalDurations());

    float prevDelta = float(profileData.getDurationByIndex(0));
    float prevHeight = borderSize.y * prevDelta / verticalSize;
    glm::vec3 prevCoord = borderPos + glm::vec3(0.0f, prevHeight, 0.0f);

    for (uint32_t ii = 1; ii < profileData.getTotalDurations(); ++ii) {

      const float currDelta = float(profileData.getDurationByIndex(ii));
      const float currHeight = borderSize.y * currDelta / verticalSize;
      const glm::vec3 currCoord = borderPos + glm::vec3(float(ii) * widthStep, currHeight, 0.0f);

      stackRenderers.getWireFramesStack().pushLine(prevCoord,
                                                   currCoord,
                                                   prevDelta < k_slowdownDelta ? whiteColor : redColor,
                                                   currDelta < k_slowdownDelta ? whiteColor : redColor);

      prevDelta = currDelta;
      prevHeight = currHeight;
      prevCoord = currCoord;
    }
  }

  {

    auto& textRenderer = graphic.hud.textRenderer;

    const float averageDuration = float(profileData.getAverageDuration());
    const float minFDuration = float(profileData.getMinDuration());
    const float maxFDuration = float(profileData.getMaxDuration());
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

    textRenderer.setMainColor(glm::vec4(1, 1, 1, 1))
      .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
      .setScale(k_textScale)
      .setDepth(inPos.z)
      .setTextAlign(TextRenderer::TextAlign::left);

    textRenderer.pushText(inPos, str);

    helpers::renderTextBackground(
      inPos.z, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 3.0f, 6.0f);
  }
}

} // namespace widgets
