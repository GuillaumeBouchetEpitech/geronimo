
#pragma once

#include "Texture.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <vector>

namespace gero {
namespace graphics {

class DataTexture {

private:
  Texture _texture;
  uint32_t _dataLength = 0;
  glm::uvec2 _textureSize = { 0, 0 };

public:
  DataTexture() = default;
  ~DataTexture() = default;

public:
  void allocate(const glm::vec4* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize);
  void allocate(const std::vector<glm::vec4>& data, const glm::uvec2& inTextureSize);

  void update(const glm::vec4* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);
  void update(const std::vector<glm::vec4>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);

public:
  void allocate(const float* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize);
  void allocate(const std::vector<float>& data, const glm::uvec2& inTextureSize);

  void update(const float* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);
  void update(const std::vector<float>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);

public:
  void allocate(const int16_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize);
  void allocate(const std::vector<int16_t>& data, const glm::uvec2& inTextureSize);

  void update(const int16_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);
  void update(const std::vector<int16_t>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);

public:
  void allocate(const int32_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize);
  void allocate(const std::vector<int32_t>& data, const glm::uvec2& inTextureSize);

  void update(const int32_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);
  void update(const std::vector<int32_t>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize);

public:
  uint32_t getDataLength() const;
  const glm::uvec2& getTextureSize() const;
  const Texture& getTexture() const;

private:
  void _validateUpdate(uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize) const;

};

} // namespace graphics
} // namespace gero
