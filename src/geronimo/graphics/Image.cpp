
#include "Image.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/math/clamp.hpp"

#include "geronimo/helpers/internals/STBImage.hpp"

#include <cstring>

#include <filesystem> // C++17
namespace fs = std::filesystem;

//

namespace gero {
namespace graphics {

Image::~Image() { dispose(); }

Image::Image(const Image& other) {
  if (&other == this)
    return;

  _doCopy(other);

  return;
}

Image& Image::operator=(const Image& other) {
  if (&other == this)
    return *this;

  _doCopy(other);

  return *this;
}

void Image::_doCopy(const Image& other) {
  _size = other._size;
  _rawPixels = nullptr;
  _stbPixels = nullptr;

  if (!other.isValid())
    return;

  const uint32_t bufferSize = _size.x * _size.y * 4;
  _rawPixels = new uint8_t[bufferSize];
  std::memcpy(_rawPixels, other._rawPixels, bufferSize);
}

//

void Image::loadFromFile(const std::string& filename, bool supportNonPowerOfTwo /*= true*/) {
  loadFromFile(fileUtils::getDefaulCallback(), filename, supportNonPowerOfTwo);
}

void Image::loadFromFile(fileUtils::FileManager& fileManager,
                         const std::string& filename,
                         bool supportNonPowerOfTwo /*= true*/) {
  loadFromFile(fileUtils::getFileManagerCallback(fileManager), filename, supportNonPowerOfTwo);
}

void Image::loadFromFile(const fileUtils::LoadCallback& loadFileCallback,
                         const std::string& filename,
                         bool supportNonPowerOfTwo /*= true*/) {
  std::string fileContent;
  loadFileCallback(filename, fileContent);

  loadFromMemory(fileContent, supportNonPowerOfTwo);
}

void Image::loadFromMemory(const std::string& content, bool supportNonPowerOfTwo /*= true*/) {
  dispose();

  int32_t width;
  int32_t height;
  int32_t bpp;
  _stbPixels = stbi_load_from_memory(
    reinterpret_cast<const uint8_t*>(content.c_str()), int32_t(content.size()), &width, &height, &bpp, 0);

  if (!_stbPixels)
    D_THROW(std::runtime_error, "image not found");
  if (bpp != 4)
    D_THROW(std::runtime_error, "image format not supported, bpp: " << bpp);

  if (supportNonPowerOfTwo == false) {
    if ((width & (width - 1)) != 0)
      D_THROW(std::runtime_error, "image width not a power of 2");

    if ((height & (height - 1)) != 0)
      D_THROW(std::runtime_error, "image height not a power of 2");
  }

  _rawPixels = _stbPixels;
  _size.x = uint32_t(width);
  _size.y = uint32_t(height);
}

void Image::dispose() {
  if (!isValid())
    return;

  _size.x = 0;
  _size.y = 0;

  if (_stbPixels) {
    stbi_image_free(_stbPixels);
    _stbPixels = nullptr;
    _rawPixels = nullptr;
  } else if (_rawPixels) {
    delete[] _rawPixels, _rawPixels = nullptr;
  }
}

bool Image::save(const std::string& filename) {
  if (!isValid())
    D_THROW(std::runtime_error, "image not initialized, filename: " << filename);

  return Image::save(filename, _size.x, _size.y, _rawPixels);
}

bool Image::save(const std::string& filename, uint32_t width, uint32_t height, const uint8_t* pixels) {
  fs::path filePath = filename;
  if (filePath.extension() == ".png")
    return stbi_write_png(filename.c_str(), int32_t(width), int32_t(height), 4, pixels, 0);
  if (filePath.extension() == ".jpg")
    return stbi_write_jpg(filename.c_str(), int32_t(width), int32_t(height), 4, pixels, 0);
  if (filePath.extension() == ".bmp")
    return stbi_write_bmp(filename.c_str(), int32_t(width), int32_t(height), 4, pixels);

  D_THROW(std::runtime_error, "unknown file extension, filename: " << filename);
}

void Image::flipY() {
  if (!isValid())
    D_THROW(std::runtime_error, "image not initialized");

  const uint32_t hsize = _size.y / 2;
  for (uint32_t yy = 0; yy < hsize; ++yy)
    for (uint32_t xx = 0; xx < _size.x; ++xx)
      std::swap(_rawPixels[yy * _size.x + xx], _rawPixels[(_size.y - yy) * _size.x + xx]);
}

//

void Image::exploreOriginSubRectangle(const glm::uvec2& inOrigin, const glm::uvec2& inSize, std::function<bool(const glm::uvec2&, glm::uvec4)> inCallback) const
{
  if (!isValid())
    D_THROW(std::runtime_error, "image not initialized");

  glm::ivec2 minCoord = {0, 0};
  glm::ivec2 maxCoord = _size - 1U;

  minCoord.x = gero::math::clamp(int32_t(inOrigin.x), minCoord.x, maxCoord.x);
  minCoord.y = gero::math::clamp(int32_t(inOrigin.y), minCoord.y, maxCoord.y);
  maxCoord.x = gero::math::clamp(int32_t(inOrigin.x + inSize.x), minCoord.x, maxCoord.x);
  maxCoord.y = gero::math::clamp(int32_t(inOrigin.y + inSize.y), minCoord.y, maxCoord.y);

  for (int32_t yy = minCoord.y; yy <= maxCoord.y; ++yy)
  for (int32_t xx = minCoord.x; xx <= maxCoord.x; ++xx)
    if (!inCallback(glm::uvec2(xx, yy), getPixelValues(uint32_t(xx), uint32_t(yy))))
      return;
}

void Image::exploreCenteredSubRectangle(const glm::uvec2& inOrigin, const glm::uvec2& inSize, std::function<bool(const glm::uvec2&, glm::uvec4)> inCallback) const
{
  exploreOriginSubRectangle(inOrigin - inSize / 2U, inSize, inCallback);
}

//

const glm::uvec2& Image::getSize() const { return _size; }
const uint8_t* Image::getPixels() const { return _rawPixels; }

uint8_t& Image::getPixel(uint32_t inX, uint32_t inY, uint32_t inComponent) {
  if (!isValid())
    D_THROW(std::runtime_error, "image not initialized");
  return _rawPixels[(inY * _size.x + inX) * 4 + inComponent];
}

uint8_t Image::getPixel(uint32_t inX, uint32_t inY, uint32_t inComponent) const {
  if (!isValid())
    D_THROW(std::runtime_error, "image not initialized");
  return _rawPixels[(inY * _size.x + inX) * 4 + inComponent];
}

glm::uvec4 Image::getPixelValues(uint32_t inX, uint32_t inY) const
{
  uint32_t tmpIndex = (inY * _size.x + inX) * 4;
  return {
    _rawPixels[tmpIndex + 0],
    _rawPixels[tmpIndex + 1],
    _rawPixels[tmpIndex + 2],
    _rawPixels[tmpIndex + 3]
  };
}

bool Image::isValid() const { return _size.x > 0 && _size.y > 0 && _rawPixels != nullptr; }

} // namespace graphics
} // namespace gero
