
#pragma once

#include "geometries-stack-renderer/GeometriesStackRenderer.hpp"
#include "scene-stack-renderers/SceneStackRenderers.hpp"

#include "geronimo/graphics/advanced-concept/clusteredDeferred/ClusteredDeferred.hpp"

class IGraphicSceneRenderer
{
public:
  virtual void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) = 0;
public:
  virtual gero::graphics::ICamera& getCamera() = 0;
  virtual gero::graphics::IStackRenderers& getStackRenderers() = 0;
  virtual IGeometriesStackRenderer& getGeometriesStackRenderer() = 0;
  virtual gero::graphics::IClusteredDeferred& getClusteredDeferred() = 0;

};

class GraphicSceneRenderer : public IGraphicSceneRenderer
{
public:
  GraphicSceneRenderer() = default;
  ~GraphicSceneRenderer() = default;

public:
  void initialize(uint32_t width, uint32_t height);
  void resize(uint32_t width, uint32_t height);
  void computeMatrices();

public:
  void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) override;

public:
  gero::graphics::ICamera& getCamera() override;
  gero::graphics::IStackRenderers& getStackRenderers() override;
  IGeometriesStackRenderer& getGeometriesStackRenderer() override;
  gero::graphics::IClusteredDeferred& getClusteredDeferred() override;

private:
  gero::graphics::Camera _camera;

  SceneStackRenderers _stackRenderers;
  GeometriesStackRenderer _geometriesStackRenderer;
  gero::graphics::ClusteredDeferred _deferred;
};

