
#include "../AbstractCityArchitect.hpp"

#include "logic/FloorManager.hpp"

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

  FloorManager _floorManager;

};
