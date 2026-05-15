
#include "KeyframesManager.hpp"

#include "application/context/Context.hpp"

#include <fstream> // <- std::ifstream
#include <algorithm> // <- std::sort


// MARK: Rect_flipX
RectData RectData::flipX(float originX) const
{
  RectData flipped;
  flipped.bottomLeft = this->bottomLeft;
  flipped.topRight = this->topRight;

  float hsizeX = (flipped.topRight.x - flipped.bottomLeft.x) * 0.5f;
  float centerX = flipped.bottomLeft.x + hsizeX;

  float diffX = centerX - originX;

  centerX -= diffX * 2.0f;

  flipped.bottomLeft.x = centerX - hsizeX;
  flipped.topRight.x = centerX + hsizeX;

  return flipped;
}

// MARK: Rect_testOverlap
bool RectData::testOverlap(const RectData& other) const
{
  // faster to check if "it does not overlap"
  const bool isNotOverlapping = (
    // "this" is too far left
    this->topRight.x < other.bottomLeft.x ||
    // "this" is too far down
    this->topRight.y < other.bottomLeft.y ||
    // "this" is too far right
    this->bottomLeft.x > other.topRight.x ||
    // "this" is too far up
    this->bottomLeft.y > other.topRight.y
  );

  // "inverse of not overlap" means: "is overlapping"
  return isNotOverlapping == false;
}

// MARK: Rect_render
void RectData::render(float thickness, float depth, const glm::vec4& inColor, gero::graphics::IStackRenderers &stackRenderer) const
{
  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  // const float k_depth = inPosition.z - 0.02f;

  const glm::vec2 size = (this->topRight - this->bottomLeft);
  // wireFramesStack.pushRectangle(this->bottomLeft, size, glm::vec3(1,1,1), depth);

  std::array<glm::vec3, 4> tmpVertices = {{
    glm::vec3(this->bottomLeft + glm::vec2(size.x*0, size.y*0), depth),
    glm::vec3(this->bottomLeft + glm::vec2(size.x*1, size.y*0), depth),
    glm::vec3(this->bottomLeft + glm::vec2(size.x*1, size.y*1), depth),
    glm::vec3(this->bottomLeft + glm::vec2(size.x*0, size.y*1), depth),
  }};

  // trianglesStack.pushThickTriangle3dLine(tmpVertices.at(0), tmpVertices.at(1), thickness, inColor);
  // trianglesStack.pushThickTriangle3dLine(tmpVertices.at(1), tmpVertices.at(2), thickness, inColor);
  // trianglesStack.pushThickTriangle3dLine(tmpVertices.at(2), tmpVertices.at(3), thickness, inColor);
  // trianglesStack.pushThickTriangle3dLine(tmpVertices.at(3), tmpVertices.at(0), thickness, inColor);

  trianglesStack.pushThickTriangle2dLine(tmpVertices.at(0), tmpVertices.at(1), thickness, inColor, depth);
  trianglesStack.pushThickTriangle2dLine(tmpVertices.at(1), tmpVertices.at(2), thickness, inColor, depth);
  trianglesStack.pushThickTriangle2dLine(tmpVertices.at(2), tmpVertices.at(3), thickness, inColor, depth);
  trianglesStack.pushThickTriangle2dLine(tmpVertices.at(3), tmpVertices.at(0), thickness, inColor, depth);
}

// MARK: ctor
KeyframesManager::KeyframesManager()
{
  {
    const std::string_view tmpFilename = "./assets/data/stupid-data.jsonc";
    this->_loadJsonFile(tmpFilename);
  }

  {
    this->_graphics.spriteStackRenderer.initialize();
  }
}

// MARK: renderSpriteSheet
void KeyframesManager::renderSpriteSheet(const glm::vec3& inOrigin)
{
  auto& context = Context::get();

  const auto& matricesData = context.graphic.renderer.getSceneRenderer().getCamera().getMatricesData();

  auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  stackRenderer.flush();

  wireFramesStack.pushLine(inOrigin + glm::vec3(0,0,0), inOrigin + glm::vec3(100,0,0), glm::vec3(1,0,0));
  wireFramesStack.pushLine(inOrigin + glm::vec3(0,0,0), inOrigin + glm::vec3(0,100,0), glm::vec3(0,1,0));
  wireFramesStack.pushLine(inOrigin + glm::vec3(0,0,0), inOrigin + glm::vec3(0,0,100), glm::vec3(0,0,1));


  {
    auto weakTexture = this->_graphics.texturesManager.getTexture(this->_animationData.imgFilename);
    if (weakTexture.expired()) {
      D_THROW(std::runtime_error, "texture is expired");
    }
    auto texture = weakTexture.lock();

    const glm::uvec2& texSize = texture->getSize();
    const float aspectRatio = float(texSize.x) / float(texSize.y);

    constexpr float height = 10.0f;
    const float width = height * aspectRatio;
    const float scale = height / float(texSize.y);

    {
      gero::graphics::GlContext::Texture::active(0);


      texture->bind();

      FrameElement frameElem;
      frameElem.pos.bottomLeft = glm::vec2(inOrigin) + glm::vec2(0,0);
      frameElem.pos.topRight = glm::vec2(inOrigin) + glm::vec2(width,height);
      frameElem.texcoord.bottomLeft = glm::vec2(0,0);
      frameElem.texcoord.topRight = glm::vec2(1,1);
      frameElem.color = glm::vec3(1,1,1);
      frameElem.depth = inOrigin.z;
      // sceneRenderer.getSpriteStackRenderer().pushFrame(frameElem);
      // sceneRenderer.getSpriteStackRenderer().render();

      this->_graphics.spriteStackRenderer.setMatricesData(matricesData);
      this->_graphics.spriteStackRenderer.clear();
      this->_graphics.spriteStackRenderer.pushFrame(frameElem);
      this->_graphics.spriteStackRenderer.render();
    }

    {

      for (const auto& currFrame : this->_animationData.allKeyframes)
      {
        wireFramesStack.pushCross(inOrigin + glm::vec3(currFrame->origin * scale, inOrigin.z +0.1f), glm::vec3(1,1,0), 0.5f);

        // {
        //   const glm::vec2 spritePos = currFrame->rect.bottomLeft * scale;
        //   const glm::vec2 spriteSize = (currFrame->rect.topRight - currFrame->rect.bottomLeft) * scale;

        //   wireFramesStack.pushRectangle(glm::vec2(inOrigin) + spritePos, spriteSize, glm::vec3(1,0,1), inOrigin.z);
        // }

        // for (const auto& currCollider : currFrame->healthColliders)
        // {
        //   const glm::vec2 colliderPos = currCollider.bottomLeft * scale;
        //   const glm::vec2 colliderSize = (currCollider.topRight - currCollider.bottomLeft) * scale;

        //   wireFramesStack.pushRectangle(glm::vec2(inOrigin) + colliderPos, colliderSize, glm::vec3(0,1,0), inOrigin.z + 0.02f);
        // }

        // for (const auto& currCollider : currFrame->attackColliders)
        // {
        //   const glm::vec2 colliderPos = currCollider.bottomLeft * scale;
        //   const glm::vec2 colliderSize = (currCollider.topRight - currCollider.bottomLeft) * scale;

        //   wireFramesStack.pushRectangle(glm::vec2(inOrigin) + colliderPos, colliderSize, glm::vec3(1,0,0), inOrigin.z + 0.02f);
        // }

        constexpr float k_thickness = 0.05f;

        // debug rect
        {
          const float k_depth = inOrigin.z;

          RectData tmpCollider = {
            glm::vec2(inOrigin) + currFrame->rect.bottomLeft * scale,
            glm::vec2(inOrigin) + currFrame->rect.topRight * scale
          };

          tmpCollider.render(k_thickness, k_depth, glm::vec4(1,1,1,1), stackRenderer);
        }

        // debug healthColliders
        for (const auto& currCollider : currFrame->healthColliders)
        {
          const float k_depth = inOrigin.z + 0.02f;

          RectData tmpCollider = {
            glm::vec2(inOrigin) + currCollider.bottomLeft * scale,
            glm::vec2(inOrigin) + currCollider.topRight * scale
          };

          tmpCollider.render(k_thickness, k_depth, glm::vec4(0,1,0,1), stackRenderer);
        }

        // debug attackColliders
        for (const auto& currCollider : currFrame->attackColliders)
        {
          const float k_depth = inOrigin.z + 0.02f;

          RectData tmpCollider = {
            glm::vec2(inOrigin) + currCollider.bottomLeft * scale,
            glm::vec2(inOrigin) + currCollider.topRight * scale
          };

          tmpCollider.render(k_thickness, k_depth, glm::vec4(1,0,0,1), stackRenderer);
        }

      }

    }

  }

  stackRenderer.flush();
}


// MARK: getFrame
FrameElement KeyframesManager::getFrame(
  std::string inAlias,
  const glm::vec3& inPosition,
  const glm::vec3& inColor,
  float inScale,
  bool inFlipX
) const {

  const float height = inScale;
  const float width = height * this->_animationData.aspectRatio;

  auto it = this->_animationData.keyframesMap.find(inAlias);
  if (it == this->_animationData.keyframesMap.end()) {
    D_THROW(std::runtime_error, "frame not found, alias: " << inAlias);
  }
  auto currKeyFrame = it->second;

  RectData targetRect = currKeyFrame->rect;

  if (inFlipX) {
    targetRect = targetRect.flipX(currKeyFrame->origin.x);
  }

  FrameElement frameElem;

  // center the position by using the sprite rect+origin
  frameElem.pos.bottomLeft = targetRect.bottomLeft - currKeyFrame->origin;
  frameElem.pos.topRight = targetRect.topRight - currKeyFrame->origin;

  // normalize + scale
  frameElem.pos.bottomLeft = frameElem.pos.bottomLeft / this->_animationData.texSize * glm::vec2(width, height);
  frameElem.pos.topRight = frameElem.pos.topRight / this->_animationData.texSize * glm::vec2(width, height);

  frameElem.depth = 0.0f;

  frameElem.texcoord.bottomLeft = currKeyFrame->rect.bottomLeft;
  frameElem.texcoord.topRight = currKeyFrame->rect.topRight;

  // invert the Y axis coordinate for the texcoord property
  frameElem.texcoord.bottomLeft.y = this->_animationData.texSize.y - frameElem.texcoord.bottomLeft.y;
  frameElem.texcoord.topRight.y = this->_animationData.texSize.y - frameElem.texcoord.topRight.y;

  // is upside down -> must swap the Y texcoord values of topRight/bottomLeft
  std::swap(frameElem.texcoord.bottomLeft.y, frameElem.texcoord.topRight.y);

  // normalize the texcoord property
  frameElem.texcoord.bottomLeft /= this->_animationData.texSize;
  frameElem.texcoord.topRight /= this->_animationData.texSize;

  if (inFlipX) {
    // must swap the X texcoord values of topRight/bottomLeft
    std::swap(frameElem.texcoord.bottomLeft.x, frameElem.texcoord.topRight.x);
  }

  frameElem.color = inColor;

  // move to the desired scene position
  frameElem.pos.bottomLeft += glm::vec2(inPosition);
  frameElem.pos.topRight += glm::vec2(inPosition);
  frameElem.depth += inPosition.z;

  return frameElem;
}

// MARK: getColliders
void KeyframesManager::getColliders(
  std::string inAlias,
  const glm::vec3& inPosition,
  float inScale,
  bool inFlipX,
  std::vector<RectData>& healthColliders,
  std::vector<RectData>& attackColliders
) const {

  const float height = inScale;
  const float width = height * this->_animationData.aspectRatio;

  auto it = this->_animationData.keyframesMap.find(inAlias);
  if (it == this->_animationData.keyframesMap.end()) {
    D_THROW(std::runtime_error, "frame not found, alias: " << inAlias);
  }
  auto currKeyFrame = it->second;

  const float scale = height / float(this->_animationData.texSize.y);

  // debug healthColliders
  healthColliders.reserve(currKeyFrame->healthColliders.size());
  for (const auto& currCollider : currKeyFrame->healthColliders)
  {
    RectData currRect = currCollider;
    if (inFlipX) {
      currRect = currRect.flipX(currKeyFrame->origin.x);
    }

    const glm::vec2 bottomLeft = currRect.bottomLeft - currKeyFrame->origin;
    const glm::vec2 topRight = currRect.topRight - currKeyFrame->origin;
    const glm::vec2 colliderPos = bottomLeft * scale;
    const glm::vec2 colliderSize = (topRight - bottomLeft) * scale;

    healthColliders.push_back({ glm::vec2(inPosition) + colliderPos, glm::vec2(inPosition) + colliderPos + colliderSize });
  }

  // debug attackColliders
  attackColliders.reserve(currKeyFrame->attackColliders.size());
  for (const auto& currCollider : currKeyFrame->attackColliders)
  {
    RectData currRect = currCollider;
    if (inFlipX) {
      currRect = currRect.flipX(currKeyFrame->origin.x);
    }

    const glm::vec2 bottomLeft = currRect.bottomLeft - currKeyFrame->origin;
    const glm::vec2 topRight = currRect.topRight - currKeyFrame->origin;
    const glm::vec2 colliderPos = bottomLeft * scale;
    const glm::vec2 colliderSize = (topRight - bottomLeft) * scale;

    attackColliders.push_back({ glm::vec2(inPosition) + colliderPos, glm::vec2(inPosition) + colliderPos + colliderSize });
  }
}

// MARK: renderFrame
void KeyframesManager::renderFrame(const FrameElement& inFrameElem)
{
  auto weakTexture = this->_graphics.texturesManager.getTexture(this->_animationData.imgFilename);
  if (weakTexture.expired()) {
    D_THROW(std::runtime_error, "texture is expired");
  }
  auto texture = weakTexture.lock();

  auto& context = Context::get();

  const auto& matricesData = context.graphic.renderer.getSceneRenderer().getCamera().getMatricesData();

  gero::graphics::GlContext::Texture::active(0);

  texture->bind();

  // render the sprite frame
  this->_graphics.spriteStackRenderer.setMatricesData(matricesData);
  this->_graphics.spriteStackRenderer.clear();
  this->_graphics.spriteStackRenderer.pushFrame(inFrameElem);
  this->_graphics.spriteStackRenderer.render();
}

// MARK: renderFrame
void KeyframesManager::renderFrame(
  std::string inAlias,
  const glm::vec3& inPosition,
  const glm::vec3& inColor,
  float inScale,
  bool inFlipX,
  bool inDebugMode
) {

  auto& context = Context::get();

  const auto& matricesData = context.graphic.renderer.getSceneRenderer().getCamera().getMatricesData();

  auto it = this->_animationData.keyframesMap.find(inAlias);
  if (it == this->_animationData.keyframesMap.end()) {
    D_THROW(std::runtime_error, "frame not found, alias: " << inAlias);
  }
  auto currKeyFrame = it->second;

  FrameElement frameElem = this->getFrame(inAlias, inPosition, inColor, inScale, inFlipX);

  this->renderFrame(frameElem);

  if (inDebugMode)
  {
    auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();

    auto& wireFramesStack = stackRenderer.getWireFramesStack();
    auto& trianglesStack = stackRenderer.getTrianglesStack();

    std::vector<RectData> healthColliders;
    std::vector<RectData> attackColliders;
    this->getColliders(
      inAlias,
      inPosition,
      inScale,
      inFlipX,
      healthColliders,
      attackColliders
    );

    // debug position
    wireFramesStack.pushCross(inPosition, glm::vec3(1,1,1), 1.0f);

    constexpr float k_thickness = 0.05f;

    // debug rect
    {
      const float k_depth = inPosition.z - 0.02f;

      RectData currCollider = { frameElem.pos.bottomLeft, frameElem.pos.topRight };

      currCollider.render(k_thickness, k_depth, glm::vec4(1,1,1,1), stackRenderer);
    }

    // debug healthColliders
    for (const auto& currCollider : healthColliders)
    {
      const float k_depth = inPosition.z + 0.02f;

      currCollider.render(k_thickness, k_depth, glm::vec4(0,1,0,1), stackRenderer);
    }

    // debug attackColliders
    for (const auto& currCollider : attackColliders)
    {
      const float k_depth = inPosition.z + 0.02f;

      currCollider.render(k_thickness, k_depth, glm::vec4(1,0,0,1), stackRenderer);
    }

  }
}

