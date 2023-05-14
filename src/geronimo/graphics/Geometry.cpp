
#include "Geometry.hpp"

#include "ShaderProgram.hpp"

#include "GlContext.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cstdint>

//

namespace gero {
namespace graphics {

void Geometry::initialize(ShaderProgram& shader, const Definition& def) {
  if (_vao.isAllocated() || _vbo.isAllocated())
    D_THROW(std::runtime_error, "vao/vbo already allocated");

  if (def.vbos.empty())
    D_THROW(std::runtime_error, "empty vbo defintion");

  for (const auto& vbo : def.vbos) {
    const auto& attrs = vbo.attrs;

    if (attrs.empty())
      D_THROW(std::runtime_error, "empty vbo attribute defintion");

    for (const auto& attr : attrs) {
      if (attr.ignored == false && !shader.hasAttribute(attr.name.c_str()))
        D_THROW(std::runtime_error, "attribute not found, name=\"" << attr.name << "\"");
    }
  }

  _def = def;

  _primitiveType = def.primitiveType;

  _vao.allocate();
  _vao.bind();

  _vbo.allocate(_def.vbos.size());

  for (std::size_t ii = 0; ii < _def.vbos.size(); ++ii) {
    const auto& vbo = _def.vbos[ii];
    const auto& attrs = vbo.attrs;

    _vbo.bind(uint32_t(ii));

    uint32_t vboStride = 0;
    if (vbo.stride > 0) {
      vboStride = uint32_t(vbo.stride);
    } else {
      // auto determine stride value
      uint32_t lastAttrIndex = 0;
      for (const auto& attr : attrs) {

        uint32_t liveAttrIndex = attr.index >= 0 ? uint32_t(attr.index) : lastAttrIndex;
        switch (attr.type) {

          //

        case AttrType::Int8:
        case AttrType::UInt8:
          liveAttrIndex += 1;
          break;

        case AttrType::Int16:
        case AttrType::UInt16:
          liveAttrIndex += 1;
          break;

        case AttrType::Int32:
        case AttrType::UInt32:
          liveAttrIndex += 1;
          break;

          //

        case AttrType::Float:
          liveAttrIndex += 1;
          break;
        case AttrType::Vec2f:
          liveAttrIndex += 2;
          break;
        case AttrType::Vec3f:
          liveAttrIndex += 3;
          break;
        case AttrType::Vec4f:
          liveAttrIndex += 4;
          break;
        case AttrType::Mat3f:
          liveAttrIndex += 3 * 3;
          break;
        case AttrType::Mat4f:
          liveAttrIndex += 4 * 4;
          break;

          //

          // case AttrType::Double: liveAttrIndex += 2 * 1; break;
          // case AttrType::Vec2d: liveAttrIndex += 2 * 2; break;
          // case AttrType::Vec3d: liveAttrIndex += 2 * 3; break;
          // case AttrType::Vec4d: liveAttrIndex += 2 * 4; break;
          // case AttrType::Mat3d: liveAttrIndex += 2 * 3 * 3; break;
          // case AttrType::Mat4d: liveAttrIndex += 2 * 4 * 4; break;

          //
        }
        lastAttrIndex = liveAttrIndex;
      }
      vboStride = lastAttrIndex * uint32_t(sizeof(float));
    }

    uint32_t lastAttrIndex = 0;

    for (const auto& attr : attrs) {
      uint32_t rowSize = 1;

      GlContext::VBO::AttribType nextType = GlContext::VBO::AttribType::Float;

      switch (attr.type) {

        //

      case AttrType::Int8:
        nextType = GlContext::VBO::AttribType::Int8;
        rowSize = 1;
        break;
      case AttrType::UInt8:
        nextType = GlContext::VBO::AttribType::UInt8;
        rowSize = 1;
        break;

      case AttrType::Int16:
        nextType = GlContext::VBO::AttribType::Int16;
        rowSize = 2;
        break;
      case AttrType::UInt16:
        nextType = GlContext::VBO::AttribType::UInt16;
        rowSize = 2;
        break;

      case AttrType::Int32:
        nextType = GlContext::VBO::AttribType::Int32;
        rowSize = 4;
        break;
      case AttrType::UInt32:
        nextType = GlContext::VBO::AttribType::UInt32;
        rowSize = 4;
        break;

        //

      case AttrType::Float:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 1;
        break;
      case AttrType::Vec2f:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 2;
        break;
      case AttrType::Vec3f:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 3;
        break;
      case AttrType::Vec4f:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 4;
        break;
      case AttrType::Mat3f:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 3;
        break;
      case AttrType::Mat4f:
        nextType = GlContext::VBO::AttribType::Float;
        rowSize = 4;
        break;

        //

        // case AttrType::Double:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 1 * 2;
        //   break;
        // case AttrType::Vec2d:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 2 * 2;
        //   break;
        // case AttrType::Vec3d:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 3 * 2;
        //   break;
        // case AttrType::Vec4d:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 4 * 2;
        //   break;
        // case AttrType::Mat3d:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 3 * 2;
        //   break;
        // case AttrType::Mat4d:
        //   nextType = GlContext::VBO::AttribType::Double;
        //   rowSize = 4 * 2;
        //   break;

        //
      }

      //

      uint32_t totalRows = 1;

      if (attr.type == AttrType::Mat3f //|| attr.type == AttrType::Mat3d
      ) {
        totalRows = 3;
      } else if (attr.type == AttrType::Mat4f //|| attr.type == AttrType::Mat4d
      ) {
        totalRows = 4;
      }

      // TODO: check if the index is 0 on k>0 and assert/throw on it

      const uint32_t liveAttrIndex = attr.index >= 0 ? uint32_t(attr.index) : lastAttrIndex;

      if (attr.ignored == false) {

        const int32_t attrLocation = shader.getAttribute(attr.name.c_str());

        for (uint32_t kk = 0; kk < totalRows; ++kk) {
          const uint32_t attrId = uint32_t(attrLocation) + kk;
          const uint32_t rowIndex = (liveAttrIndex + kk * rowSize) * uint32_t(sizeof(float));

          GlContext::VBO::enableAttribArray(attrId);
          GlContext::VBO::setAttribPointer(attrId, rowSize, vboStride, rowIndex, nextType);

          if (vbo.instanced) {
            GlContext::VBO::enableAttribDivisor(attrId);

            if (!_isInstanced)
              _isInstanced = true;
          }
        }
      }

      lastAttrIndex = liveAttrIndex + rowSize * totalRows;
    }
  }

  _vao.unbind();
}

void Geometry::allocateBuffer(uint32_t index, uint32_t dataSize, const void* data /*= nullptr*/) {
  if (!_vao.isAllocated() || !_vbo.isAllocated())
    D_THROW(std::runtime_error, "vao/vbo not allocated");

  const auto& vboDef = _def.vbos.at(index);

  _vbo.allocateBuffer(index, dataSize, vboDef.dynamic, data);
}

void Geometry::updateBuffer(uint32_t index, const void* data, uint32_t dataSize) {
  if (!_vao.isAllocated() || !_vbo.isAllocated())
    D_THROW(std::runtime_error, "vao/vbo not allocated");

  _vbo.updateBuffer(index, dataSize, data);
}

void Geometry::updateOrAllocateBuffer(uint32_t index, uint32_t dataSize, const void* data /*= nullptr*/) {
  if (!_vao.isAllocated() || !_vbo.isAllocated())
    D_THROW(std::runtime_error, "vao/vbo not allocated");

  const auto& vboDef = _def.vbos.at(index);

  _vbo.updateOrAllocateBuffer(index, dataSize, vboDef.dynamic, data);
}

void Geometry::render() const {
  if (!_vao.isAllocated() || !_vbo.isAllocated())
    D_THROW(std::runtime_error, "vao/vbo not allocated");

  if (_primitiveCount == 0 || (_isInstanced && _instanceCount == 0))
    return;

  _vao.bind();

  GlContext::VBO::Primitives primitive = GlContext::VBO::Primitives::lines;
  if (_primitiveType == PrimitiveType::triangles)
    primitive = GlContext::VBO::Primitives::triangles;
  else if (_primitiveType == PrimitiveType::line_strip)
    primitive = GlContext::VBO::Primitives::line_strip;

  if (_isInstanced)
    GlContext::VBO::drawInstancedArrays(primitive, _primitiveStart, _primitiveCount, _instanceCount);
  else
    GlContext::VBO::drawArrays(primitive, _primitiveStart, _primitiveCount);

  _vao.unbind();
}

void Geometry::setPrimitiveStart(uint32_t start) { _primitiveStart = start; }

void Geometry::setPrimitiveCount(uint32_t count) { _primitiveCount = count; }

void Geometry::setInstancedCount(uint32_t count) {
  if (!_isInstanced)
    D_THROW(std::runtime_error, "geometry not instanced");

  _instanceCount = count;
}

uint32_t Geometry::getPrimitiveStart() const { return _primitiveStart; }

uint32_t Geometry::getPrimitiveCount() const { return _primitiveCount; }

uint32_t Geometry::getInstancedCount() const { return _instanceCount; }

} // namespace graphics
} // namespace gero
