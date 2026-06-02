
#pragma once

#include "internals/TrianglesStackRenderer.hpp"
#include "internals/WireFramesStackRenderer.hpp"

#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <functional>

namespace gero {
namespace graphics {
namespace opengl {
namespace advanced {

class IStackRenderers {
public:
  virtual ~IStackRenderers() = default;

public:
public:
  virtual void setMatricesData(const gero::graphics::camera::ICamera::MatricesData& matricesData) = 0;

public:
  virtual void flush() = 0;
  virtual void safeMode(const std::function<void()>& callback) = 0;

public:
  virtual stackRenderers::ITrianglesStackRenderer& getTrianglesStack() = 0;
  virtual stackRenderers::IWireFramesStackRenderer& getWireFramesStack() = 0;
};

class StackRenderers : public IStackRenderers, public gero::NonCopyable {
public:
  void initialize(std::shared_ptr<gero::graphics::opengl::IUnboundShaderProgram> inShader,
                  const gero::graphics::opengl::Geometry::Definition& geoDef);
  void initialize(const gero::graphics::opengl::ShaderProgram::Definition& inShaderDef,
                  const gero::graphics::opengl::Geometry::Definition& geoDef);
  void initialize(const std::string& inRootPath);

public:
  void setMatricesData(const gero::graphics::camera::ICamera::MatricesData& matricesData) override;

public:
  void flush() override;
  void safeMode(const std::function<void()>& callback) override;

public:
  stackRenderers::ITrianglesStackRenderer& getTrianglesStack() override;
  stackRenderers::IWireFramesStackRenderer& getWireFramesStack() override;

private:
  std::shared_ptr<gero::graphics::opengl::IUnboundShaderProgram> _shader;

  gero::graphics::camera::ICamera::MatricesData _matricesData;

  stackRenderers::TrianglesStackRenderer _trianglesStackRenderer;
  stackRenderers::WireFramesStackRenderer _wireFramesStackRenderer;
};

} // namespace advanced
} // namespace opengl
} // namespace graphics
} // namespace gero
