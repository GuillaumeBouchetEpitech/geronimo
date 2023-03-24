
#pragma once

#include "Geometry.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

#include "geronimo/system/file-utils/FileManager.hpp"

#include <memory>
#include <unordered_map>

namespace gero {
namespace graphics {

class ResourceManager {
public:
  ResourceManager() = default;
  ~ResourceManager() = default;

private:
  fileUtils::FileManager _fileManager;

private:
  std::unordered_map<std::string, int32_t> _shaderDefsMap;
  std::unordered_map<int32_t, std::shared_ptr<ShaderProgram>> _shadersMap;

public:
  std::shared_ptr<ShaderProgram>
  createShader(int32_t aliasCode, const ShaderProgram::Definition def, bool allowDuplicates = false);
  std::shared_ptr<ShaderProgram> getShader(int32_t aliasCode);

private:
  std::unordered_map<std::string, int32_t> _textureDefsMap;
  std::unordered_map<int32_t, std::shared_ptr<Texture>> _texturesMap;

public:
  std::shared_ptr<Texture> createTexture(int32_t aliasCode,
                                         const std::string& filename,
                                         Texture::Quality quality = Texture::Quality::pixelated,
                                         Texture::Pattern pattern = Texture::Pattern::clamped,
                                         bool allowDuplicates = false);
  std::shared_ptr<Texture> getTexture(int32_t aliasCode);

private:
  std::unordered_map<std::string, int32_t> _geometryDefsMap;
  std::unordered_map<int32_t, Geometry::Definition> _geometriesMap;

public:
  const Geometry::Definition&
  createGeometryDefinition(int32_t aliasCode, const Geometry::Definition& def, bool allowDuplicates = false);
  const Geometry::Definition& getGeometryDefinition(int32_t aliasCode);
};

} // namespace graphics
} // namespace gero
