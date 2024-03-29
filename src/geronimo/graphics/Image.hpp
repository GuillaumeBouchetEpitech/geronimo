
#pragma once

#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/FileManager.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>

namespace gero {
namespace graphics {

class FrameBuffer;

class Image {
  friend FrameBuffer;

private:
  glm::uvec2 _size = {0, 0};
  uint8_t* _stbPixels = nullptr;
  uint8_t* _rawPixels = nullptr;

public:
  Image() = default;
  ~Image();

  Image(const Image& other);
  Image& operator=(const Image& other);

private:
  void _doCopy(const Image& other);

public:
  void loadFromFile(const std::string& filename, bool supportNonPowerOfTwo = true);
  void loadFromFile(const fileUtils::LoadCallback& loadFileCallback,
                    const std::string& filename,
                    bool supportNonPowerOfTwo = true);
  void loadFromFile(fileUtils::FileManager& fileManager, const std::string& filename, bool supportNonPowerOfTwo = true);
  void loadFromMemory(const std::string& content, bool supportNonPowerOfTwo = true);

public:
  void dispose();

public:
  bool save(const std::string& filename);

  static bool save(const std::string& filename, uint32_t width, uint32_t height, const uint8_t* pixels);

public:
  void flipY();

public:
  void exploreOriginSubRectangle(const glm::uvec2& inOrigin,
                                 const glm::uvec2& inSize,
                                 std::function<bool(const glm::uvec2&, glm::uvec4)> inCallback) const;
  void exploreCenteredSubRectangle(const glm::uvec2& inOrigin,
                                   const glm::uvec2& inSize,
                                   std::function<bool(const glm::uvec2&, glm::uvec4)> inCallback) const;

public:
  const glm::uvec2& getSize() const;
  const uint8_t* getPixels() const;
  uint8_t& getPixel(uint32_t inX, uint32_t inY, uint32_t inComponent);
  uint8_t getPixel(uint32_t inX, uint32_t inY, uint32_t inComponent) const;
  glm::uvec4 getPixelValues(uint32_t inX, uint32_t inY) const;
  bool isValid() const;
};

} // namespace graphics
} // namespace gero
