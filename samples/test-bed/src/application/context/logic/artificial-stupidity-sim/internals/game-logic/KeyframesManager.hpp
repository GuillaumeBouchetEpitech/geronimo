
#pragma once


#include "graphics/textures-manager/TexturesManager.hpp"
#include "graphics/sprite-stack-renderer/SpriteStackRenderer.hpp"

#include "geronimo/graphics/advanced-concept/stackRenderers/StackRenderers.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include <unordered_map>

struct RectData {
  glm::vec2 bottomLeft;
  glm::vec2 topRight;

  RectData flipX(float originX) const;
  bool testOverlap(const RectData& other) const;
  void render(float thickness, float depth, const glm::vec4& color, gero::graphics::IStackRenderers &stackRenderer) const;
};

struct KeyframeData {
  std::string alias;
  glm::vec2 origin;
  RectData rect;
  std::vector<RectData> healthColliders;
  std::vector<RectData> attackColliders;
};

class KeyframesManager
{
public:
  KeyframesManager();
  ~KeyframesManager() = default;

private:
  void _loadJsonFile(const std::string_view filepath);

public:
  void renderSpriteSheet(const glm::vec3& inOrigin);

  FrameElement getFrame(
    std::string alias,
    const glm::vec3& position,
    const glm::vec3& color,
    float scale,
    bool flipX
  ) const;

  void getColliders(
    std::string alias,
    const glm::vec3& position,
    float scale,
    bool flipX,
    std::vector<RectData>& healthColliders,
    std::vector<RectData>& attackColliders
  ) const;

  void renderFrame(const FrameElement& frame);

  void renderFrame(
    std::string alias,
    const glm::vec3& position,
    const glm::vec3& color,
    float scale,
    bool flipX,
    bool debugMode
  );


private:
  struct AnimationData {
    std::string imgFilename;
    glm::vec2 texSize;
    float aspectRatio;
    std::vector<std::shared_ptr<KeyframeData>> allKeyframes;
    std::unordered_map<std::string, std::shared_ptr<KeyframeData>> keyframesMap;
  }
  _animationData;

  struct Graphics {
    TexturesManager texturesManager;
    SpriteStackRenderer spriteStackRenderer;
  }
  _graphics;
};

