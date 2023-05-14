
#include "../GlContext.hpp"

#include "glCheckError.hpp"

#include "geronimo/helpers/internals/OpenGLES.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/asValue.hpp"

#include <memory>

namespace gero {
namespace graphics {

namespace GlContext {

namespace VBO {

void generateMany(uint32_t total, uint32_t* buffers) { glCheck(glGenBuffers(GLsizei(total), buffers)); }

void deleteMany(uint32_t total, const uint32_t* buffers) { glCheck(glDeleteBuffers(GLsizei(total), buffers)); }

void bind(uint32_t vboId) { glCheck(glBindBuffer(GL_ARRAY_BUFFER, vboId)); }

void enableAttribArray(uint32_t attrId) { glCheck(glEnableVertexAttribArray(attrId)); }

void setAttribPointer(uint32_t attrId, uint32_t rowSize, uint32_t stride, uint32_t rowIndex, AttribType inType) {

  const void* rowAddr = reinterpret_cast<void*>(rowIndex);

  GLenum rawType = GL_FLOAT;
  switch (inType) {
  case AttribType::Int8:
    rawType = GL_BYTE;
    break;
  case AttribType::UInt8:
    rawType = GL_UNSIGNED_BYTE;
    break;
  case AttribType::Int16:
    rawType = GL_SHORT;
    break;
  case AttribType::UInt16:
    rawType = GL_UNSIGNED_SHORT;
    break;
  case AttribType::Int32:
    rawType = GL_INT;
    break;
  case AttribType::UInt32:
    rawType = GL_UNSIGNED_INT;
    break;
  case AttribType::Float:
    rawType = GL_FLOAT;
    break;
  case AttribType::Double:
    rawType = GL_DOUBLE;
    break;
  };

  glCheck(glVertexAttribPointer(attrId, GLsizei(rowSize), rawType, GL_FALSE, GLsizei(stride), rowAddr));
}

void enableAttribDivisor(uint32_t attrId) { glCheck(glVertexAttribDivisor(attrId, 1)); }

void allocateBuffer(uint32_t dataSize, bool dynamic, const void* data) {
  const GLenum usage = (dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

  glCheck(glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(dataSize), data, GLenum(usage)));
}

void uploadBuffer(const void* data, uint32_t dataSize) {
  glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, GLsizeiptr(dataSize), data));
}

void drawArrays(Primitives primitive, uint32_t primitiveStart, uint32_t primitiveCount) {
  int32_t rawPrimitive = GL_LINES;
  switch (primitive) {
  case Primitives::lines:
    rawPrimitive = GL_LINES;
    break;
  case Primitives::line_strip:
    rawPrimitive = GL_LINE_STRIP;
    break;
  case Primitives::triangles:
    rawPrimitive = GL_TRIANGLES;
    break;
  default:
    D_THROW(std::runtime_error, "unknown primitive");
    break;
  }
  glCheck(glDrawArrays(GLenum(rawPrimitive), GLint(primitiveStart), GLsizei(primitiveCount)));
}

void drawInstancedArrays(Primitives primitive,
                         uint32_t primitiveStart,
                         uint32_t primitiveCount,
                         uint32_t instanceCount) {
  int32_t rawPrimitive = GL_LINES;
  switch (primitive) {
  case Primitives::lines:
    rawPrimitive = GL_LINES;
    break;
  case Primitives::line_strip:
    rawPrimitive = GL_LINE_STRIP;
    break;
  case Primitives::triangles:
    rawPrimitive = GL_TRIANGLES;
    break;
  default:
    D_THROW(std::runtime_error, "unknown primitive");
    break;
  }
  glCheck(glDrawArraysInstanced(
    GLenum(rawPrimitive), GLint(primitiveStart), GLsizei(primitiveCount), GLsizei(instanceCount)));
}

} // namespace VBO

} // namespace GlContext

} // namespace graphics
} // namespace gero
