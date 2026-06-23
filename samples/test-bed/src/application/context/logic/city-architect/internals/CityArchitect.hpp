
#include "../AbstractCityArchitect.hpp"

#include "logic/BrickModelsManager.hpp"

#include "graphics/instanced-brick-models/InstancedBrickModels.hpp"
#include "graphics/instanced-brick-models/WireFramesStackRenderer.hpp"
#include "graphics/instanced-brick-models/TrianglesAccumulator.hpp"

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/NonCopyable.hpp"

#include <string_view>

class CityArchitect : public AbstractCityArchitect {
public:
  CityArchitect();
  ~CityArchitect() = default;

public:
  void loadJson(std::string_view inFilepath);
  void update(float deltaTimeSec) override;
  void render() override;

private:

  BrickModelsManager _brickModelsManager;

  InstancedBrickModels _instancedBrickModels;
  WireFramesStackRenderer _wireFramesStackRenderer;
  TrianglesAccumulator _trianglesAccumulator;

};
