
#include "../AbstractArtificialStupiditySim.hpp"

#include "game-logic/KeyframesManager.hpp"
#include "game-logic/PlatformerDuel.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include <unordered_map>

// forward declaration
class ArtificialStupiditySimImpl;

class ArtificialStupiditySimImpl : public AbstractArtificialStupiditySim {
public:
  ArtificialStupiditySimImpl();
  ~ArtificialStupiditySimImpl() = default;

private:
  void _loadJsonFile(const std::string_view inFilepath);

public:
  void update(float deltaTimeSec) override;
  void renderScene() override;
  void renderHUD() override;

private:
  KeyframesManager _keyframesManager;
  PlatformerDuel _platformerDuel;

};
