
#pragma once

#include "geronimo/graphics/opengl/Texture.hpp"

#include "geronimo/system/NonCopyable.hpp"

#include <unordered_map>
#include <memory>

class TexturesManager : public gero::NonCopyable
{
public:
  TexturesManager() = default;
  ~TexturesManager() = default;

public:
  std::weak_ptr<gero::graphics::opengl::Texture> loadFromFile(const std::string& filepath);
  std::weak_ptr<gero::graphics::opengl::Texture> getTexture(const std::string& filepath) const;

private:
  using TextureUniquePtr = std::shared_ptr<gero::graphics::opengl::Texture>;
  std::unordered_map<std::string, TextureUniquePtr> _texturesMap;

};
