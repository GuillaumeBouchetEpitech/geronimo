
#include "HeightField.hpp"

#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/rng/proceduralGeneration/SimplexNoise.hpp"

void HeightField::generate(uint32_t fieldSize, float horizontalScale, float verticalScale) {
  _fieldSize = fieldSize;
  _horizontalScale = horizontalScale;
  _verticalScale = verticalScale;
  _arenaSize = float(_fieldSize) * _horizontalScale - _horizontalScale;

  _heightFieldGridData.allocate(_fieldSize, _fieldSize);
  _normalsGridData.allocate(_fieldSize, _fieldSize);

  constexpr float frequency = 1.0f;
  constexpr float amplitude = 1.0f;
  constexpr float lacunarity = 2.0f;
  constexpr float persistence = 0.5f;
  constexpr int seed = 0;
  gero::procGen::SimplexNoise terrainNoise(frequency, amplitude, lacunarity, persistence, seed);

  for (int yy = 0; yy < int(_fieldSize); ++yy)
    for (int xx = 0; xx < int(_fieldSize); ++xx) {
      constexpr int noiseOctaves = 3;
      constexpr float noiseCoef = 0.05f;

      const glm::vec2 position = glm::vec2(xx, yy);
      const glm::vec2 noisePos = position * noiseCoef;

      const float terrainNoiseVal = terrainNoise.fractal(noiseOctaves, noisePos.x, noisePos.y);

      const float heightValue = gero::math::clamp(terrainNoiseVal * 0.2f, -1.0f, 1.0f) * 1.0f;

      _heightFieldGridData(yy, xx) = heightValue;

      _minHeight = std::min(_minHeight, heightValue);
      _maxHeight = std::max(_maxHeight, heightValue);
    }

  {
    int32_t minX = 0;
    int32_t maxX = int32_t(_fieldSize) - 1;
    int32_t minY = 0;
    int32_t maxY = int32_t(_fieldSize) - 1;

    for (int32_t yy = 0; yy < int32_t(_fieldSize); ++yy)
      for (int32_t xx = 0; xx < int32_t(_fieldSize); ++xx) {

        glm::vec3 accNormal = {0, 0, 0};
        int32_t totalSamples = 0;

        glm::vec3 pos0 = getPosition(uint32_t(xx), uint32_t(yy));

        struct TmpStep {
          glm::ivec2 step1;
          glm::ivec2 step2;
        };
        std::array<TmpStep, 4> allSteps = {{
          {{0, -1}, {-1, 0}},
          {{+1, 0}, {0, -1}},
          {{0, +1}, {+1, 0}},
          {{+1, 0}, {0, -1}},
        }};

        for (const TmpStep& currStep : allSteps) {
          int32_t step1X = xx + currStep.step1.x;
          int32_t step1Y = yy + currStep.step1.y;
          int32_t step2X = xx + currStep.step2.x;
          int32_t step2Y = yy + currStep.step2.y;

          if (step1X <= minX || step1X >= maxX || step1Y <= minY || step1Y >= maxY || step2X <= minX ||
              step2X >= maxX || step2Y <= minY || step2Y >= maxY) {
            continue;
          }

          const glm::vec3 pos1 = getPosition(uint32_t(step1X), uint32_t(step1Y));
          const glm::vec3 pos2 = getPosition(uint32_t(step2X), uint32_t(step2Y));

          accNormal += glm::normalize(glm::cross(pos1 - pos0, pos0 - pos2));
          totalSamples += 1;
        }

        if (totalSamples == 0)
          _normalsGridData(yy, xx) = glm::vec3(0, 0, 1);
        else
          _normalsGridData(yy, xx) = glm::normalize(accNormal / float(totalSamples));
      }
  }
}

float HeightField::getHeight(uint32_t x, uint32_t y) const { return _heightFieldGridData(int(y), int(x)); }

glm::vec3 HeightField::getPosition(uint32_t x, uint32_t y) const {
  return glm::vec3(float(x) * _horizontalScale, float(y) * _horizontalScale, getHeight(x, y) * _verticalScale);
}

glm::vec3 HeightField::getNormal(uint32_t x, uint32_t y) const { return _normalsGridData(int(y), int(x)); }

float HeightField::getMinHeight() const { return _minHeight; }

float HeightField::getMaxHeight() const { return _maxHeight; }

uint32_t HeightField::getFieldSize() const { return _fieldSize; }

float HeightField::getHorizontalScale() const { return _horizontalScale; }

float HeightField::getVerticalScale() const { return _verticalScale; }

float HeightField::getArenaSize() const { return _arenaSize; }

bool HeightField::contains(const glm::vec2& position) const {
  return (position.x >= 0.0f && position.x <= _arenaSize && position.y >= 0.0f && position.y <= _arenaSize);
}

void HeightField::clamp(glm::vec2& position) const {
  position.x = gero::math::clamp(position.x, 0.0f, _arenaSize);
  position.y = gero::math::clamp(position.y, 0.0f, _arenaSize);
}

void HeightField::clamp(glm::vec3& position) const {
  position.x = gero::math::clamp(position.x, 0.0f, _arenaSize);
  position.y = gero::math::clamp(position.y, 0.0f, _arenaSize);
}
