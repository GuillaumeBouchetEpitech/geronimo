
#include "ResourceManager.hpp"

#include "Image.hpp"

#include "geronimo/system/asValue.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <sstream>

namespace gero {
namespace graphics {

std::shared_ptr<IUnboundShaderProgram> ResourceManager::createShader(int32_t aliasCode,
                                                                     const ShaderProgram::Definition def,
                                                                     bool allowDuplicates /*= false*/) {
  std::stringstream sstr;
  sstr << def.filenames.vertex << "-" << def.filenames.fragment;
  std::string shaderUniqueName = sstr.str();

  auto itDef = _shaderDefsMap.find(shaderUniqueName);
  if (itDef != _shaderDefsMap.end()) {

    if (!allowDuplicates) {
      D_THROW(std::runtime_error, "resource manager shader is duplicated");
    }

    auto itShader = _shadersMap.find(itDef->second);
    if (itShader == _shadersMap.end())
      D_THROW(std::runtime_error, "resource manager shader map corrupted");

    _shadersMap[aliasCode] = itShader->second; // save same under new alias
    return itShader->second;
  }

  if (_shadersMap.count(aliasCode) > 0)
    D_THROW(std::runtime_error, "resource manager new shader alias is duplicated, aliasCode=" << aliasCode);

  auto newShader = ShaderProgram::buildUnbound(def, _fileManager);
  _shadersMap[aliasCode] = newShader;
  _shaderDefsMap[shaderUniqueName] = aliasCode;
  return newShader;
}

std::shared_ptr<IUnboundShaderProgram> ResourceManager::getShader(int32_t aliasCode) {
  auto it = _shadersMap.find(aliasCode);
  if (it == _shadersMap.end())
    D_THROW(std::runtime_error, "resource manager shader does not exist, aliasCode=" << aliasCode);

  return it->second;
}

//
//
//

std::shared_ptr<Texture> ResourceManager::createTexture(int32_t aliasCode,
                                                        const std::string& filename,
                                                        Texture::Quality quality /*= Texture::Quality::pixelated*/,
                                                        Texture::Pattern pattern /*= Texture::Pattern::clamped*/,
                                                        bool allowDuplicates /*= false*/) {
  std::stringstream sstr;
  sstr << filename << "-quality=" << asValue(quality) << "-repeat=" << asValue(pattern);
  std::string textureUniqueName = sstr.str();

  auto itDef = _textureDefsMap.find(textureUniqueName);
  if (itDef != _textureDefsMap.end()) {

    if (!allowDuplicates) {
      D_THROW(std::runtime_error, "resource manager texture is duplicated");
    }

    auto itTexture = _texturesMap.find(itDef->second);
    if (itTexture == _texturesMap.end())
      D_THROW(std::runtime_error, "resource manager texture map corrupted");

    _texturesMap[aliasCode] = itTexture->second; // save same under new alias
    return itTexture->second;
  }

  if (_texturesMap.count(aliasCode) > 0)
    D_THROW(std::runtime_error, "resource manager new texture alias is duplicated, aliasCode=" << aliasCode);

  auto newTexture = std::make_shared<Texture>();

  Image tmpImg;
  tmpImg.loadFromFile(_fileManager, filename);
  newTexture->setFromImage(tmpImg, quality, pattern);

  _texturesMap[aliasCode] = newTexture;
  _textureDefsMap[textureUniqueName] = aliasCode;
  return newTexture;
}

std::shared_ptr<Texture> ResourceManager::getTexture(int32_t aliasCode) {
  auto it = _texturesMap.find(aliasCode);
  if (it == _texturesMap.end())
    D_THROW(std::runtime_error, "resource manager texture does not exist, aliasCode=" << aliasCode);

  return it->second;
}

//
//
//

const Geometry::Definition& ResourceManager::createGeometryDefinition(int32_t aliasCode,
                                                                      const Geometry::Definition& def,
                                                                      bool allowDuplicates /*= false*/) {
  std::stringstream sstr;

  sstr << asValue(def.primitiveType) << "-" << def.vbos.size();
  for (const auto& vbo : def.vbos) {
    sstr << "-" << vbo.instanced;
    sstr << "-" << vbo.stride;
    sstr << "-" << vbo.attrs.size();
    for (const auto& attr : vbo.attrs) {
      sstr << "-" << attr.index;
      sstr << "-" << attr.name;
      sstr << "-" << asValue(attr.type);
      sstr << "-" << attr.ignored;
    }
  }

  std::string geoDefUniqueName = sstr.str();

  auto itDef = _geometryDefsMap.find(geoDefUniqueName);
  if (itDef != _geometryDefsMap.end()) {

    if (!allowDuplicates) {
      D_THROW(std::runtime_error, "resource manager geometry definition is duplicated");
    }

    auto itGeoDef = _geometriesMap.find(itDef->second);
    if (itGeoDef == _geometriesMap.end())
      D_THROW(std::runtime_error, "resource manager geometry definition map corrupted");

    _geometriesMap[aliasCode] = itGeoDef->second; // save same under new alias
    return itGeoDef->second;
  }

  if (_geometriesMap.count(aliasCode) > 0)
    D_THROW(std::runtime_error,
            "resource manager new geometry definition alias is duplicated, aliasCode=" << aliasCode);

  _geometriesMap[aliasCode] = def; // hard copy
  _geometryDefsMap[geoDefUniqueName] = aliasCode;
  return def;
}

const Geometry::Definition& ResourceManager::getGeometryDefinition(int32_t aliasCode) {
  auto it = _geometriesMap.find(aliasCode);
  if (it == _geometriesMap.end())
    D_THROW(std::runtime_error, "resource manager geometry definition does not exist, aliasCode=" << aliasCode);

  return it->second;
}

} // namespace graphics
} // namespace gero
