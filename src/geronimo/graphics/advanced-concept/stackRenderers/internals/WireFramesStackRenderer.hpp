
#pragma once

#include "geronimo/graphics/Geometry.hpp"
#include "geronimo/graphics/ShaderProgram.hpp"
#include "geronimo/graphics/camera/Camera.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <memory>
#include <vector>

namespace gero {
namespace graphics {

class IWireFramesStackRenderer {

public:
  virtual void
  pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& colorA, const glm::vec4& colorB) = 0;
  virtual void pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) = 0;
  virtual void
  pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& colorA, const glm::vec3& colorB) = 0;
  virtual void pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) = 0;

public:
  virtual void pushCross(const glm::vec3& pos, const glm::vec3& color, float halfExtent) = 0;

public:
  virtual void
  pushRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color, float depth = 0.0f) = 0;
};

class WireFramesStackRenderer : public IWireFramesStackRenderer, public gero::NonCopyable {

public:
  void initialize(gero::graphics::IUnboundShaderProgram& shader, const gero::graphics::Geometry::Definition& geoDef);

public:
  void
  pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& colorA, const glm::vec4& colorB) override;
  void pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec4& color) override;
  void
  pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& colorA, const glm::vec3& colorB) override;
  void pushLine(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) override;

public:
  void pushCross(const glm::vec3& pos, const glm::vec3& color, float halfExtent) override;

public:
  void pushRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color, float depth = 0.0f) override;

public:
  void flush();

public:
  bool canRender() const;

public:
  void startSafeMode();
  void stopSafeMode();

private:
  struct Vertex {
    glm::vec3 position;
    glm::vec4 color;

    Vertex(const glm::vec3& in_position, const glm::vec4& in_color) : position(in_position), color(in_color) {}
  };
  using Vertices = std::vector<Vertex>;

private:
  gero::graphics::Geometry _geometry;
  Vertices _vertices;

  bool _safeMode = false;
};

} // namespace graphics
} // namespace gero
