
#include "VertexArrayObject.hpp"

#include "../GlContext.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <stdexcept> // <= std::invalid_argument / runtime_error / out_of_range

namespace gero {
namespace graphics {

VertexArrayObject::~VertexArrayObject() {
  if (!_vaoIds.empty())
    deallocate();
}

void VertexArrayObject::allocate(uint32_t size /* = 1 */) {
  if (size < 1)
    D_THROW(std::invalid_argument, "invalid size, input=" << size);

  if (!_vaoIds.empty())
    deallocate();

  _vaoIds.resize(size);
  GlContext::VAO::generateMany(uint32_t(_vaoIds.size()), _vaoIds.data());
}

void VertexArrayObject::deallocate() {
  if (_vaoIds.empty())
    D_THROW(std::runtime_error, "not allocated");

  GlContext::VAO::deleteMany(uint32_t(_vaoIds.size()), _vaoIds.data());
  _vaoIds.clear();
}

void VertexArrayObject::bind(uint32_t index /* = 0 */) const {
  if (_vaoIds.empty())
    D_THROW(std::runtime_error, "not allocated");

  if (index >= uint32_t(_vaoIds.size()))
    D_THROW(std::out_of_range, "index out of range, input=" << index << ", max=" << _vaoIds.size());

  GlContext::VAO::bind(_vaoIds[index]);
}

void VertexArrayObject::unbind() { GlContext::VAO::bind(0); }

bool VertexArrayObject::isAllocated() const { return !_vaoIds.empty(); }

} // namespace graphics
} // namespace gero
