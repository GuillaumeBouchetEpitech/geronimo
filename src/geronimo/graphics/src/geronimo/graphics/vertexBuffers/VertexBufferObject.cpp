
#include "VertexBufferObject.hpp"

#include "../GlContext.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cstdint>
#include <stdexcept> // <= std::invalid_argument / runtime_error / out_of_range

namespace gero {
namespace graphics {

VertexBufferObject::~VertexBufferObject() {
  if (!_ids.empty())
    deallocate();
}

void VertexBufferObject::allocate(std::size_t size /* = 1 */) {
  if (size == 0)
    D_THROW(std::invalid_argument, "invalid size, input=" << size);

  if (!_ids.empty())
    deallocate();

  _ids.resize(size);
  GlContext::VBO::generateMany(uint32_t(_ids.size()), _ids.data());
  _buffersData.resize(size);
}

void VertexBufferObject::deallocate() {
  if (_ids.empty())
    D_THROW(std::runtime_error, "not allocated");

  GlContext::VBO::deleteMany(uint32_t(_ids.size()), _ids.data());
  _ids.clear();
  _buffersData.clear();
}

bool VertexBufferObject::isAllocated() const { return !_ids.empty(); }

void VertexBufferObject::bind(uint32_t index /* = 0 */) const {
  if (_ids.empty())
    D_THROW(std::runtime_error, "not allocated");

  if (index >= uint32_t(_ids.size()))
    D_THROW(std::out_of_range, "index out of range, input=" << index << ", max=" << _ids.size());

  GlContext::VBO::bind(_ids[index]);
}

void VertexBufferObject::allocateBuffer(uint32_t index,
                                        uint32_t dataSize,
                                        bool dynamic,
                                        const void* data /*= nullptr*/) {
  // uint32_t allocatedSize = _buffersData.at(index).allocatedSize;
  // if (dataSize < allocatedSize)
  //   return;

  bind(index);

  _buffersData.at(index).allocatedSize = dataSize;

  GlContext::VBO::allocateBuffer(dataSize, dynamic, data);

  unbind();
}

void VertexBufferObject::updateBuffer(uint32_t index, uint32_t dataSize, const void* data) {
  bind(index);

  uint32_t allocatedSize = _buffersData.at(index).allocatedSize;
  if (dataSize > allocatedSize) {
    D_THROW(std::out_of_range, "update buffer size out of range, input=" << dataSize << ", max=" << allocatedSize);
  }

  GlContext::VBO::uploadBuffer(data, dataSize);

  unbind();
}

void VertexBufferObject::updateOrAllocateBuffer(uint32_t index,
                                                uint32_t dataSize,
                                                bool dynamic,
                                                const void* data /*= nullptr*/) {
  bind(index);

  uint32_t allocatedSize = _buffersData.at(index).allocatedSize;

  if (dataSize > allocatedSize) {
    _buffersData.at(index).allocatedSize = dataSize;

    GlContext::VBO::allocateBuffer(dataSize, dynamic, data);
  } else {
    GlContext::VBO::uploadBuffer(data, dataSize);
  }

  unbind();
}

void VertexBufferObject::unbind() { GlContext::VBO::bind(0); }

} // namespace graphics
} // namespace gero
