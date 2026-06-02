
#pragma once

#include "geronimo/helpers/GLMath.hpp"

class GenericAABB
{
public:
  GenericAABB();
  GenericAABB(const glm::vec3& inMin, const glm::vec3& inMax);

public:
  void reset();
  void makeInvalid();

public:
  void set(const glm::vec3& inMin, const glm::vec3& inMax);
  void expand(const glm::vec3& inValue);
  void grow(float inValue);

public:
  bool contains(const glm::vec3& inValue) const;
  bool overlap(const GenericAABB& inAABB) const;

public:
  bool isValid() const {
    return (
      this->_min.x <= this->_max.x &&
      this->_min.y <= this->_max.y &&
      this->_min.z <= this->_max.z
    );
  }
  const glm::vec3& getMin() const { return this->_min; }
  const glm::vec3& getMax() const { return this->_max; }

private:
  glm::vec3 _min;
  glm::vec3 _max;

};
