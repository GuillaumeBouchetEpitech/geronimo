
#include "TexturesManager.hpp"

#include "geronimo/graphics/Image.hpp"

std::weak_ptr<gero::graphics::opengl::Texture> TexturesManager::loadFromFile(const std::string& filepath)
{
  auto it = _texturesMap.find(filepath);
  if (it != _texturesMap.end())
  {
    return it->second;
  }

  gero::graphics::Image img;
  // img.loadFromFile("./assets/graphics/textures/hacky-sprites-v2.png");
  img.loadFromFile(filepath);


  auto newTexture = std::make_shared<gero::graphics::opengl::Texture>();
  newTexture->setFromImage(img);

  _texturesMap[filepath] = newTexture;

  return newTexture;
}

std::weak_ptr<gero::graphics::opengl::Texture> TexturesManager::getTexture(const std::string& filepath) const
{
  auto it = _texturesMap.find(filepath);
  if (it == _texturesMap.end())
  {
    D_THROW(std::runtime_error, "texture not found");
  }

  return it->second;
}
