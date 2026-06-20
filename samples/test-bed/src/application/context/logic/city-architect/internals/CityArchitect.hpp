
#include "../AbstractCityArchitect.hpp"

#include "logic/BrickModelsManager.hpp"

#include "graphics/instanced-brick-models/InstancedBrickModels.hpp"
#include "graphics/instanced-brick-models/WireFramesStackRenderer.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

class CityArchitect : public AbstractCityArchitect {
public:
  CityArchitect();
  ~CityArchitect() = default;

public:
  void update(float deltaTimeSec) override;
  void render() override;

private:

  BrickModelsManager _brickModelsManager;

  InstancedBrickModels _instancedBrickModels;
  WireFramesStackRenderer _wireFramesStackRenderer;

};
