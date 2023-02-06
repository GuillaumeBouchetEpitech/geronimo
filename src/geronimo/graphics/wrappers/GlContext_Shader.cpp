
#include "../GlContext.hpp"

#include "glCheckError.hpp"

#include "geronimo/helpers/internals/OpenGLES.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/asValue.hpp"

#include <memory>

namespace gero {
namespace graphics {

namespace utilities {

using GetDataFunc = void (*)(uint32_t shader, GLenum pname, int32_t* params);
using GetInfoFunc = void (*)(uint32_t shader, GLsizei maxLength,
                             GLsizei* length, GLchar* infoLog);

void _printInfo(uint32_t id, GetDataFunc getDataFunc, GetInfoFunc getInfoFunc) {
  int32_t infoLogLength = 0;
  glCheck(getDataFunc(id, GL_INFO_LOG_LENGTH, &infoLogLength));

  if (infoLogLength == 0)
    return;

  auto infoLog = std::make_unique<char[]>(std::size_t(infoLogLength));

  GLsizei lengthUsed;
  glCheck(getInfoFunc(id, infoLogLength, &lengthUsed, infoLog.get()));

  if (lengthUsed == 0)
    return;

  D_MYLOG("###################################");
  D_MYLOG("# InfoLog: " << infoLog.get());
  D_MYLOG("###################################");
}

void printShaderInfo(uint32_t shader) {
  utilities::_printInfo(shader, glGetShaderiv, glGetShaderInfoLog);
}

void printProgramInfo(uint32_t program) {
  utilities::_printInfo(program, glGetProgramiv, glGetProgramInfoLog);
}

void printShaderSource(const std::string& filename, const std::string& source) {

  D_MYLOG("######################");
  D_MYLOG("filename: " << filename);

  std::stringstream sstrFile;
  sstrFile << source;

  std::size_t lineNumber = 0;
  std::string currentLine;
  while (std::getline(sstrFile, currentLine)) {
    ++lineNumber;
    D_MYLOG(" " << lineNumber << ": " << currentLine);
  }
  D_MYLOG("######################");
}

uint32_t loadShader(GLenum type, const std::string& filename,
                    const std::string& source) {
  uint32_t shaderId = glCreateShader(type);

  if (shaderId == 0) {
    printShaderSource(filename, source);
    D_THROW(std::runtime_error, "fail to create a shader");
  }

  const char* content = source.c_str();
  const int32_t length = int32_t(source.size());
  glCheck(glShaderSource(shaderId, 1, &content, &length));
  glCheck(glCompileShader(shaderId));

  int32_t compiled;
  glCheck(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled));

  if (!compiled) {
    utilities::printShaderInfo(shaderId);

    glCheck(glDeleteShader(shaderId));

    printShaderSource(filename, source);
    D_THROW(std::runtime_error, "fail to compile a shader");
  }

  return shaderId;
}

} // namespace utilities

namespace GlContext {

namespace Shader {

uint32_t loadVertexShader(const std::string& filename,
                          const std::string& source) {
  return utilities::loadShader(GL_VERTEX_SHADER, filename, source);
}

uint32_t loadFragmentShader(const std::string& filename,
                            const std::string& source) {
  return utilities::loadShader(GL_FRAGMENT_SHADER, filename, source);
}

uint32_t createProgram() {
  uint32_t program;
  glCheck(program = glCreateProgram());
  return program;
}

void deleteProgram(uint32_t programId) { glCheck(glDeleteProgram(programId)); }

bool linkProgram(uint32_t programId, uint32_t vertexShader,
                 uint32_t fragmentShader) {
  glCheck(glAttachShader(programId, vertexShader));
  glCheck(glAttachShader(programId, fragmentShader));
  glCheck(glLinkProgram(programId));
  glCheck(glDeleteShader(vertexShader));
  glCheck(glDeleteShader(fragmentShader));

  int32_t linkedStatus;
  glCheck(glGetProgramiv(programId, GL_LINK_STATUS, &linkedStatus));

  bool success = (linkedStatus != GL_FALSE);

  if (!success)
    utilities::printProgramInfo(programId);

  return success;
}

int32_t getAttribLocation(uint32_t programId, const char* name) {
  int32_t location;
  glCheck(location = glGetAttribLocation(programId, name));
  return location;
}

int32_t getUniformLocation(uint32_t programId, const char* name) {
  int32_t location;
  glCheck(location = glGetUniformLocation(programId, name));
  return location;
}

void useProgram(uint32_t programId) { glCheck(glUseProgram(programId)); }

void setUniform(int32_t location, int32_t value) {
  glCheck(glUniform1i(location, value));
}

void setUniform(int32_t location, int32_t x, int32_t y) {
  glCheck(glUniform2i(location, x, y));
}

void setUniform(int32_t location, int32_t x, int32_t y, int32_t z) {
  glCheck(glUniform3i(location, x, y, z));
}

void setUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) {
  glCheck(glUniform4i(location, x, y, z, w));
}

void setUniform(int32_t location, float value) {
  glCheck(glUniform1f(location, value));
}

void setUniform(int32_t location, float x, float y) {
  glCheck(glUniform2f(location, x, y));
}

void setUniform(int32_t location, float x, float y, float z) {
  glCheck(glUniform3f(location, x, y, z));
}

void setUniform(int32_t location, float x, float y, float z, float w) {
  glCheck(glUniform4f(location, x, y, z, w));
}

void setUniform(int32_t location, const glm::mat3& matrix) {
  glCheck(glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix)));
}

void setUniform(int32_t location, const glm::mat4& matrix) {
  glCheck(glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix)));
}

} // namespace Shader

} // namespace GlContext

} // namespace graphics
} // namespace gero
