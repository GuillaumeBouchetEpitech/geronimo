
#include "Context.hpp"

#include "geronimo/graphics/GeometryBuilder.hpp"
#include "geronimo/graphics/GlContext.hpp"
#include "geronimo/graphics/Image.hpp"
#include "geronimo/graphics/ShaderProgramBuilder.hpp"
#include "geronimo/system/TraceLogger.hpp"

void Context::initializeGraphicResources() {

  D_MYLOG("MaxTextureSize: " << gero::graphics::GlContext::getMaxTextureSize());

  {
    const auto& vSize = graphic.camera.viewportSize;

    graphic.camera.scene.setPerspective(70.0f, 1.0f, 150.0f);
    graphic.camera.scene.setSize(vSize.x, vSize.y);

    graphic.camera.hud.setOrthographic(0.0f, float(vSize.x), 0.0f, float(vSize.y), -10.0f, +10.0f);
    graphic.camera.hud.setSize(vSize.x, vSize.y);

    const glm::vec3 eye = {0.0f, 0.0f, 1.0f};
    const glm::vec3 center = {0.0f, 0.0f, 0.0f};
    const glm::vec3 upAxis = {0.0f, 1.0f, 0.0f};
    graphic.camera.hud.lookAt(eye, center, upAxis);
  }

  // graphic.scene.stackRenderers.initialize(ShadersAliases::stackRendererScene,
  //                                         GeometriesAliases::stackRendererTrianglesScene,
  //                                         GeometriesAliases::stackRendererWireFramesScene);

  graphic.scene.stackRenderers.initialize();

  graphic.scene.geometriesStackRenderer.initialize();

  {
    gero::graphics::PerspectiveClustering::Def clusterDef;
    clusterDef.clusterSliceX = 15U;
    clusterDef.clusterSliceY = 15U;
    clusterDef.clusterSliceZ = 15U;
    clusterDef.maxLightsPerCluster = 10U;

    graphic.scene.deferred.initialize("../../src", clusterDef, graphic.camera.viewportSize);
  }

  // graphic.hud.stackRenderers.initialize(ShadersAliases::stackRendererHud,
  //                                       GeometriesAliases::stackRendererTrianglesHud,
  //                                       GeometriesAliases::stackRendererWireFramesHud);

  graphic.hud.stackRenderers.initialize("../../src");

  graphic.hud.textRenderer.initialize("../../src");

  {
    //
    // sphere geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeSphere(vertices, 2, 1.0f);
    // gero::graphics::MakeGeometries::convertToPerFacesNormals(vertices);

    graphic.scene.geometriesStackRenderer.createAlias(1111, vertices);
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

    graphic.scene.geometriesStackRenderer.createAlias(1112, vertices);
  }

  {
    //
    // box geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeBox(vertices, 1, 1, 1);

    graphic.scene.geometriesStackRenderer.createAlias(2222, vertices);
  }

  {
    //
    // torus geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makePartialTorus(vertices, 8, 8, 0.6f, 0.1f, true, true, 0, 350);

    graphic.scene.geometriesStackRenderer.createAlias(3333, vertices);
  }

  {
    //
    // cylinder geometry
    //

    gero::graphics::MakeGeometries::Vertices vertices;
    gero::graphics::MakeGeometries::makeCylinder(vertices, 16, 1.0f, 1.0f, 1.0f, true, true);

    graphic.scene.geometriesStackRenderer.createAlias(4444, vertices);
  }
}
