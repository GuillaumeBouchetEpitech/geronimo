
#pragma once

#include "internals/TrianglesStackRenderer.hpp"
#include "internals/WireFramesStackRenderer.hpp"

#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <functional>

namespace gero {
namespace graphics {

class IStackRenderers {
public:
  virtual ~IStackRenderers() = default;

public:
public:
  virtual void setMatricesData(const gero::graphics::ICamera::MatricesData& matricesData) = 0;

public:
  virtual void flush() = 0;
  virtual void safeMode(const std::function<void()>& callback) = 0;

public:
  virtual ITrianglesStackRenderer& getTrianglesStack() = 0;
  virtual IWireFramesStackRenderer& getWireFramesStack() = 0;
};

class StackRenderers : public IStackRenderers, public gero::NonCopyable {
public:
  void initialize(std::shared_ptr<gero::graphics::IUnboundShaderProgram> inShader,
                  const gero::graphics::Geometry::Definition& geoDef);
  void initialize(const gero::graphics::ShaderProgram::Definition& inShaderDef,
                  const gero::graphics::Geometry::Definition& geoDef);
  void initialize(const std::string& inRootPath);

public:
  void setMatricesData(const gero::graphics::ICamera::MatricesData& matricesData) override;

public:
  void flush() override;
  void safeMode(const std::function<void()>& callback) override;

public:
  ITrianglesStackRenderer& getTrianglesStack() override;
  IWireFramesStackRenderer& getWireFramesStack() override;

private:
  std::shared_ptr<gero::graphics::IUnboundShaderProgram> _shader;

  gero::graphics::ICamera::MatricesData _matricesData;

  TrianglesStackRenderer _trianglesStackRenderer;
  WireFramesStackRenderer _wireFramesStackRenderer;
};

} // namespace graphics
} // namespace gero
