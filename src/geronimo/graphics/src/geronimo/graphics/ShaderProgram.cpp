
#include "ShaderProgram.hpp"

#include "GlContext.hpp"

#include "geronimo/system/file-utils/getFileContent.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept> // <= std::invalid_argument / runtime_error
#include <string>

namespace gero {
namespace graphics {

namespace utilities {

bool getShaderSource(const std::string& filename,
                     std::string& source,
                     const fileUtils::LoadCallback& loadFileCallback) {
  std::string subSource;
  loadFileCallback(filename, subSource);

  std::vector<std::string> newLines;
  newLines.reserve(512);

  std::stringstream sstr;
  sstr << subSource;

  std::regex inlcudeRegexp(R"(^\s*#\s*include\s+\"(\S+)\")");

  std::string currentLine;
  while (std::getline(sstr, currentLine)) {

    std::smatch match;
    std::regex_search(currentLine, match, inlcudeRegexp);

    if (match.size() >= 2) {
      std::string includeFilename = match[1].str();

      std::string includeSource;
      includeSource.clear();
      loadFileCallback(includeFilename, includeSource);

      newLines.push_back(includeSource);
    } else {
      newLines.push_back(currentLine);
    }
  }

  source = sstr.str();

  std::stringstream sstrResult;
  for (auto& line : newLines)
    sstrResult << line << std::endl;
  source = sstrResult.str();

  return true;
}

} // namespace utilities

//

ShaderProgram::ShaderProgram(const Definition& inDef) : ShaderProgram(inDef, fileUtils::getDefaulCallback()) {}

ShaderProgram::ShaderProgram(const Definition& inDef, fileUtils::FileManager& fileManager)
  : ShaderProgram(inDef, fileUtils::getFileManagerCallback(fileManager)) {}

ShaderProgram::ShaderProgram(const Definition& inDef, const fileUtils::LoadCallback& loadFileCallback) {
  std::string vertexSourceCode;
  std::string fragmentSourceCode;

  if (!utilities::getShaderSource(inDef.filenames.vertex, vertexSourceCode, loadFileCallback))
    D_THROW(std::invalid_argument, "fail to read a file, filename=" << inDef.filenames.vertex);

  if (!utilities::getShaderSource(inDef.filenames.fragment, fragmentSourceCode, loadFileCallback))
    D_THROW(std::invalid_argument, "fail to read a file, filename=" << inDef.filenames.fragment);

  uint32_t vertexShader = GlContext::Shader::loadVertexShader(inDef.filenames.vertex, vertexSourceCode);
  uint32_t fragmentShader = GlContext::Shader::loadFragmentShader(inDef.filenames.fragment, fragmentSourceCode);

  _programId = GlContext::Shader::createProgram();

  if (!_programId)
    D_THROW(std::invalid_argument, "fail to create a shader program");

  if (!GlContext::Shader::linkProgram(_programId, vertexShader, fragmentShader)) {

    GlContext::Shader::deleteProgram(_programId);

    D_THROW(std::runtime_error,
            "fail to link a shader"
              << ", vertex=" << inDef.filenames.vertex << ", fragment=" << inDef.filenames.fragment);
  }

  for (const auto& attribute : inDef.attributes) {
    if (_attributesMap.count(attribute) > 0)
      D_THROW(std::runtime_error,
              "duplicate attribute"
                << ", input=" << attribute << ", vertex=" << inDef.filenames.vertex);

    const int32_t location = GlContext::Shader::getAttribLocation(_programId, attribute.c_str());

    if (location == -1)
      D_THROW(std::runtime_error,
              "fail to find an attribute location (missing or unused)"
                << ", input=" << attribute << ", vertex=" << inDef.filenames.vertex);

    _attributesMap[attribute] = location;
  }

  for (const auto& uniform : inDef.uniforms) {
    if (_uniformsMap.count(uniform) > 0)
      D_THROW(std::runtime_error,
              "duplicate uniform"
                << ", input=" << uniform << ", vertex=" << inDef.filenames.vertex
                << ", fragment=" << inDef.filenames.fragment);

    const int32_t location = GlContext::Shader::getUniformLocation(_programId, uniform.c_str());

    if (location == -1)
      D_THROW(std::runtime_error,
              "fail to find an uniform location (missing or unused)"
                << ", input=" << uniform << ", vertex=" << inDef.filenames.vertex
                << ", fragment=" << inDef.filenames.fragment);

    _uniformsMap[uniform] = location;
  }
}

ShaderProgram::~ShaderProgram() {
  if (_programId)
    GlContext::Shader::deleteProgram(_programId);
}

//

std::shared_ptr<IUnboundShaderProgram> ShaderProgram::buildUnbound(const ShaderProgram::Definition& inDef) {
  return std::make_shared<ShaderProgram>(inDef);
}
std::shared_ptr<IUnboundShaderProgram> ShaderProgram::buildUnbound(const Definition& inDef,
                                                                   fileUtils::FileManager& fileManager) {
  return std::make_shared<ShaderProgram>(inDef, fileManager);
}
std::shared_ptr<IUnboundShaderProgram> ShaderProgram::buildUnbound(const Definition& inDef,
                                                                   const fileUtils::LoadCallback& loadFileCallback) {
  return std::make_shared<ShaderProgram>(inDef, loadFileCallback);
}

//

void ShaderProgram::rawBind() const {
  if (!_programId)
    D_THROW(std::runtime_error, "shader not initialized");

  GlContext::Shader::useProgram(_programId);
}

void ShaderProgram::preBind(const std::function<void(IBoundShaderProgram&)>& callback) {
  rawBind();
  callback(*this);
}

void ShaderProgram::bind(const std::function<void(IBoundShaderProgram&)>& callback) {
  preBind(callback);
  ShaderProgram::unbind();
}

void ShaderProgram::unbind() { GlContext::Shader::useProgram(0); }

//

int32_t ShaderProgram::getAttribute(const char* name) const {
  auto it = _attributesMap.find(name);

  if (it == _attributesMap.end())
    D_THROW(std::invalid_argument, "unknown attribute, input=" << name);

  return it->second;
}

int32_t ShaderProgram::getUniform(const char* name) const {
  auto it = _uniformsMap.find(name);

  if (it == _uniformsMap.end())
    D_THROW(std::invalid_argument, "unknown uniform, input=" << name);

  return it->second;
}

bool ShaderProgram::hasAttribute(const char* name) const { return (_attributesMap.find(name) != _attributesMap.end()); }

bool ShaderProgram::hasUniform(const char* name) const { return (_uniformsMap.count(name) > 0); }

//

void ShaderProgram::setUniform(const char* name, int32_t value) const { setUniform(getUniform(name), value); }

void ShaderProgram::setUniform(const char* name, int32_t x, int32_t y) const { setUniform(getUniform(name), x, y); }

void ShaderProgram::setUniform(const char* name, int32_t x, int32_t y, int32_t z) const {
  setUniform(getUniform(name), x, y, z);
}

void ShaderProgram::setUniform(const char* name, int32_t x, int32_t y, int32_t z, int32_t w) const {
  setUniform(getUniform(name), x, y, z, w);
}

void ShaderProgram::setUniform(const char* name, float value) const { setUniform(getUniform(name), value); }

void ShaderProgram::setUniform(const char* name, float x, float y) const { setUniform(getUniform(name), x, y); }

void ShaderProgram::setUniform(const char* name, float x, float y, float z) const {
  setUniform(getUniform(name), x, y, z);
}

void ShaderProgram::setUniform(const char* name, float x, float y, float z, float w) const {
  setUniform(getUniform(name), x, y, z, w);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& vec3) const {
  setUniform(getUniform(name), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::setUniform(const char* name, const glm::vec4& vec4) const {
  setUniform(getUniform(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderProgram::setUniform(const char* name, const glm::mat3& mat3) const { setUniform(getUniform(name), mat3); }

void ShaderProgram::setUniform(const char* name, const glm::mat4& mat4) const { setUniform(getUniform(name), mat4); }

//

void ShaderProgram::setUniform(int32_t location, int32_t value) const {
  GlContext::Shader::setUniform(location, value);
}

void ShaderProgram::setUniform(int32_t location, int32_t x, int32_t y) const {
  GlContext::Shader::setUniform(location, x, y);
}

void ShaderProgram::setUniform(int32_t location, int32_t x, int32_t y, int32_t z) const {
  GlContext::Shader::setUniform(location, x, y, z);
}

void ShaderProgram::setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const {
  GlContext::Shader::setUniform(location, x, y, z, w);
}

void ShaderProgram::setUniform(int32_t location, float value) const { GlContext::Shader::setUniform(location, value); }

void ShaderProgram::setUniform(int32_t location, float x, float y) const {
  GlContext::Shader::setUniform(location, x, y);
}

void ShaderProgram::setUniform(int32_t location, float x, float y, float z) const {
  GlContext::Shader::setUniform(location, x, y, z);
}

void ShaderProgram::setUniform(int32_t location, float x, float y, float z, float w) const {
  GlContext::Shader::setUniform(location, x, y, z, w);
}

void ShaderProgram::setUniform(int32_t location, const glm::vec3& vec3) const {
  GlContext::Shader::setUniform(location, vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::setUniform(int32_t location, const glm::vec4& vec4) const {
  GlContext::Shader::setUniform(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderProgram::setUniform(int32_t location, const glm::mat3& mat3) const {
  GlContext::Shader::setUniform(location, mat3);
}

void ShaderProgram::setUniform(int32_t location, const glm::mat4& mat4) const {
  GlContext::Shader::setUniform(location, mat4);
}

} // namespace graphics
} // namespace gero
