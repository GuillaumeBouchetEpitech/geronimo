
#pragma once

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/system/containers/static_heap_grid_array.hpp"

#include <memory>

class HeightField {
private:
  uint32_t _fieldSize = 0;
  float _horizontalScale = 1.0f;
  float _verticalScale = 1.0f;
  gero::static_heap_grid_array<float> _heightFieldGridData;
  gero::static_heap_grid_array<glm::vec3> _normalsGridData;
  float _minHeight = +999999999.0f;
  float _maxHeight = -999999999.0f;
  float _arenaSize = 0.0f;

public:
  HeightField() = default;
  ~HeightField() = default;

public:
  void generate(uint32_t fieldSize, float horizontalScale, float verticalScale);

public:
  glm::vec3 getPosition(uint32_t x, uint32_t y) const;
  glm::vec3 getNormal(uint32_t x, uint32_t y) const;

  float getHeight(uint32_t x, uint32_t y) const;

  float getMinHeight() const;
  float getMaxHeight() const;

  uint32_t getFieldSize() const;
  float getHorizontalScale() const;
  float getVerticalScale() const;
  float getArenaSize() const;
  bool contains(const glm::vec2& position) const;

  void clamp(glm::vec2& position) const;
  void clamp(glm::vec3& position) const;
};
