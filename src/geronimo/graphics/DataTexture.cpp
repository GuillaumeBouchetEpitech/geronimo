
#include "DataTexture.hpp"

#include "FrameBuffer.hpp"
#include "Image.hpp"

#include "GlContext.hpp"

#include "geronimo/system/ErrorHandler.hpp"

namespace gero {
namespace graphics {

//
//
//

void DataTexture::allocate(const glm::vec4* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize) {

  _dataLength = inDataLength;
  _textureSize = inTextureSize;

  _validateUpdate(inDataLength, {0, 0}, inTextureSize);

  _texture.allocateBlankRgbaFloat(_textureSize, Texture::Quality::pixelated, Texture::Pattern::clamped, inDataPointer);
}

void DataTexture::allocate(const std::vector<glm::vec4>& data, const glm::uvec2& inTextureSize) {
  allocate(data.data(), uint32_t(data.size()), inTextureSize);
}

void DataTexture::update(const glm::vec4* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  _validateUpdate(inDataLength, inOriginSize, inUpdateSize);

  _texture.updateRgbaFloat(inOriginSize, inUpdateSize, inDataPointer);
}

void DataTexture::update(const std::vector<glm::vec4>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  update(data.data(), uint32_t(data.size()), inOriginSize, inUpdateSize);
}

//
//
//

void DataTexture::allocate(const float* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize)
{
  _dataLength = inDataLength;
  _textureSize = inTextureSize;

  _validateUpdate(inDataLength, {0, 0}, inTextureSize);

  _texture.allocateBlankSingleFloat(_textureSize, inDataPointer);
}

void DataTexture::allocate(const std::vector<float>& data, const glm::uvec2& inTextureSize) {
  allocate(data.data(), uint32_t(data.size()), inTextureSize);
}

void DataTexture::update(const float* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  _validateUpdate(inDataLength, inOriginSize, inUpdateSize);

  _texture.updateSingleFloat(inOriginSize, inUpdateSize, inDataPointer);
}

void DataTexture::update(const std::vector<float>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  update(data.data(), uint32_t(data.size()), inOriginSize, inUpdateSize);
}

//
//
//

void DataTexture::allocate(const int16_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize)
{
  _dataLength = inDataLength;
  _textureSize = inTextureSize;

  _validateUpdate(inDataLength, {0, 0}, inTextureSize);

  _texture.allocateBlankSingleShort(_textureSize, inDataPointer);
}

void DataTexture::allocate(const std::vector<int16_t>& data, const glm::uvec2& inTextureSize) {
  allocate(data.data(), uint32_t(data.size()), inTextureSize);
}

void DataTexture::update(const int16_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  _validateUpdate(inDataLength, inOriginSize, inUpdateSize);

  _texture.updateSingleShort(inOriginSize, inUpdateSize, inDataPointer);
}

void DataTexture::update(const std::vector<int16_t>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  update(data.data(), uint32_t(data.size()), inOriginSize, inUpdateSize);
}

//
//
//

void DataTexture::allocate(const int32_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inTextureSize)
{
  _dataLength = inDataLength;
  _textureSize = inTextureSize;

  _validateUpdate(inDataLength, {0, 0}, inTextureSize);

  _texture.allocateBlankSingleInt32(_textureSize, inDataPointer);
}

void DataTexture::allocate(const std::vector<int32_t>& data, const glm::uvec2& inTextureSize) {
  allocate(data.data(), uint32_t(data.size()), inTextureSize);
}

void DataTexture::update(const int32_t* inDataPointer, uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  _validateUpdate(inDataLength, inOriginSize, inUpdateSize);

  _texture.updateSingleInt32(inOriginSize, inUpdateSize, inDataPointer);
}

void DataTexture::update(const std::vector<int32_t>& data, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize)
{
  update(data.data(), uint32_t(data.size()), inOriginSize, inUpdateSize);
}

//
//
//

uint32_t DataTexture::getDataLength() const { return _dataLength; }
const glm::uvec2& DataTexture::getTextureSize() const { return _textureSize; }
const Texture& DataTexture::getTexture() const { return _texture; }

void DataTexture::_validateUpdate(uint32_t inDataLength, const glm::uvec2& inOriginSize, const glm::uvec2& inUpdateSize) const
{
  const int32_t k_maxTextureWidth = GlContext::getMaxTextureSize();

  if (int32_t(inOriginSize.x) > k_maxTextureWidth)
    D_THROW(std::runtime_error, "data texture origin x too high enough, input: " << inOriginSize.x << ", max: " << k_maxTextureWidth);
  if (int32_t(inOriginSize.y) > k_maxTextureWidth)
    D_THROW(std::runtime_error, "data texture origin y too high enough, input: " << inOriginSize.y << ", max: " << k_maxTextureWidth);

  if (inUpdateSize.x == 0U)
    D_THROW(std::runtime_error, "data texture width cannot be 0");
  if (inUpdateSize.y == 0U)
    D_THROW(std::runtime_error, "data texture height cannot be 0");
  if (int32_t(inOriginSize.x + inUpdateSize.x) > k_maxTextureWidth)
    D_THROW(std::runtime_error, "data texture width too high enough, input: " << (inOriginSize.x + inUpdateSize.x) << ", max: " << k_maxTextureWidth);
  if (int32_t(inOriginSize.y + inUpdateSize.y) > k_maxTextureWidth)
    D_THROW(std::runtime_error, "data texture height too high enough, input: " << (inOriginSize.y + inUpdateSize.y) << ", max: " << k_maxTextureWidth);

  if (inDataLength > 0 && inUpdateSize.y > 1)
  {
    const uint32_t divideDiff = inDataLength / inUpdateSize.x;
    const uint32_t moduloDiff = inDataLength % inUpdateSize.x;
    if (divideDiff != inUpdateSize.y || moduloDiff != 0U)
      D_THROW(std::runtime_error, "data texture size not matching the texture size, data length: " << inDataLength << ", texture size: " << inUpdateSize);
  }
}

} // namespace graphics
} // namespace gero
