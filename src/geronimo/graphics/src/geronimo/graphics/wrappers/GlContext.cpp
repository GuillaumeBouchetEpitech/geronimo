
#include "../GlContext.hpp"

#include "glCheckError.hpp"

#include "geronimo/helpers/internals/OpenGLES.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <memory>

namespace {
int32_t s_maxTextureSize = -1;
}

namespace gero {
namespace graphics {

namespace GlContext {

//
//
//
//
//

namespace RenderBuffer {

void generateMany(uint32_t total, uint32_t* buffers) { glCheck(glGenRenderbuffers(GLsizei(total), buffers)); }

void deleteMany(uint32_t total, const uint32_t* buffers) { glCheck(glDeleteRenderbuffers(GLsizei(total), buffers)); }

void bind(uint32_t renderBufferId) { glCheck(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId)); }

namespace {
int32_t getRawDepthFormat(DepthFormat depthFormat) {
  switch (depthFormat) {
  case DepthFormat::depth24:
    return GL_DEPTH_COMPONENT24;
  case DepthFormat::depth32:
    return GL_DEPTH_COMPONENT32;
  case DepthFormat::depth32f:
    return GL_DEPTH_COMPONENT32F;
  default:
    return GL_DEPTH_COMPONENT16;
  }
}
} // namespace

void setSize(uint32_t width, uint32_t height, DepthFormat depthFormat) {

  const int32_t internalFormat = getRawDepthFormat(depthFormat);

  glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GLenum(internalFormat), GLsizei(width), GLsizei(height)));
}

} // namespace RenderBuffer

//
//
//
//
//

namespace VAO {

void generateMany(uint32_t total, uint32_t* buffers) { glCheck(glGenVertexArrays(GLsizei(total), buffers)); }

void deleteMany(uint32_t total, const uint32_t* buffers) { glCheck(glDeleteVertexArrays(GLsizei(total), buffers)); }

void bind(uint32_t vaoId) { glCheck(glBindVertexArray(vaoId)); }

} // namespace VAO

//
//
//
//
//

void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) {
  glCheck(glViewport(x, y, GLsizei(width), GLsizei(height)));
}

void setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) {
  glCheck(glScissor(x, y, GLsizei(width), GLsizei(height)));
}

void setDepthFunc(DepthFuncs func) {
  switch (func) {
  case DepthFuncs::never:
    glCheck(glDepthFunc(GL_NEVER));
    break;
  case DepthFuncs::less:
    glCheck(glDepthFunc(GL_LESS));
    break;
  case DepthFuncs::equal:
    glCheck(glDepthFunc(GL_EQUAL));
    break;
  case DepthFuncs::lessAndEqual:
    glCheck(glDepthFunc(GL_LEQUAL));
    break;
  case DepthFuncs::greater:
    glCheck(glDepthFunc(GL_GREATER));
    break;
  case DepthFuncs::notEqual:
    glCheck(glDepthFunc(GL_NOTEQUAL));
    break;
  case DepthFuncs::greaterAndEqual:
    glCheck(glDepthFunc(GL_GEQUAL));
    break;
  case DepthFuncs::always:
    glCheck(glDepthFunc(GL_ALWAYS));
    break;

  default:
    D_THROW(std::runtime_error, "unknown depth func");
    break;
  }
}

void setDepthMask(bool isEnabled) { glDepthMask(isEnabled ? GL_TRUE : GL_FALSE); }

void setColorMask(bool red, bool green, bool blue, bool alpha) {
#define D_GL_BOOL(_tmp_val) (_tmp_val ? GL_TRUE : GL_FALSE)
  glColorMask(D_GL_BOOL(red), D_GL_BOOL(green), D_GL_BOOL(blue), D_GL_BOOL(alpha));
#undef D_GL_BOOL
}

namespace {
GLenum getRawBlendFunc(BlendFuncs func) {
  switch (func) {
  case BlendFuncs::one:
    return GL_ONE;
    break;
  case BlendFuncs::srcAlpha:
    return GL_SRC_ALPHA;
    break;
  case BlendFuncs::oneMinusSrcAlpha:
    return GL_ONE_MINUS_SRC_ALPHA;
    break;

  default:
    D_THROW(std::runtime_error, "unknown blend func");
    break;
  }
}
} // namespace

void setBlendFunc(BlendFuncs sfactor, BlendFuncs dfactor) {
  glCheck(glBlendFunc(getRawBlendFunc(sfactor), getRawBlendFunc(dfactor)));
}

namespace {
GLenum getRawStencilFunc(StencilFuncs func) {
  switch (func) {
  case StencilFuncs::never:
    return GL_NEVER;
    break;
  case StencilFuncs::less:
    return GL_LESS;
    break;
  case StencilFuncs::equal:
    return GL_EQUAL;
    break;
  case StencilFuncs::lessEqual:
    return GL_LEQUAL;
    break;
  case StencilFuncs::greater:
    return GL_GREATER;
    break;
  case StencilFuncs::notEqual:
    return GL_NOTEQUAL;
    break;
  case StencilFuncs::greaterEqual:
    return GL_GEQUAL;
    break;
  case StencilFuncs::always:
    return GL_ALWAYS;
    break;

  default:
    D_THROW(std::runtime_error, "unknown stencil func");
    break;
  }
}
} // namespace

void setStencilFunc(StencilFuncs srcFactor, int32_t ref, uint32_t mask) {
  glCheck(glStencilFunc(getRawStencilFunc(srcFactor), ref, mask));
}

namespace {
GLenum getRawStencilOp(StencilOps func) {
  switch (func) {

  case StencilOps::keep:
    return GL_KEEP;
    break;
  case StencilOps::zero:
    return GL_ZERO;
    break;
  case StencilOps::replace:
    return GL_REPLACE;
    break;
  case StencilOps::increment:
    return GL_INCR;
    break;
  case StencilOps::incrementWrap:
    return GL_INCR_WRAP;
    break;
  case StencilOps::decrement:
    return GL_DECR;
    break;
  case StencilOps::decrementWrap:
    return GL_DECR_WRAP;
    break;
  case StencilOps::invert:
    return GL_INVERT;
    break;
  default:
    D_THROW(std::runtime_error, "unknown stencil op");
    break;
  }
}
} // namespace

void setStencilOp(StencilOps fail, StencilOps zFail, StencilOps zPass) {
  glStencilOp(getRawStencilOp(fail), getRawStencilOp(zFail), getRawStencilOp(zPass));
}

void enable(States state) {
  switch (state) {
  case States::cullFace:
    glCheck(glEnable(GL_CULL_FACE));
    break;
  case States::depthTest:
    glCheck(glEnable(GL_DEPTH_TEST));
    break;
  case States::blend:
    glCheck(glEnable(GL_BLEND));
    break;
  case States::scissorTest:
    glCheck(glEnable(GL_SCISSOR_TEST));
    break;
  case States::stencilTest:
    glCheck(glEnable(GL_STENCIL_TEST));
    break;

  default:
    D_THROW(std::runtime_error, "unknown state");
    break;
  }
}

void disable(States state) {
  switch (state) {
  case States::cullFace:
    glCheck(glDisable(GL_CULL_FACE));
    break;
  case States::depthTest:
    glCheck(glDisable(GL_DEPTH_TEST));
    break;
  case States::blend:
    glCheck(glDisable(GL_BLEND));
    break;
  case States::scissorTest:
    glCheck(glDisable(GL_SCISSOR_TEST));
    break;
  case States::stencilTest:
    glCheck(glDisable(GL_STENCIL_TEST));
    break;

  default:
    D_THROW(std::runtime_error, "unknown state");
    break;
  }
}

void setBackFaceCullingDirection(BackFaceCullingDirection direction) {
  switch (direction) {
  case BackFaceCullingDirection::clockWise:
    glCheck(glFrontFace(GL_CW));
    break;
  case BackFaceCullingDirection::counterClockWise:
    glCheck(glFrontFace(GL_CCW));
    break;

  default:
    D_THROW(std::runtime_error, "unknown backface culling direction");
    break;
  }
}

void clear(int32_t mask) {
  int32_t rawMask = 0;
  if (mask & asValue(Buffers::color))
    rawMask |= GL_COLOR_BUFFER_BIT;
  if (mask & asValue(Buffers::depth))
    rawMask |= GL_DEPTH_BUFFER_BIT;

  if (mask == 0) {
    D_THROW(std::runtime_error, "clear: empty mask");
  }

  glCheck(glClear(GLbitfield(rawMask)));
}

void clearColor(float r, float g, float b, float a) { glCheck(glClearColor(r, g, b, a)); }

void clearDepth(float value) { glCheck(glClearDepthf(value)); }

// void polygonModeAsLine()
// {
//   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// }

// void polygonModeAsFill()
// {
//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// }

std::string getVersion() {
  const uint8_t* msg = glGetString(GL_VERSION);
  return std::string(reinterpret_cast<const char*>(msg));
}

int32_t getMaxTextureSize() {
  if (s_maxTextureSize < 0) {
    glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize));
  }
  return s_maxTextureSize;
}

} // namespace GlContext

} // namespace graphics
} // namespace gero
