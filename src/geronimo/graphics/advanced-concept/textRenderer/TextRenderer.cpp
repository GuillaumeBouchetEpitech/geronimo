
#include "TextRenderer.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/Image.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/asValue.hpp"
#include "geronimo/system/parser-utils/string_view_regexp.hpp"

namespace gero {
namespace graphics {

namespace {

const glm::vec2 k_letterSize = {16, 16};
const float k_letterAspectRatio = k_letterSize.x / k_letterSize.y;
const glm::vec2 k_letterScale = {k_letterAspectRatio, 1.0f};

} // namespace

void TextRenderer::initialize(const std::string& inRootPath) {

  _logic.allLinesWidth.reserve(64);
  _logic.latestMessageRectangles.reserve(64);

  std::string currSrcFolder = __FILE__;
  currSrcFolder = currSrcFolder.substr(0, currSrcFolder.rfind("src/") + 4);
  const std::string k_rootPath = inRootPath + "/geronimo/graphics/advanced-concept/textRenderer/assets/";

  gero::graphics::ShaderProgramBuilder shaderProgramBuilder;
  shaderProgramBuilder.reset()
    .setVertexFilename(k_rootPath + "shaders/textRenderer.glsl.vert")
    .setFragmentFilename(k_rootPath + "shaders/textRenderer.glsl.frag")
    .addAttribute("a_position")
    .addAttribute("a_texCoord")
    .addAttribute("a_offsetPosition")
    .addAttribute("a_offsetTexCoord")
    .addAttribute("a_offsetColor")
    .addAttribute("a_offsetScale")
    .addUniform("u_composedMatrix")
    .addUniform("u_texture");

  _graphic.shader = std::make_shared<gero::graphics::ShaderProgram>(shaderProgramBuilder.getDefinition());

  gero::graphics::Image tmpImg;
  tmpImg.loadFromFile(k_rootPath + "textures/ascii_font.png");

  _graphic.texture = std::make_shared<gero::graphics::Texture>();
  _graphic.texture->setFromImage(
    tmpImg, gero::graphics::Texture::Quality::pixelated, gero::graphics::Texture::Pattern::clamped);

  const glm::vec2 textureSize = glm::vec2(_graphic.texture->getSize());

  const glm::vec2 texCoord = 1.0f / textureSize * k_letterSize;

  {

    const glm::vec2 minPosCoord = k_letterScale * -0.5f;
    const glm::vec2 maxPosCoord = k_letterScale * +0.5f;

    const glm::vec2 minTexCoord = {0.0f, 0.0f};
    const glm::vec2 maxTexCoord = {texCoord.x, texCoord.y};

    struct VertexData {
      glm::vec2 position;
      glm::vec2 texCoord;
    };

    const std::array<VertexData, 4> vertices{{{{maxPosCoord.x, minPosCoord.y}, {maxTexCoord.x, maxTexCoord.y}},
                                              {{minPosCoord.x, minPosCoord.y}, {minTexCoord.x, maxTexCoord.y}},
                                              {{maxPosCoord.x, maxPosCoord.y}, {maxTexCoord.x, minTexCoord.y}},
                                              {{minPosCoord.x, maxPosCoord.y}, {minTexCoord.x, minTexCoord.y}}}};

    std::array<std::size_t, 6> indices{{1, 0, 2, 1, 3, 2}};

    std::vector<VertexData> letterVertices;
    letterVertices.reserve(indices.size()); // pre-allocate

    for (std::size_t index : indices)
      letterVertices.push_back(vertices.at(index));

    gero::graphics::GeometryBuilder geometryBuilder;
    geometryBuilder.reset()
      .setShader(*_graphic.shader)
      .setPrimitiveType(gero::graphics::Geometry::PrimitiveType::triangles)
      .addVbo()
      .addVboAttribute("a_position", gero::graphics::Geometry::AttrType::Vec2f)
      .addVboAttribute("a_texCoord", gero::graphics::Geometry::AttrType::Vec2f)
      .addVbo()
      .setVboAsInstanced()
      .addVboAttribute("a_offsetPosition", gero::graphics::Geometry::AttrType::Vec3f)
      .addVboAttribute("a_offsetTexCoord", gero::graphics::Geometry::AttrType::Vec2f)
      .addVboAttribute("a_offsetColor", gero::graphics::Geometry::AttrType::Vec4f)
      .addVboAttribute("a_offsetScale", gero::graphics::Geometry::AttrType::Float);

    _graphic.geometry.initialize(*_graphic.shader, geometryBuilder.getDefinition());
    _graphic.geometry.allocateBuffer(0, letterVertices);
    _graphic.geometry.setPrimitiveCount(uint32_t(letterVertices.size()));
  }

  constexpr std::size_t preAllocatedSize = 1024;

  _graphic.vertices.reserve(preAllocatedSize * 8); // <= pre-allocate
  _graphic.geometry.preAllocateBufferFromCapacity(1, _graphic.vertices);

  _logic.charactersTexCoordMap = {
    {' ', {0 * texCoord.x, 0 * texCoord.y}},   {'!', {1 * texCoord.x, 0 * texCoord.y}},
    {'\"', {2 * texCoord.x, 0 * texCoord.y}},  {'#', {3 * texCoord.x, 0 * texCoord.y}},
    {'$', {4 * texCoord.x, 0 * texCoord.y}},   {'%', {5 * texCoord.x, 0 * texCoord.y}},
    {'&', {6 * texCoord.x, 0 * texCoord.y}},   {'\'', {7 * texCoord.x, 0 * texCoord.y}},
    {'(', {8 * texCoord.x, 0 * texCoord.y}},   {')', {9 * texCoord.x, 0 * texCoord.y}},
    {'*', {10 * texCoord.x, 0 * texCoord.y}},  {'+', {11 * texCoord.x, 0 * texCoord.y}},
    {',', {12 * texCoord.x, 0 * texCoord.y}},  {'-', {13 * texCoord.x, 0 * texCoord.y}},
    {'.', {14 * texCoord.x, 0 * texCoord.y}},  {'/', {15 * texCoord.x, 0 * texCoord.y}},

    {'0', {0 * texCoord.x, 1 * texCoord.y}},   {'1', {1 * texCoord.x, 1 * texCoord.y}},
    {'2', {2 * texCoord.x, 1 * texCoord.y}},   {'3', {3 * texCoord.x, 1 * texCoord.y}},
    {'4', {4 * texCoord.x, 1 * texCoord.y}},   {'5', {5 * texCoord.x, 1 * texCoord.y}},
    {'6', {6 * texCoord.x, 1 * texCoord.y}},   {'7', {7 * texCoord.x, 1 * texCoord.y}},
    {'8', {8 * texCoord.x, 1 * texCoord.y}},   {'9', {9 * texCoord.x, 1 * texCoord.y}},
    {':', {10 * texCoord.x, 1 * texCoord.y}},  {';', {11 * texCoord.x, 1 * texCoord.y}},
    {'<', {12 * texCoord.x, 1 * texCoord.y}},  {'=', {13 * texCoord.x, 1 * texCoord.y}},
    {'>', {14 * texCoord.x, 1 * texCoord.y}},  {'?', {15 * texCoord.x, 1 * texCoord.y}},

    {'@', {0 * texCoord.x, 2 * texCoord.y}},   {'A', {1 * texCoord.x, 2 * texCoord.y}},
    {'B', {2 * texCoord.x, 2 * texCoord.y}},   {'C', {3 * texCoord.x, 2 * texCoord.y}},
    {'D', {4 * texCoord.x, 2 * texCoord.y}},   {'E', {5 * texCoord.x, 2 * texCoord.y}},
    {'F', {6 * texCoord.x, 2 * texCoord.y}},   {'G', {7 * texCoord.x, 2 * texCoord.y}},
    {'H', {8 * texCoord.x, 2 * texCoord.y}},   {'I', {9 * texCoord.x, 2 * texCoord.y}},
    {'J', {10 * texCoord.x, 2 * texCoord.y}},  {'K', {11 * texCoord.x, 2 * texCoord.y}},
    {'L', {12 * texCoord.x, 2 * texCoord.y}},  {'M', {13 * texCoord.x, 2 * texCoord.y}},
    {'N', {14 * texCoord.x, 2 * texCoord.y}},  {'O', {15 * texCoord.x, 2 * texCoord.y}},

    {'P', {0 * texCoord.x, 3 * texCoord.y}},   {'Q', {1 * texCoord.x, 3 * texCoord.y}},
    {'R', {2 * texCoord.x, 3 * texCoord.y}},   {'S', {3 * texCoord.x, 3 * texCoord.y}},
    {'T', {4 * texCoord.x, 3 * texCoord.y}},   {'U', {5 * texCoord.x, 3 * texCoord.y}},
    {'V', {6 * texCoord.x, 3 * texCoord.y}},   {'W', {7 * texCoord.x, 3 * texCoord.y}},
    {'X', {8 * texCoord.x, 3 * texCoord.y}},   {'Y', {9 * texCoord.x, 3 * texCoord.y}},
    {'Z', {10 * texCoord.x, 3 * texCoord.y}},  {'[', {11 * texCoord.x, 3 * texCoord.y}},
    {'\\', {12 * texCoord.x, 3 * texCoord.y}}, {']', {13 * texCoord.x, 3 * texCoord.y}},
    {'^', {14 * texCoord.x, 3 * texCoord.y}},  {'_', {15 * texCoord.x, 3 * texCoord.y}},

    {'`', {0 * texCoord.x, 4 * texCoord.y}},   {'a', {1 * texCoord.x, 4 * texCoord.y}},
    {'b', {2 * texCoord.x, 4 * texCoord.y}},   {'c', {3 * texCoord.x, 4 * texCoord.y}},
    {'d', {4 * texCoord.x, 4 * texCoord.y}},   {'e', {5 * texCoord.x, 4 * texCoord.y}},
    {'f', {6 * texCoord.x, 4 * texCoord.y}},   {'g', {7 * texCoord.x, 4 * texCoord.y}},
    {'h', {8 * texCoord.x, 4 * texCoord.y}},   {'i', {9 * texCoord.x, 4 * texCoord.y}},
    {'j', {10 * texCoord.x, 4 * texCoord.y}},  {'k', {11 * texCoord.x, 4 * texCoord.y}},
    {'l', {12 * texCoord.x, 4 * texCoord.y}},  {'m', {13 * texCoord.x, 4 * texCoord.y}},
    {'n', {14 * texCoord.x, 4 * texCoord.y}},  {'o', {15 * texCoord.x, 4 * texCoord.y}},

    {'p', {0 * texCoord.x, 5 * texCoord.y}},   {'q', {1 * texCoord.x, 5 * texCoord.y}},
    {'r', {2 * texCoord.x, 5 * texCoord.y}},   {'s', {3 * texCoord.x, 5 * texCoord.y}},
    {'t', {4 * texCoord.x, 5 * texCoord.y}},   {'u', {5 * texCoord.x, 5 * texCoord.y}},
    {'v', {6 * texCoord.x, 5 * texCoord.y}},   {'w', {7 * texCoord.x, 5 * texCoord.y}},
    {'x', {8 * texCoord.x, 5 * texCoord.y}},   {'y', {9 * texCoord.x, 5 * texCoord.y}},
    {'z', {10 * texCoord.x, 5 * texCoord.y}},  {'{', {11 * texCoord.x, 5 * texCoord.y}},
    {'|', {12 * texCoord.x, 5 * texCoord.y}},  {'}', {13 * texCoord.x, 5 * texCoord.y}},
    {'~', {14 * texCoord.x, 5 * texCoord.y}},

  };

  _logic.stateRegexp = std::regex(R"(\$\{(\d+)\})");
}

TextRenderer& TextRenderer::setMatricesData(const gero::graphics::Camera::MatricesData& matricesData) {
  _graphic.matricesData = matricesData;
  return *this;
}

TextRenderer& TextRenderer::setMainColor(const glm::vec4& inColor) {
  _logic.mainColor = inColor;
  return *this;
}

TextRenderer& TextRenderer::setOutlineColor(const glm::vec4& inColor) {
  _logic.outlineColor = inColor;
  return *this;
}

TextRenderer& TextRenderer::setScale(float inScale) {
  _logic.scale = inScale;
  return *this;
}

TextRenderer& TextRenderer::setDepth(float inDepth) {
  _logic.depth = inDepth;
  return *this;
}

TextRenderer& TextRenderer::setTextAlign(TextAlign inTextAlign) {
  _logic.textAlign = inTextAlign;
  return *this;
}

//

TextRenderer& TextRenderer::pushText(const glm::vec2& inPosition, const std::string_view inMessage) {
  _pushText(inPosition, inMessage, nullptr, 0);

  return *this;
}

void TextRenderer::_pushText(const glm::vec2& inPosition,
                             const std::string_view inMessage,
                             const State* inPStates,
                             std::size_t inTotalStates) {

  const glm::vec2 charSize = k_letterScale * _logic.scale;
  const glm::vec2 charHSize = charSize * 0.5f;

  _logic.latestMessageRectangles.clear();

  if (_logic.textAlign != TextAlign::left) {

    _logic.allLinesWidth.clear();
    _logic.allLinesWidth.push_back(0);

    for (std::size_t index = 0; index < inMessage.size();) {
      const char currCharacter = inMessage.at(index);

      if (currCharacter == '$') {

        std::string_view subStr = &inMessage.at(index);

        gero::parserUtils::string_view_regexp::match match;
        if (gero::parserUtils::string_view_regexp::regex_search(subStr.data(), match, _logic.stateRegexp)) {
          index += std::size_t(match.length(0));
          continue;
        }
      }

      if (currCharacter == '\n')
        _logic.allLinesWidth.push_back(0);
      else
        _logic.allLinesWidth.back() += 1;

      ++index;
    }
  }

  std::size_t currentLineIndex = 0;
  const glm::vec2 basePos = inPosition + charHSize;
  glm::vec3 currPos = glm::vec3(basePos, _logic.depth); // TextAlign::left

  if (_logic.textAlign != TextAlign::left) {
    const uint32_t currenLineSize = _logic.allLinesWidth.at(currentLineIndex);

    if (_logic.textAlign == TextAlign::center) {
      currPos.x -= float(currenLineSize) * charHSize.x;
    } else if (_logic.textAlign == TextAlign::right) {
      currPos.x -= float(currenLineSize) * charSize.x;
    }
  }

  MessageRectangle currentRectangle;
  currentRectangle.pos = glm::vec2(currPos) - charHSize;
  currentRectangle.size = {0.0f, charSize.y};

  glm::vec4 currColor = _logic.mainColor;
  glm::vec4 currOutline = _logic.outlineColor;

  //

  for (std::size_t index = 0; index < inMessage.size();) {
    const char currCharacter = inMessage.at(index);

    if (currCharacter == '$') {

      std::string_view subStr = &inMessage.at(index);

      gero::parserUtils::string_view_regexp::match match;
      if (gero::parserUtils::string_view_regexp::regex_search(subStr.data(), match, _logic.stateRegexp)) {

        const gero::parserUtils::string_view_regexp::sub_match sub_match = match[1];
        const std::string_view value = gero::parserUtils::string_view_regexp::get_string_view(sub_match);

        const int32_t intVal = std::atoi(value.data());

        if (intVal >= 0 && intVal < int32_t(inTotalStates)) {
          const State& currState = inPStates[intVal];

          if (currState.color) {
            currColor = currState.color.value();
          }
          if (currState.outline) {
            currOutline = currState.outline.value();
          }
        }

        index += std::size_t(match.length(0));
        continue;
      }
    }

    if (currCharacter == '\n') {
      currentLineIndex += 1;

      currPos.x = basePos.x; // TextAlign::left

      if (_logic.textAlign != TextAlign::left) {
        const uint32_t currenLineSize = _logic.allLinesWidth.at(currentLineIndex);

        if (_logic.textAlign == TextAlign::center) {
          currPos.x -= float(currenLineSize) * charHSize.x;
        } else if (_logic.textAlign == TextAlign::right) {
          currPos.x -= float(currenLineSize) * charSize.x;
        }
      }

      currPos.y -= charSize.y;

      if (currentRectangle.size.x > 0)
        _logic.latestMessageRectangles.push_back(currentRectangle);

      currentRectangle.pos = glm::vec2(currPos) - charHSize;
      currentRectangle.size = {0.0f, charSize.y};

      ++index;
      continue;
    }

    auto it = _logic.charactersTexCoordMap.find(currCharacter);
    if (it == _logic.charactersTexCoordMap.end())
      D_THROW(std::runtime_error, "fail to find a letter, letter=" << currCharacter);

    const auto& texCoord = it->second;

    _pushSingleCharacter(currPos, texCoord, _logic.scale, currColor, currOutline);

    currPos.x += charSize.x;
    currentRectangle.size.x += _logic.scale;
    ++index;
  }

  if (currentRectangle.size.x > 0)
    _logic.latestMessageRectangles.push_back(currentRectangle);
}

void TextRenderer::_pushSingleCharacter(const glm::vec3& inPosition,
                                        const glm::vec2& inTexCoord,
                                        float inScale,
                                        const glm::vec4& inColor,
                                        const glm::vec4& inOutlineColor) {

  { // background

    const glm::vec2 tmpScale = glm::vec2(inScale) / k_letterSize;

    const glm::vec4 currColor = inOutlineColor;
    constexpr int k_range = 1;
    glm::vec3 offsetPos = glm::vec3(0, 0, -0.01f);

    for (int stepY = -k_range; stepY <= +k_range; stepY += k_range) {

      offsetPos.y = tmpScale.y * float(stepY);

      for (int stepX = -k_range; stepX <= +k_range; stepX += k_range) {
        if (stepX == 0 && stepY == 0)
          continue;

        offsetPos.x = tmpScale.x * float(stepX);

        _graphic.vertices.push_back({inPosition + offsetPos, inTexCoord, currColor, inScale});
      }
    }

  } // background

  _graphic.vertices.push_back({inPosition, inTexCoord, inColor, inScale});
}

const std::vector<TextRenderer::MessageRectangle>& TextRenderer::getLatestTextRectangles() const {
  return _logic.latestMessageRectangles;
}

TextRenderer& TextRenderer::clear() {
  _graphic.vertices.clear();
  return *this;
}

TextRenderer& TextRenderer::render() {
  if (_graphic.vertices.empty())
    return *this;

  if (!_graphic.shader)
    D_THROW(std::runtime_error, "shader not setup");

  _graphic.shader->bind();
  _graphic.shader->setUniform("u_composedMatrix", _graphic.matricesData.composed);

  _graphic.texture->bind();

  _graphic.geometry.updateOrAllocateBuffer(1, _graphic.vertices);
  _graphic.geometry.setInstancedCount(uint32_t(_graphic.vertices.size()));

  _graphic.geometry.render();

  _graphic.vertices.clear();

  return *this;
}

} // namespace graphics
} // namespace gero
