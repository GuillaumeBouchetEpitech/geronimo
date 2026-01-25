
#pragma once

#include <cstdint>
#include <vector>

namespace gero {
namespace graphics {

class VertexBufferObject {
private:
  struct BufferData {
    uint32_t allocatedSize = 0;
  };

  std::vector<uint32_t> _ids;
  std::vector<BufferData> _buffersData;

public:
  ~VertexBufferObject();

public:
  void allocate(std::size_t size = 1);
  void deallocate();

public:
  void allocateBuffer(uint32_t index, uint32_t dataSize, bool dynamic, const void* data = nullptr);
  void updateBuffer(uint32_t index, uint32_t dataSize, const void* data);
  void updateOrAllocateBuffer(uint32_t index, uint32_t dataSize, bool dynamic, const void* data = nullptr);

public:
  bool isAllocated() const;

public:
  void bind(uint32_t index = 0) const;

public:
  static void unbind();
};

} // namespace graphics
} // namespace gero
