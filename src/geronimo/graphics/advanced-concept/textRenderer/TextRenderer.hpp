
#pragma once

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/Texture.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <memory>
#include <regex>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace gero {
namespace graphics {

struct TextRenderer : public gero::NonCopyable {

public:
  enum class HorizontalTextAlign {
    left = 0,
    center,
    right,
  };
  enum class VerticalTextAlign {
    top = 0,
    center,
    bottom,
  };

public:
  struct State {
    std::optional<glm::vec4> color;
    std::optional<glm::vec4> outline;

    State(std::optional<glm::vec4> inColor = std::nullopt, std::optional<glm::vec4> inOutline = std::nullopt)
      : color(inColor), outline(inOutline) {}
  };

public:
  struct MessageRectangle {
    glm::vec2 pos;
    glm::vec2 size;
  };

private:
  struct LetterOffset {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec4 color;
    float scale;
  };

private:
  struct Graphic {
    std::vector<LetterOffset> vertices;
    std::shared_ptr<gero::graphics::ShaderProgram> shader;
    std::shared_ptr<gero::graphics::Texture> texture;
    gero::graphics::Geometry geometry;
    gero::graphics::ICamera::MatricesData matricesData;
  } _graphic;

  struct Logic {
    std::unordered_map<char, glm::vec2> charactersTexCoordMap;
    std::vector<uint32_t> allLinesWidth;
    std::vector<MessageRectangle> latestMessageRectangles;
    std::regex stateRegexp;

    glm::vec4 mainColor;
    glm::vec4 outlineColor;
    float scale;
    float depth;
    HorizontalTextAlign horizontalTextAlign = HorizontalTextAlign::left;
    VerticalTextAlign verticalTextAlign = VerticalTextAlign::top;

  } _logic;

public:
  void initialize(const std::string& inRootPath);

public:
  TextRenderer& setMatricesData(const gero::graphics::ICamera::MatricesData& matricesData);

public:
  TextRenderer& setMainColor(const glm::vec4& inColor);
  TextRenderer& setOutlineColor(const glm::vec4& inColor);
  TextRenderer& setScale(float inScale);
  TextRenderer& setDepth(float inDepth);
  TextRenderer& setHorizontalTextAlign(HorizontalTextAlign inHorizontalTextAlign);
  TextRenderer& setVerticalTextAlign(VerticalTextAlign inVerticalTextAlign);

public:
  TextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage);

  template <typename... Args>
  TextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage, Args... args) {

    std::array<State, sizeof...(args)> allStates{{args...}};

    _pushText(inPosition, inMessage, allStates.data(), allStates.size());

    return *this;
  }

private:
  void _pushText(const glm::vec2& inPosition,
                 const std::string_view inMessage,
                 const State* pStates,
                 std::size_t totalStates);

private:
  void _pushSingleCharacter(const glm::vec3& inPosition,
                            const glm::vec2& inTexCoord,
                            float inScale,
                            const glm::vec4& inColor,
                            const glm::vec4& inOutlineColor);

public:
  const std::vector<MessageRectangle>& getLatestTextRectangles() const;

public:
  TextRenderer& clear();
  TextRenderer& render();
};

} // namespace graphics
} // namespace gero
