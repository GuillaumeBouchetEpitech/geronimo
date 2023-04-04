
#include "Context.hpp"

#include "application/defines.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/physics/body/PhysicBody.hpp"
#include "geronimo/physics/vehicle/PhysicVehicle.hpp"
#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/TraceLogger.hpp"

#include <iomanip>
#include <sstream>

//
//
// singleton

Context* Context::_instance = nullptr;

Context::~Context() {

  delete physic.world;
  delete audio.soundManager;
}

void Context::initialize(uint32_t width, uint32_t height) {
  graphic.camera.viewportSize = {width, height};

  initializeGraphicResources();

  graphic.scene.stackRenderers.initialize(ShadersAliases::stackRendererScene,
                                          GeometriesAliases::stackRendererTrianglesScene,
                                          GeometriesAliases::stackRendererWireFramesScene);

  graphic.scene.geometriesStackRenderer.initialize();

  graphic.scene.deferred.initialize({width, height});

  graphic.hud.stackRenderers.initialize(ShadersAliases::stackRendererHud,
                                        GeometriesAliases::stackRendererTrianglesHud,
                                        GeometriesAliases::stackRendererWireFramesHud);

  graphic.hud.textRenderer.initialize();

  physic.world = new gero::physics::PhysicWorld();
  physic.world->setDebuggerPushLine([this](const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& color) {
    graphic.scene.stackRenderers.getWireFramesStack().pushLine(posA, posB, color);
  });

  {
    logic.heightField.generate(32, 1.5f, 20.0f);

    const uint32_t fieldSize = logic.heightField.getFieldSize();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<int32_t> indices;

    {
      vertices.reserve(2048 * 4);
      indices.reserve(2048 * 6);

      for (uint32_t yy = 0; yy + 1 < fieldSize; ++yy)
        for (uint32_t xx = 0; xx + 1 < fieldSize; ++xx) {
          const int32_t index = int32_t(vertices.size());

          vertices.push_back(logic.heightField.getPosition(xx + 0, yy + 0));
          vertices.push_back(logic.heightField.getPosition(xx + 1, yy + 0));
          vertices.push_back(logic.heightField.getPosition(xx + 1, yy + 1));
          vertices.push_back(logic.heightField.getPosition(xx + 0, yy + 1));

          normals.push_back(logic.heightField.getNormal(xx + 0, yy + 0));
          normals.push_back(logic.heightField.getNormal(xx + 1, yy + 0));
          normals.push_back(logic.heightField.getNormal(xx + 1, yy + 1));
          normals.push_back(logic.heightField.getNormal(xx + 0, yy + 1));

          //

          indices.push_back(index + 1);
          indices.push_back(index + 2);
          indices.push_back(index + 3);

          indices.push_back(index + 1);
          indices.push_back(index + 3);
          indices.push_back(index + 0);
        }
    }

    const float arenaHSize = logic.heightField.getArenaSize() * 0.5f;

    {
      gero::physics::PhysicBodyDef def;

      def.shape.type = gero::physics::PhysicShapeDef::Type::staticMesh;
      def.shape.data.staticMesh.verticesData = vertices.data();
      def.shape.data.staticMesh.verticesLength = vertices.size();
      def.shape.data.staticMesh.indicesData = indices.data();
      def.shape.data.staticMesh.indicesLength = indices.size();

      def.mass = 0.0f;
      def.group = gero::asValue(Groups::ground);
      def.mask = gero::asValue(Masks::ground);

      auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

      body->setPosition({-arenaHSize, -arenaHSize, 0});
      body->setFriction(0.5f);
      body->setUserValue(0);
    }

    {

      const glm::vec3 offset = glm::vec3(-arenaHSize, -arenaHSize, 0.0f);

      gero::graphics::MakeGeometries::Vertices geoVertices;
      vertices.reserve(1024 * 8);

      for (std::size_t ii = 0; ii < indices.size(); ii += 3) {
        const int32_t index1 = indices.at(ii + 0);
        const int32_t index2 = indices.at(ii + 1);
        const int32_t index3 = indices.at(ii + 2);

        const glm::vec3 pos1 = vertices.at(std::size_t(index1)) + offset;
        const glm::vec3 pos2 = vertices.at(std::size_t(index2)) + offset;
        const glm::vec3 pos3 = vertices.at(std::size_t(index3)) + offset;

        const glm::vec3 normal1 = normals.at(std::size_t(index1));
        const glm::vec3 normal2 = normals.at(std::size_t(index2));
        const glm::vec3 normal3 = normals.at(std::size_t(index3));

        geoVertices.push_back({pos1, normal1});
        geoVertices.push_back({pos2, normal2});
        geoVertices.push_back({pos3, normal3});
      }

      // gero::graphics::MakeGeometries::convertToPerFacesNormals(geoVertices);

      graphic.scene.geometriesStackRenderer.createAlias(100, geoVertices);
    }
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::box;
    def.shape.data.box.size = {75, 75, 1};
    def.mass = 0.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({0, 0, -5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    def.shape.data.sphere.radius = 1.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({1, 1, 10});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::sphere;
    def.shape.data.sphere.radius = 2.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({2, 2, 10});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::box;
    def.shape.data.box.size = {4, 2, 1};
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({3, 3, 5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::capsule;
    def.shape.data.capsule.radius = 0.5f;
    def.shape.data.capsule.height = 2.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({4, 4, 5});
  }

  {

    gero::physics::PhysicBodyDef def;
    def.shape.type = gero::physics::PhysicShapeDef::Type::capsule;
    def.shape.data.capsule.radius = 1.0f;
    def.shape.data.capsule.height = 4.0f;
    def.mass = 1.0f;

    auto body = this->physic.world->getPhysicBodyManager().createAndAddBody(def);

    body->setPosition({5, 5, 5});
  }

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

  audio.soundManager = new gero::audio::OpenALSoundManager();
  audio.soundManager->setVolume(0.5f);
  audio.soundManager->loadOggFromFile(666, "./assets/audio/Cannon_3.ogg");

  // this->logic.controllers.freeFly.setPosition(glm::vec3(10, 10, 10));

  experimentalPhysicVehicle();
}

//

void Context::create(uint32_t width, uint32_t height) {
  if (_instance)
    D_THROW(std::runtime_error, "Context singleton already initialized");

  _instance = new Context();
  _instance->initialize(width, height);
}

void Context::destroy() {
  if (!_instance)
    D_THROW(std::runtime_error, "Context singleton already destroyed");

  delete _instance, _instance = nullptr;
}

Context& Context::get() {
  if (!_instance)
    D_THROW(std::runtime_error, "Context singleton not initialized");

  return *_instance;
}

// singleton
//
//
