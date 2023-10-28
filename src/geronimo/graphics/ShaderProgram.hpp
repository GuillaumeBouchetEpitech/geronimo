
#pragma once

#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/FileManager.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace gero {
namespace graphics {

// class UniformBufferObject;

class IBoundShaderProgram;

class IUnboundShaderProgram {
public:
  virtual ~IUnboundShaderProgram() = default;

public:
  virtual int32_t getAttribute(const char* name) const = 0;
  virtual int32_t getUniform(const char* name) const = 0;
  virtual bool hasAttribute(const char* name) const = 0;
  virtual bool hasUniform(const char* name) const = 0;

public:
  virtual void rawBind() const = 0;
  virtual void preBind(const std::function<void(IBoundShaderProgram&)>& callback) = 0;
  virtual void bind(const std::function<void(IBoundShaderProgram&)>& callback) = 0;
};

class IBoundShaderProgram : public IUnboundShaderProgram {
public:
  virtual void setUniform(const char* name, int32_t value) const = 0;
  virtual void setUniform(const char* name, int32_t x, int32_t y) const = 0;
  virtual void setUniform(const char* name, int32_t x, int32_t y, int32_t z) const = 0;
  virtual void setUniform(const char* name, int32_t x, int32_t y, int32_t z, int32_t w) const = 0;
  virtual void setUniform(const char* name, float value) const = 0;
  virtual void setUniform(const char* name, float x, float y) const = 0;
  virtual void setUniform(const char* name, float x, float y, float z) const = 0;
  virtual void setUniform(const char* name, float x, float y, float z, float w) const = 0;
  virtual void setUniform(const char* name, const glm::vec3& vec3) const = 0;
  virtual void setUniform(const char* name, const glm::vec4& vec4) const = 0;
  virtual void setUniform(const char* name, const glm::mat3& mat3) const = 0;
  virtual void setUniform(const char* name, const glm::mat4& mat4) const = 0;

public:
  virtual void setUniform(int32_t location, int32_t value) const = 0;
  virtual void setUniform(int32_t location, int32_t x, int32_t y) const = 0;
  virtual void setUniform(int32_t location, int32_t x, int32_t y, int32_t z) const = 0;
  virtual void setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const = 0;
  virtual void setUniform(int32_t location, float value) const = 0;
  virtual void setUniform(int32_t location, float x, float y) const = 0;
  virtual void setUniform(int32_t location, float x, float y, float z) const = 0;
  virtual void setUniform(int32_t location, float x, float y, float z, float w) const = 0;
  virtual void setUniform(int32_t location, const glm::vec3& vec3) const = 0;
  virtual void setUniform(int32_t location, const glm::vec4& vec4) const = 0;
  virtual void setUniform(int32_t location, const glm::mat3& mat3) const = 0;
  virtual void setUniform(int32_t location, const glm::mat4& mat4) const = 0;

};

class ShaderProgram : public IBoundShaderProgram {
// private:
//   friend UniformBufferObject;

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
  ShaderProgram(const Definition& inDef);
  ShaderProgram(const Definition& inDef, fileUtils::FileManager& fileManager);
  ShaderProgram(const Definition& inDef, const fileUtils::LoadCallback& loadFileCallback);
  ~ShaderProgram();

public:
  static std::shared_ptr<IUnboundShaderProgram> buildUnbound(const Definition& inDef);
  static std::shared_ptr<IUnboundShaderProgram> buildUnbound(const Definition& inDef, fileUtils::FileManager& fileManager);
  static std::shared_ptr<IUnboundShaderProgram> buildUnbound(const Definition& inDef, const fileUtils::LoadCallback& loadFileCallback);

public:
  // void bind() const;
  void rawBind() const override;
  void preBind(const std::function<void(IBoundShaderProgram&)>& callback) override;
  void bind(const std::function<void(IBoundShaderProgram&)>& callback) override;

public:
  static void unbind();

public:
  int32_t getAttribute(const char* name) const override;
  int32_t getUniform(const char* name) const override;
  bool hasAttribute(const char* name) const override;
  bool hasUniform(const char* name) const override;

public:
  void setUniform(const char* name, int32_t value) const override;
  void setUniform(const char* name, int32_t x, int32_t y) const override;
  void setUniform(const char* name, int32_t x, int32_t y, int32_t z) const override;
  void setUniform(const char* name, int32_t x, int32_t y, int32_t z, int32_t w) const override;
  void setUniform(const char* name, float value) const override;
  void setUniform(const char* name, float x, float y) const override;
  void setUniform(const char* name, float x, float y, float z) const override;
  void setUniform(const char* name, float x, float y, float z, float w) const override;
  void setUniform(const char* name, const glm::vec3& vec3) const override;
  void setUniform(const char* name, const glm::vec4& vec4) const override;
  void setUniform(const char* name, const glm::mat3& mat3) const override;
  void setUniform(const char* name, const glm::mat4& mat4) const override;

public:
  void setUniform(int32_t location, int32_t value) const override;
  void setUniform(int32_t location, int32_t x, int32_t y) const override;
  void setUniform(int32_t location, int32_t x, int32_t y, int32_t z) const override;
  void setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const override;
  void setUniform(int32_t location, float value) const override;
  void setUniform(int32_t location, float x, float y) const override;
  void setUniform(int32_t location, float x, float y, float z) const override;
  void setUniform(int32_t location, float x, float y, float z, float w) const override;
  void setUniform(int32_t location, const glm::vec3& vec3) const override;
  void setUniform(int32_t location, const glm::vec4& vec4) const override;
  void setUniform(int32_t location, const glm::mat3& mat3) const override;
  void setUniform(int32_t location, const glm::mat4& mat4) const override;
};

} // namespace graphics
} // namespace gero
