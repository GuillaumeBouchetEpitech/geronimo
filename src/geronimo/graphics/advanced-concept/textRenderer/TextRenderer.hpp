
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

struct ITextRenderer {

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

  struct State {
    std::optional<glm::vec4> color;
    std::optional<glm::vec4> outline;

    State(std::optional<glm::vec4> inColor = std::nullopt, std::optional<glm::vec4> inOutline = std::nullopt)
      : color(inColor), outline(inOutline) {}
  };

  struct MessageRectangle {
    glm::vec2 pos;
    glm::vec2 size;
  };


public:
  ~ITextRenderer() = default;

public:
  virtual ITextRenderer& setMatricesData(const gero::graphics::ICamera::MatricesData& matricesData) = 0;

public:
  virtual ITextRenderer& setMainColor(const glm::vec4& inColor) = 0;
  virtual ITextRenderer& setOutlineColor(const glm::vec4& inColor) = 0;
  virtual ITextRenderer& setScale(float inScale) = 0;
  virtual ITextRenderer& setDepth(float inDepth) = 0;
  virtual ITextRenderer& setHorizontalTextAlign(HorizontalTextAlign inHorizontalTextAlign) = 0;
  virtual ITextRenderer& setVerticalTextAlign(VerticalTextAlign inVerticalTextAlign) = 0;

public:
  virtual ITextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage) = 0;

  template <typename... Args>
  ITextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage, Args... args) {

    std::array<State, sizeof...(args)> allStates{{args...}};

    _pushText(inPosition, inMessage, allStates.data(), allStates.size());

    return *this;
  }

public:
  virtual const std::vector<MessageRectangle>& getLatestTextRectangles() const = 0;

public:
  virtual ITextRenderer& clear() = 0;
  virtual ITextRenderer& render() = 0;

protected:
  virtual void
  _pushText(const glm::vec2& inPosition,
    const std::string_view inMessage,
    const State* pStates,
    std::size_t totalStates) = 0;


};

struct TextRenderer : public ITextRenderer, public gero::NonCopyable {

public:
  using HorizontalTextAlign = ITextRenderer::HorizontalTextAlign;
  using VerticalTextAlign = ITextRenderer::VerticalTextAlign;
  using State = ITextRenderer::State;
  using MessageRectangle = ITextRenderer::MessageRectangle;

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
  ITextRenderer& setMatricesData(const gero::graphics::ICamera::MatricesData& matricesData) override;

public:
  ITextRenderer& setMainColor(const glm::vec4& inColor) override;
  ITextRenderer& setOutlineColor(const glm::vec4& inColor) override;
  ITextRenderer& setScale(float inScale) override;
  ITextRenderer& setDepth(float inDepth) override;
  ITextRenderer& setHorizontalTextAlign(HorizontalTextAlign inHorizontalTextAlign) override;
  ITextRenderer& setVerticalTextAlign(VerticalTextAlign inVerticalTextAlign) override;

public:
  ITextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage) override;

  template <typename... Args>
  ITextRenderer& pushText(const glm::vec2& inPosition, const std::string_view inMessage, Args... args) {

    std::array<State, sizeof...(args)> allStates{{args...}};

    _pushText(inPosition, inMessage, allStates.data(), allStates.size());

    return *this;
  }

private:
  void _pushText(const glm::vec2& inPosition,
                 const std::string_view inMessage,
                 const State* pStates,
                 std::size_t totalStates) override;

private:
  void _pushSingleCharacter(const glm::vec3& inPosition,
                            const glm::vec2& inTexCoord,
                            float inScale,
                            const glm::vec4& inColor,
                            const glm::vec4& inOutlineColor);

public:
  const std::vector<MessageRectangle>& getLatestTextRectangles() const override;

public:
  ITextRenderer& clear() override;
  ITextRenderer& render() override;

};

} // namespace graphics
} // namespace gero
