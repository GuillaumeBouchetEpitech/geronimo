
#pragma once

#include "internals/TrianglesStackRenderer.hpp"
#include "internals/WireFramesStackRenderer.hpp"

#include "geronimo/graphics/camera/Camera.hpp"

#include <functional>

namespace gero {
namespace graphics {

class StackRenderers {
public:
  StackRenderers() = default;
  virtual ~StackRenderers() = default;

public:
  void initialize(std::shared_ptr<gero::graphics::ShaderProgram> inShader,
                  const gero::graphics::Geometry::Definition& geoDef);
  void initialize(const gero::graphics::ShaderProgram::Definition& inShaderDef,
                  const gero::graphics::Geometry::Definition& geoDef);
  void initialize(const std::string& inRootPath);

public:
  void setMatricesData(const gero::graphics::Camera::MatricesData& matricesData);

public:
  void flush();
  void safeMode(const std::function<void()>& callback);

public:
  ITrianglesStackRenderer& getTrianglesStack();
  IWireFramesStackRenderer& getWireFramesStack();

private:
  std::shared_ptr<gero::graphics::ShaderProgram> _shader;

  gero::graphics::Camera::MatricesData _matricesData;

  TrianglesStackRenderer _trianglesStackRenderer;
  WireFramesStackRenderer _wireFramesStackRenderer;
};

} // namespace graphics
} // namespace gero
