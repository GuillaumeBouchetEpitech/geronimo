
#pragma once

#include "ShaderProgram.hpp"

#include <memory>

namespace gero {
namespace graphics {

class ShaderProgramBuilder {
private:
  ShaderProgram::Definition _def;

public:
  ShaderProgramBuilder();
  ~ShaderProgramBuilder() = default;

public:
  ShaderProgramBuilder& reset();

  ShaderProgramBuilder& setVertexFilename(const std::string& filename);
  ShaderProgramBuilder& setFragmentFilename(const std::string& filename);
  ShaderProgramBuilder& addAttribute(const std::string& name);
  ShaderProgramBuilder& addUniform(const std::string& name);

  ShaderProgramBuilder& validate();

  std::shared_ptr<ShaderProgram> build();
  std::shared_ptr<IUnboundShaderProgram> buildUnbound();
  ShaderProgram::Definition getDefinition();
};

} // namespace graphics
} // namespace gero
