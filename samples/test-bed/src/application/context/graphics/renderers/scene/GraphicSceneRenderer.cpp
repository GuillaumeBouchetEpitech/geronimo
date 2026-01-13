
#include "GraphicSceneRenderer.hpp"

void GraphicSceneRenderer::initialize(uint32_t width, uint32_t height) {

  _camera.setPerspective(70.0f, 1.0f, 200.0f);
  _camera.setSize(width, height);

  _stackRenderers.initialize();

  _geometriesStackRenderer.initialize();

  {
    // gero::graphics::PerspectiveClustering::Def clusterDef;
    // clusterDef.clusterSliceX = 15U;
    // clusterDef.clusterSliceY = 15U;
    // clusterDef.clusterSliceZ = 15U;
    // clusterDef.maxLightsPerCluster = 10U;

    // _deferred.initialize("../../src", clusterDef, _camera.getSize());
    _deferred.initialize("../../src", glm::ivec2(width, height), 1.0f);
  }

  {
    //
    // sphere geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeSphere(vertices, 2, 1.0f);
    // gero::graphics::MakeGeometries::convertToPerFacesNormals(vertices);

    _geometriesStackRenderer.createAlias(1111, vertices);
  }

  {
    //
    // sphere geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeSphere(vertices, 1, 1.0f);
    // gero::graphics::MakeGeometries::convertToPerFacesNormals(vertices);

    for (auto& vertex : vertices)
      vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);

    _geometriesStackRenderer.createAlias(1112, vertices);
  }

  {
    //
    // box geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeBox(vertices, 1, 1, 1);

    _geometriesStackRenderer.createAlias(2222, vertices);
    _geometriesStackRenderer.preAllocateAlias(2222, 2048);
  }

  {
    //
    // torus geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makePartialTorus(vertices, 8, 8, 0.6f, 0.1f, true, true, 0, 350);

    _geometriesStackRenderer.createAlias(3333, vertices);
  }

  {
    //
    // cylinder geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeCylinder(vertices, 16, 1.0f, 1.0f, 1.0f, true, true);

    _geometriesStackRenderer.createAlias(4444, vertices);
  }
}

void GraphicSceneRenderer::resize(uint32_t width, uint32_t height) { _camera.setSize(width, height); }

void GraphicSceneRenderer::computeMatrices() {
  _camera.computeMatrices();
  _stackRenderers.setMatricesData(_camera.getMatricesData());
  _geometriesStackRenderer.setMatricesData(_camera.getMatricesData());
}

void GraphicSceneRenderer::lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
  _camera.lookAt(eye, target, up);
}

gero::graphics::ICamera& GraphicSceneRenderer::getCamera() { return _camera; }

gero::graphics::IStackRenderers& GraphicSceneRenderer::getStackRenderers() { return _stackRenderers; }

IGeometriesStackRenderer& GraphicSceneRenderer::getGeometriesStackRenderer() { return _geometriesStackRenderer; }

// gero::graphics::IClusteredDeferred& GraphicSceneRenderer::getDeferred() { return _deferred; }
gero::graphics::DepthDeferred& GraphicSceneRenderer::getDeferred() { return _deferred; }
