
#pragma once

#include "opengl/Geometry.hpp"
#include "opengl/ShaderProgram.hpp"
#include "opengl/Texture.hpp"

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
  std::unordered_map<int32_t, std::shared_ptr<opengl::IUnboundShaderProgram>> _shadersMap;

public:
  std::shared_ptr<opengl::IUnboundShaderProgram>
  createShader(int32_t aliasCode, const opengl::ShaderProgram::Definition def, bool allowDuplicates = false);
  std::shared_ptr<opengl::IUnboundShaderProgram> getShader(int32_t aliasCode);

private:
  std::unordered_map<std::string, int32_t> _textureDefsMap;
  std::unordered_map<int32_t, std::shared_ptr<opengl::Texture>> _texturesMap;

public:
  std::shared_ptr<opengl::Texture> createTexture(int32_t aliasCode,
                                         const std::string& filename,
                                         opengl::Texture::Quality quality = opengl::Texture::Quality::pixelated,
                                         opengl::Texture::Pattern pattern = opengl::Texture::Pattern::clamped,
                                         bool allowDuplicates = false);
  std::shared_ptr<opengl::Texture> getTexture(int32_t aliasCode);

private:
  std::unordered_map<std::string, int32_t> _geometryDefsMap;
  std::unordered_map<int32_t, opengl::Geometry::Definition> _geometriesMap;

public:
  const opengl::Geometry::Definition&
  createGeometryDefinition(int32_t aliasCode, const opengl::Geometry::Definition& def, bool allowDuplicates = false);
  const opengl::Geometry::Definition& getGeometryDefinition(int32_t aliasCode);
};

} // namespace graphics
} // namespace gero
