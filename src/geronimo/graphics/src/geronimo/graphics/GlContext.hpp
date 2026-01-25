
#pragma once

#include "geronimo/helpers/GLMath.hpp"

#include "geronimo/system/asValue.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace gero {
namespace graphics {

namespace GlContext {

//MARK: FrameBuffers
namespace FrameBuffers {

void generateMany(uint32_t total, uint32_t* buffers);
void deleteMany(uint32_t total, const uint32_t* buffers);
uint32_t generateOne();
void deleteOne(uint32_t bufferId);
uint32_t getColorAttachment(uint32_t index);
void attachTexture2D(uint32_t index, uint32_t textureId);
void attachDepthTexture2D(uint32_t textureId);
void attachRenderBuffer(uint32_t bufferId);
bool check(bool throwException = true);
void drawMany(uint32_t total, const uint32_t* buffers);
void bind(uint32_t frameBufferId);
void downloadPixels(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, void* pixels);

} // namespace FrameBuffers

//MARK: VBO
namespace VBO {

enum class Primitives {
  points,
  lines,
  line_strip,
  line_loop,
  triangles,
  triangles_strip,
  triangles_fan,
};

void generateMany(uint32_t total, uint32_t* buffers);
void deleteMany(uint32_t total, const uint32_t* buffers);
void bind(uint32_t vboId);

enum class AttribType {
  Int8,
  UInt8,
  Int16,
  UInt16,
  Int32,
  UInt32,
  Float,
  Double,
};

void enableAttribArray(uint32_t attrId);
void setAttribPointer(uint32_t attrId, uint32_t rowSize, uint32_t stride, uint32_t rowIndex, AttribType type);
void enableAttribDivisor(uint32_t attrId);
void allocateBuffer(uint32_t dataSize, bool dynamic, const void* data);
void uploadBuffer(const void* data, uint32_t dataSize);
void drawArrays(Primitives primitive, uint32_t primitiveStart, uint32_t primitiveCount);
void drawInstancedArrays(Primitives primitive,
                         uint32_t primitiveStart,
                         uint32_t primitiveCount,
                         uint32_t instanceCount);

} // namespace VBO

//MARK: RenderBuffer
namespace RenderBuffer {

enum class DepthFormat {
  depth16,
  depth24,
  depth32,
  depth32f,
};

void generateMany(uint32_t total, uint32_t* buffers);
void deleteMany(uint32_t total, const uint32_t* buffers);
void bind(uint32_t renderBufferId);
void setSize(uint32_t width, uint32_t height, DepthFormat depthFormat);

} // namespace RenderBuffer

//MARK: Shader
namespace Shader {

uint32_t loadVertexShader(const std::string& filename, const std::string& source);
uint32_t loadFragmentShader(const std::string& filename, const std::string& source);
uint32_t createProgram();
void deleteProgram(uint32_t programId);
bool linkProgram(uint32_t programId, uint32_t vertexShader, uint32_t fragmentShader);
int32_t getAttribLocation(uint32_t programId, const char* name);
int32_t getUniformLocation(uint32_t programId, const char* name);
void useProgram(uint32_t programId);
void setUniform(int32_t location, int32_t value);
void setUniform(int32_t location, int32_t x, int32_t y);
void setUniform(int32_t location, int32_t x, int32_t y, int32_t z);
void setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w);
void setUniform(int32_t location, float value);
void setUniform(int32_t location, float x, float y);
void setUniform(int32_t location, float x, float y, float z);
void setUniform(int32_t location, float x, float y, float z, float w);
void setUniform(int32_t location, const glm::mat3& matrix);
void setUniform(int32_t location, const glm::mat4& matrix);

} // namespace Shader

//MARK: Texture
namespace Texture {

enum class DepthFormat {
  depth16,
  depth24,
  depth32,
  depth32f,
};

enum class DepthType {
  unsignedShort,
  unsignedInt,
  float32,
};

void generateMany(uint32_t total, uint32_t* buffers);
void deleteMany(uint32_t total, const uint32_t* buffers);
uint32_t generateOne();
void deleteOne(uint32_t textureId);
void active(uint32_t index);
void bind(uint32_t textureId);

//
//
//

void allocateRgbaUBytesPixels(uint32_t width, uint32_t height, const void* pixels);
void updateRgbaUBytesPixels(const glm::uvec2& origin, const glm::uvec2& size, const void* pixels);

//

void allocateRgbaFloatPixels(uint32_t width, uint32_t height, const void* pixels);
void updateRgbaFloatPixels(const glm::uvec2& origin, const glm::uvec2& size, const void* pixels);

//

void allocateSingleFloatPixels(uint32_t width, uint32_t height, const void* pixels);
void updateSingleFloatPixels(const glm::uvec2& origin, const glm::uvec2& size, const void* pixels);

//

void allocateSingleShortPixels(uint32_t width, uint32_t height, const void* pixels);
void updateSingleShortPixels(const glm::uvec2& origin, const glm::uvec2& size, const void* pixels);

//

void allocateSingleInt32Pixels(uint32_t width, uint32_t height, const void* pixels);
void updateSingleInt32Pixels(const glm::uvec2& origin, const glm::uvec2& size, const void* pixels);

//
//
//

void setAsDepthTexture(uint32_t width, uint32_t height, DepthFormat depthFormat, DepthType depthType);
void setTextureAsRepeat(bool repeat);
void setTextureAsPixelated();
void setTextureAsSmoothed(bool generateMipMap = false);

void setPixelPackAlignment(uint32_t inValueInBytes);
void setPixelUnpackAlignment(uint32_t inValueInBytes);

} // namespace Texture

//MARK: VAO
namespace VAO {

void generateMany(uint32_t total, uint32_t* buffers);
void deleteMany(uint32_t total, const uint32_t* buffers);
void bind(uint32_t vaoId);
} // namespace VAO

//MARK: Viewport/scissor
void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
void setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);

//MARK: Depth
enum class DepthFuncs {
  never,
  less,
  equal,
  lessAndEqual,
  greater,
  notEqual,
  greaterAndEqual,
  always,
};
void setDepthFunc(DepthFuncs func);
void setDepthMask(bool isEnabled);

void setColorMask(bool red, bool green, bool blue, bool alpha);

//MARK: Blending
enum class BlendFuncs {
  one,
  srcAlpha,
  oneMinusSrcAlpha,
};
void setBlendFunc(BlendFuncs srcFactor, BlendFuncs destFactor);

//MARK: Stencil
enum class StencilFuncs {
  // gl.NEVER: Never pass.
  never,
  // gl.LESS: Pass if (ref & mask) < (stencil & mask).
  less,
  // gl.EQUAL: Pass if (ref & mask) = (stencil & mask).
  equal,
  // gl.LEQUAL: Pass if (ref & mask) <= (stencil & mask).
  lessEqual,
  // gl.GREATER: Pass if (ref & mask) > (stencil & mask).
  greater,
  // gl.NOTEQUAL: Pass if (ref & mask) !== (stencil & mask).
  notEqual,
  // gl.GEQUAL: Pass if (ref & mask) >= (stencil & mask).
  greaterEqual,
  // gl.ALWAYS: Always pass.
  always,
};
void setStencilFunc(StencilFuncs srcFactor, int32_t ref, uint32_t mask);

enum class StencilOps {
  // gl.KEEP Keeps the current value.
  keep,
  // gl.ZERO Sets the stencil buffer value to 0.
  zero,
  // gl.REPLACE Sets the stencil buffer value to the reference value as specified by
  // WebGLRenderingContext.stencilFunc().
  replace,
  // gl.INCR Increments the current stencil buffer value. Clamps to the maximum representable unsigned value.
  increment,
  // gl.INCR_WRAP Increments the current stencil buffer value. Wraps stencil buffer value to zero when incrementing the
  // maximum representable unsigned value.
  incrementWrap,
  // gl.DECR Decrements the current stencil buffer value. Clamps to 0.
  decrement,
  // gl.DECR_WRAP Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable
  // unsigned value when decrementing a stencil buffer value of 0.
  decrementWrap,
  // gl.INVERT Inverts the current stencil buffer value bitwise.
  invert,
};
void setStencilOp(StencilOps fail, StencilOps zFail, StencilOps zPass);

//
//
//

//MARK: enable/disable
enum class States {
  cullFace,
  depthTest,
  blend,
  scissorTest,
  stencilTest,
};
void enable(States state);

template <typename... Args> void enables(Args... args) {
  std::array<States, sizeof...(args)> allStates{{args...}};
  for (States& currState : allStates)
    enable(currState);
}

//
//
//

void disable(States state);

template <typename... Args> void disables(Args... args) {
  std::array<States, sizeof...(args)> allStates{{args...}};
  for (States& currState : allStates)
    disable(currState);
}

//
//
//

//MARK: face culling
enum class BackFaceCullingDirection {
  clockWise,
  counterClockWise,
};
void setBackFaceCullingDirection(BackFaceCullingDirection direction);

//
//
//

//MARK: clear
enum class Buffers : int32_t {
  color = 1 << 0,
  depth = 1 << 1,
};

void clear(int32_t mask);

template <typename... Args> void clears(Args... args) {
  std::array<Buffers, sizeof...(args)> allBuffers{{args...}};

  int32_t mask = 0;
  for (Buffers& currBuffer : allBuffers)
    mask |= asValue(currBuffer);

  clear(mask);
}

//
//
//

void clearColor(float r, float g, float b, float a);
void clearDepth(float value);

//
//
//

// static void polygonModeAsLine();
// static void polygonModeAsFill();

//MARK: misc
std::string getVersion();
int32_t getMaxTextureSize();

}; // namespace GlContext

} // namespace graphics
} // namespace gero
