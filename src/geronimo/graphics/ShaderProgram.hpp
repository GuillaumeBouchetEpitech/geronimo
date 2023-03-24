
#pragma once

#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/FileManager.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace gero {
namespace graphics {

class UniformBufferObject;

class ShaderProgram {
private:
  friend UniformBufferObject;

public:
  struct Definition {
    struct Filenames {
      std::string vertex;
      std::string fragment;
    } filenames;

    std::vector<std::string> attributes;
    std::vector<std::string> uniforms;
  };

private:
  uint32_t _programId = 0;
  std::unordered_map<std::string, int32_t> _attributesMap;
  std::unordered_map<std::string, int32_t> _uniformsMap;

public:
  ShaderProgram(const Definition& def);
  ShaderProgram(const Definition& def, fileUtils::FileManager& fileManager);
  ShaderProgram(const Definition& def, const fileUtils::LoadCallback& loadFileCallback);
  ~ShaderProgram();

public:
  void bind() const;

public:
  static void unbind();

public:
  int32_t getAttribute(const char* name) const;
  int32_t getUniform(const char* name) const;
  bool hasAttribute(const char* name) const;
  bool hasUniform(const char* name) const;

public:
  void setUniform(const char* name, int32_t value) const;
  void setUniform(const char* name, int32_t x, int32_t y) const;
  void setUniform(const char* name, int32_t x, int32_t y, int32_t z) const;
  void setUniform(const char* name, int32_t x, int32_t y, int32_t z, int32_t w) const;
  void setUniform(const char* name, float value) const;
  void setUniform(const char* name, float x, float y) const;
  void setUniform(const char* name, float x, float y, float z) const;
  void setUniform(const char* name, float x, float y, float z, float w) const;
  void setUniform(const char* name, const glm::vec3& vec3) const;
  void setUniform(const char* name, const glm::vec4& vec4) const;
  void setUniform(const char* name, const glm::mat3& mat3) const;
  void setUniform(const char* name, const glm::mat4& mat4) const;

public:
  void setUniform(int32_t location, int32_t value) const;
  void setUniform(int32_t location, int32_t x, int32_t y) const;
  void setUniform(int32_t location, int32_t x, int32_t y, int32_t z) const;
  void setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const;
  void setUniform(int32_t location, float value) const;
  void setUniform(int32_t location, float x, float y) const;
  void setUniform(int32_t location, float x, float y, float z) const;
  void setUniform(int32_t location, float x, float y, float z, float w) const;
  void setUniform(int32_t location, const glm::vec3& vec3) const;
  void setUniform(int32_t location, const glm::vec4& vec4) const;
  void setUniform(int32_t location, const glm::mat3& mat3) const;
  void setUniform(int32_t location, const glm::mat4& mat4) const;
};

} // namespace graphics
} // namespace gero
