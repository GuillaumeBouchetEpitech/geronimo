
#include "renderHistoricalTimeData.hpp"

#include "./helpers/renderTextBackground.hpp"

#include "geronimo/system/math/clamp.hpp"

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

void renderHistoricalTimeData(const glm::vec3& inPos,
                              const glm::vec2& inSize,
                              bool inShowFps,
                              const gero::metrics::HistoricalTimeData& inTimeData,
                              gero::graphics::IStackRenderers& inStackRenderers,
                              gero::graphics::ITextRenderer& inTextRenderer) {

  const glm::vec3 whiteColor = glm::vec3(1.0f, 1.0f, 1.0f) * 0.8f;
  const glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f) * 0.8f;

  const glm::vec3 borderPos = glm::vec3(inPos.x, inPos.y, inPos.z);
  const glm::vec2 borderSize = glm::vec2(inSize.x, inSize.y);

  const int32_t maxDuration = inTimeData.getMaxDuration();

  const float verticalSize = (std::ceil(float(maxDuration) / k_divider)) * k_divider;

  auto& trianglesStack = inStackRenderers.getTrianglesStack();
  auto& wireFramesStack = inStackRenderers.getWireFramesStack();

  { // background

    glm::vec4 bgColor = glm::vec4(0.0f, 0.25f, 0.0f, 1.0f);
    if (maxDuration > k_slowdownDelta * 2)
      bgColor = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);
    else if (maxDuration > k_slowdownDelta)
      bgColor = glm::vec4(0.25f, 0.25f, 0.0f, 1.0f);

    const glm::vec3 center = borderPos + glm::vec3(borderSize * 0.5f, 0);

    trianglesStack.pushQuad(center, borderSize, bgColor, inPos.z - 0.03f);
    wireFramesStack.pushRectangle(borderPos, borderSize, whiteColor, inPos.z - 0.02f);

  } // background

  //
  //

  if (inTimeData.getTotalDurations() > 0) {

    { // dividers

      if (maxDuration < k_slowdownDelta * 2) {

        for (float currDivider = k_divider; currDivider < verticalSize; currDivider += k_divider) {
          const float ratio = currDivider / verticalSize;

          wireFramesStack.pushLine(borderPos + glm::vec3(0, borderSize.y * ratio, 0.0f),
                                   borderPos + glm::vec3(borderSize.x, borderSize.y * ratio, -0.01f),
                                   whiteColor * 0.5f);
        }
      }

    } // dividers

    //
    //

    { // curve

      const float widthStep = borderSize.x / float(inTimeData.getTotalDurations());

      float prevDelta = float(inTimeData.getDurationByIndex(0));
      const float prevX = 0.0f;
      const float prevY = borderSize.y * gero::math::clamp(prevDelta / verticalSize, 0.0f, 1.0f);
      // const float prevY = borderSize.y * prevDelta / verticalSize;
      glm::vec3 prevCoord = borderPos + glm::vec3(prevX, prevY, 0.0f);
      glm::vec3 prevColor = (prevDelta < k_slowdownDelta ? whiteColor : redColor);

      for (uint32_t ii = 1; ii < inTimeData.getTotalDurations(); ++ii) {

        const float currDelta = float(inTimeData.getDurationByIndex(ii));
        const float currX = float(ii) * widthStep;
        const float currY = borderSize.y * gero::math::clamp(currDelta / verticalSize, 0.0f, 1.0f);
        // const float currY = borderSize.y * currDelta / verticalSize;
        const glm::vec3 currCoord = borderPos + glm::vec3(currX, currY, 0.0f);
        const glm::vec3 currColor = (currDelta < k_slowdownDelta ? whiteColor : redColor);

        wireFramesStack.pushLine(prevCoord, currCoord, prevColor, currColor);

        prevDelta = currDelta;
        prevCoord = currCoord;
        prevColor = currColor;
      }

    } // curve
  }

  if (inTimeData.getTotalDurations() == inTimeData.getSize()) {

    const float averageDuration = float(inTimeData.getAverageDuration());
    const float minFDuration = float(inTimeData.getMinDuration());
    const float maxFDuration = float(inTimeData.getMaxDuration());

    if (averageDuration < 999 || minFDuration < 999 || maxFDuration < 999) {

      { // text

        inTextRenderer.setOutlineColor(glm::vec4(0, 0, 0, 1))
          .setScale(k_textScale)
          .setDepth(inPos.z)
          .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left);

        if (averageDuration < 999) {
          std::stringstream sstr;
          sstr << std::fixed;
          sstr << std::setprecision(0);
          sstr << "~" << std::setw(2) << averageDuration << "ms";
          if (inShowFps) {
            const float avgFps = averageDuration > 0.0f ? 1000.0f / averageDuration : 0.0f;
            sstr << std::endl << "~" << std::setw(3) << avgFps << "fps";
          }

          std::string str = sstr.str();

          inTextRenderer.setMainColor(glm::vec4(1.0f, 1.0f, 0.75f, 1.0f))
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top);

          inTextRenderer.pushText(inPos, str);

          // wireFramesStack.pushCross(glm::vec3(inPos.x, inPos.y, +0.2f), whiteColor, 50.0f);

          // helpers::renderTextBackground(inPos.z,
          //                               glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          //                               glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
          //                               3.0f,
          //                               6.0f,
          //                               inStackRenderers,
          //                               inTextRenderer);
        }

        if (maxFDuration < 999) {
          std::stringstream sstr;
          sstr << std::fixed;
          sstr << std::setprecision(0);
          sstr << "<" << std::setw(2) << maxFDuration << "ms";
          if (inShowFps) {
            const float minFps = maxFDuration > 0.0f ? 1000.0f / maxFDuration : 0.0f;
            sstr << std::endl << "<" << std::setw(3) << minFps << "fps";
          }

          std::string str = sstr.str();

          inTextRenderer
            .setMainColor(glm::vec4(1.0f, 0.75f, 0.75f, 1.0f))
            // .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
            // .setScale(k_textScale)
            // .setDepth(inPos.z)
            // .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
            .setVerticalTextAlign(TextRenderer::VerticalTextAlign::center);

          const glm::vec2 textPos = glm::vec2(inPos.x + inSize.x, inPos.y + inSize.y);

          // wireFramesStack.pushCross(glm::vec3(textPos, +0.2f), whiteColor, 50.0f);

          inTextRenderer.pushText(textPos, str);

          // helpers::renderTextBackground(inPos.z,
          //                               glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          //                               glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
          //                               3.0f,
          //                               6.0f,
          //                               inStackRenderers,
          //                               inTextRenderer);
        }

        if (minFDuration < 999) {
          std::stringstream sstr;
          sstr << std::fixed;
          sstr << std::setprecision(0);
          sstr << ">" << std::setw(2) << minFDuration << "ms";
          if (inShowFps) {
            const float maxFps = minFDuration > 0.0f ? 1000.0f / minFDuration : 0.0f;
            sstr << std::endl << ">" << std::setw(3) << maxFps << "fps";
          }

          std::string str = sstr.str();

          inTextRenderer.setMainColor(glm::vec4(0.75f, 1.0f, 0.75f, 1.0f))
            // .setTextAlign(TextRenderer::TextAlign::left)
            // .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
            // .setVerticalTextAlign(TextRenderer::VerticalTextAlign::center)
            ;

          const glm::vec2 textPos = glm::vec2(inPos.x + inSize.x, inPos.y);

          // wireFramesStack.pushCross(glm::vec3(textPos, +0.2f), whiteColor, 50.0f);

          inTextRenderer.pushText(textPos, str);

          // helpers::renderTextBackground(inPos.z,
          //                               glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
          //                               glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
          //                               3.0f,
          //                               6.0f,
          //                               inStackRenderers,
          //                               inTextRenderer);
        }

      } // text
    }
  }
}

} // namespace widgets

} // namespace graphics
} // namespace gero
