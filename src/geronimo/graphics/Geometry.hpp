
#pragma once

#include "vertexBuffers/VertexArrayObject.hpp"
#include "vertexBuffers/VertexBufferObject.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace gero {
namespace graphics {

class IUnboundShaderProgram;

class Geometry {
public:
  enum class AttrType : uint32_t {

    Int8,
    UInt8,
    Int16,
    UInt16,
    Int32,
    UInt32,

    Float,
    Vec2f,
    Vec3f,
    Vec4f,
    Mat3f,
    Mat4f,

    // Double,
    // Vec2d,
    // Vec3d,
    // Vec4d,
    // Mat3d,
    // Mat4d
  };

  enum class PrimitiveType : uint32_t {
    lines,
    line_strip,
    line_loop,
    triangles,
    triangles_strip,
    triangles_fan,
  };

  struct Definition {
    struct Vbo {
      struct Attr {
        std::string name = "";
        AttrType type = AttrType::Float;
        int32_t index = 0;
        bool ignored = false;

        Attr() = default;
        ~Attr() = default;

        Attr(const std::string& inName, AttrType inType, int32_t inIndex, bool inIgnored)
          : name(inName), type(inType), index(inIndex), ignored(inIgnored) {}
      };

      int32_t stride = -1;
      bool instanced = false;
      bool dynamic = false;

      std::vector<Attr> attrs;

      Vbo() = default;
      ~Vbo() = default;
    };

    std::vector<Vbo> vbos;

    PrimitiveType primitiveType = PrimitiveType::lines;
  };

private:
  Definition _def;
  VertexArrayObject _vao;
  VertexBufferObject _vbo;
  PrimitiveType _primitiveType = PrimitiveType::lines;
  uint32_t _primitiveStart = 0;
  uint32_t _primitiveCount = 0;
  uint32_t _instanceCount = 0;
  bool _isInstanced = false;

public:
  Geometry() = default;

public:
  void initialize(IUnboundShaderProgram& shader, const Definition& def);

public:
  void allocateBuffer(uint32_t index, uint32_t dataSize, const void* data = nullptr);
  void updateBuffer(uint32_t index, const void* data, uint32_t dataSize);
  void updateOrAllocateBuffer(uint32_t index, uint32_t dataSize, const void* data = nullptr);
  void render() const;

public:
  template <typename VertexType> void allocateBuffer(uint32_t index, const std::vector<VertexType>& data) {
    uint32_t dataSize = uint32_t(data.size()) * uint32_t(sizeof(VertexType));
    allocateBuffer(index, dataSize, static_cast<const void*>(data.data()));
  }

  template <typename VertexType>
  void preAllocateBufferFromCapacity(uint32_t index, const std::vector<VertexType>& data) {
    uint32_t dataSize = uint32_t(data.capacity()) * uint32_t(sizeof(VertexType));
    allocateBuffer(index, dataSize, nullptr);
  }

  template <typename VertexType> void updateBuffer(uint32_t index, const std::vector<VertexType>& data) {
    uint32_t dataSize = uint32_t(data.size()) * uint32_t(sizeof(VertexType));
    updateBuffer(index, static_cast<const void*>(data.data()), dataSize);
  }

  template <typename VertexType> void updateOrAllocateBuffer(uint32_t index, const std::vector<VertexType>& data) {
    uint32_t dataSize = uint32_t(data.size()) * uint32_t(sizeof(VertexType));
    updateOrAllocateBuffer(index, dataSize, static_cast<const void*>(data.data()));
  }

  void setPrimitiveStart(uint32_t start);
  void setPrimitiveCount(uint32_t count);
  void setInstancedCount(uint32_t count);

  uint32_t getPrimitiveStart() const;
  uint32_t getPrimitiveCount() const;
  uint32_t getInstancedCount() const;
};

} // namespace graphics
} // namespace gero
