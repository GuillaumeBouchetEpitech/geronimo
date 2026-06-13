
#include "../AbstractBasicScene.hpp"

#include "HeightField.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include "geronimo/physics/AbstractPhysicWorld.hpp"

#include "geronimo/system/NonCopyable.hpp"
#include "geronimo/system/metrics/PerformanceProfiler.hpp"
#include "geronimo/system/asValue.hpp"

enum class Groups : short {
  all = -1,
  player = (1 << 0),
  ground = (1 << 1),
  dynamic = (1 << 2),
  projectile = (1 << 3),
  sensor = (1 << 4),
};

enum class Masks : short {
  all = -1,
  player = gero::asValue(Groups::ground) | gero::asValue(Groups::projectile) | gero::asValue(Groups::sensor),
  ground = gero::asValue(Groups::player) | gero::asValue(Groups::dynamic) | gero::asValue(Groups::projectile),
  dynamic = (1 << 2),
  projectile = gero::asValue(Groups::ground) | gero::asValue(Groups::player),
  sensor = (1 << 4),
};

class BasicScene : public AbstractBasicScene {
public:
  BasicScene();
  ~BasicScene() = default;

public:
  void update(float deltaTimeSec) override;
  void render() override;

private:
  void initializePhysicResources();
  void experimentalPhysicVehicle();

private:

  struct t_physic {
    std::unique_ptr<gero::physics::AbstractPhysicWorld> world;
  } physic;

  struct Logic {
    HeightField heightField;
  } logic;

};
