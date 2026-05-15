
#include "KeyframesManager.hpp"

#include "geronimo/system/parser-utils/jsonUtils.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include "geronimo/helpers/Json.hpp"

#include <fstream> // <- std::ifstream
#include <algorithm> // <- std::sort

// MARK: loadJsonFile
void KeyframesManager::_loadJsonFile(const std::string_view inFilepath)
{
  // parse json file
  std::ifstream filestream(inFilepath.data());

  constexpr bool allow_exceptions = true;
  constexpr bool ignore_comments = true;
  const json data = json::parse(filestream, nullptr, allow_exceptions, ignore_comments);

  // D_MYLOG(" -> data[\"hello\"] => " << data["hello"]);

  D_MYLOG(" -> data[\"image-filepath\"] => " << data["image-filepath"]);

  jsonUtils::str::get(data, "image-filepath", this->_animationData.imgFilename);

  this->_graphics.texturesManager.loadFromFile(this->_animationData.imgFilename);
  auto weakTexture = this->_graphics.texturesManager.getTexture(this->_animationData.imgFilename);
  if (weakTexture.expired()) {
    D_THROW(std::runtime_error, "texture is expired");
  }
  auto texture = weakTexture.lock();
  this->_animationData.texSize = glm::vec2(texture->getSize());
  this->_animationData.aspectRatio = this->_animationData.texSize.x / this->_animationData.texSize.y;

  jsonUtils::common::propertyExist(data, "keyframes");
  auto keyframes = data["keyframes"];
  if (!keyframes.is_object()) {
    D_THROW(std::invalid_argument, "keyframes not an object");
  }

  for (auto pairVals : keyframes.items())
  {
    auto newKeyframe = std::make_shared<KeyframeData>();

    newKeyframe->alias = pairVals.key();
    D_MYLOG("   -> newKeyframe->alias => " << newKeyframe->alias);

    this->_animationData.allKeyframes.push_back(newKeyframe);
    this->_animationData.keyframesMap[newKeyframe->alias] = newKeyframe;

    const auto keyframeVal = pairVals.value();

    jsonUtils::fvec2::get(keyframeVal, "origin", newKeyframe->origin);
    D_MYLOG("   -> newKeyframe->origin => " << newKeyframe->origin);

    // correct Y axis
    newKeyframe->origin.y = this->_animationData.texSize.y - newKeyframe->origin.y;

    if (!keyframeVal.is_object()) {
      D_THROW(std::invalid_argument, "keyframe not an object");
    }
    if (!keyframeVal["sprite-rect"].is_object()) {
      D_THROW(std::invalid_argument, "keyframe sprite-rect not an array");
    }

    // jsonUtils::fvec4::get(keyframeVal, "sprite-rect", newKeyframe->rect);

    jsonUtils::fvec2::get(keyframeVal["sprite-rect"], "bl", newKeyframe->rect.bottomLeft);
    jsonUtils::fvec2::get(keyframeVal["sprite-rect"], "tr", newKeyframe->rect.topRight);
    D_MYLOG("   -> newKeyframe->rect.bottomLeft => " << newKeyframe->rect.bottomLeft);
    D_MYLOG("   -> newKeyframe->rect.topRight => " << newKeyframe->rect.topRight);

    // correct Y axis
    newKeyframe->rect.bottomLeft.y = this->_animationData.texSize.y - newKeyframe->rect.bottomLeft.y;
    newKeyframe->rect.topRight.y = this->_animationData.texSize.y - newKeyframe->rect.topRight.y;

    if (keyframeVal.contains("health-colliders")) {
      auto healthColliders = keyframeVal["health-colliders"];
      if (!healthColliders.is_array()) {
        D_THROW(std::invalid_argument, "health-colliders not an array");
      }

      D_MYLOG("   -> healthColliders => " << healthColliders.size());

      newKeyframe->healthColliders.reserve(healthColliders.size());

      for (auto healthCollider : healthColliders)
      {
        if (!healthCollider.is_object()) {
          D_THROW(std::invalid_argument, "healthCollider not an object");
        }

        RectData newRect;
        jsonUtils::fvec2::get(healthCollider["bl"], newRect.bottomLeft);
        jsonUtils::fvec2::get(healthCollider["tr"], newRect.topRight);
        D_MYLOG("     -> healthColliderRect.bottomLeft => " << newRect.bottomLeft);
        D_MYLOG("     -> healthColliderRect.topRight => " << newRect.topRight);

        // correct Y axis
        newRect.bottomLeft.y = this->_animationData.texSize.y - newRect.bottomLeft.y;
        newRect.topRight.y = this->_animationData.texSize.y - newRect.topRight.y;

        newKeyframe->healthColliders.push_back(newRect);
      }
    }

    if (keyframeVal.contains("attack-colliders")) {
      auto attackColliders = keyframeVal["attack-colliders"];
      if (!attackColliders.is_array()) {
        D_THROW(std::invalid_argument, "attack-colliders not an array");
      }

      D_MYLOG("   -> attackColliders => " << attackColliders.size());

      newKeyframe->attackColliders.reserve(attackColliders.size());

      for (auto attackCollider : attackColliders)
      {
        if (!attackCollider.is_object()) {
          D_THROW(std::invalid_argument, "attackCollider not an object");
        }

        RectData newRect;
        jsonUtils::fvec2::get(attackCollider["bl"], newRect.bottomLeft);
        jsonUtils::fvec2::get(attackCollider["tr"], newRect.topRight);
        D_MYLOG("     -> attackColliderRect.bottomLeft => " << newRect.bottomLeft);
        D_MYLOG("     -> attackColliderRect.topRight => " << newRect.topRight);

        // correct Y axis
        newRect.bottomLeft.y = this->_animationData.texSize.y - newRect.bottomLeft.y;
        newRect.topRight.y = this->_animationData.texSize.y - newRect.topRight.y;

        newKeyframe->attackColliders.push_back(newRect);
      }
    }
  }

}
