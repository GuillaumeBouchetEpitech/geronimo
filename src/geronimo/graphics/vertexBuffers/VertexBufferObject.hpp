
#pragma once

#include <cstdint>
#include <vector>

namespace gero {
namespace graphics {

class VertexBufferObject {
private:
  std::vector<uint32_t> _ids;

public:
  ~VertexBufferObject();

public:
  void allocate(std::size_t size = 1);
  void deallocate();

public:
  bool isAllocated() const;

public:
  void bind(uint32_t index = 0) const;

public:
  static void unbind();
};

} // namespace graphics
} // namespace gero
