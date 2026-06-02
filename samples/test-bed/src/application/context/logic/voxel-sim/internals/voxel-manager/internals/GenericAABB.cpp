
#include "GenericAABB.hpp"

#include <numeric>

namespace /*anonymous*/ {

  constexpr float k_minVal = std::numeric_limits<float>::lowest();
  constexpr float k_maxVal = std::numeric_limits<float>::max();
};

GenericAABB::GenericAABB() {
  this->reset();
}

GenericAABB::GenericAABB(const glm::vec3& inMin, const glm::vec3& inMax) {
  this->set(inMin, inMax);
}

void GenericAABB::reset() {
  this->_min = glm::vec3(k_maxVal,k_maxVal,k_maxVal);
  this->_max = glm::vec3(k_minVal,k_minVal,k_minVal);
}

void GenericAABB::makeInvalid() {
  this->_min = glm::vec3(k_minVal,k_minVal,k_minVal);
  this->_max = glm::vec3(k_maxVal,k_maxVal,k_maxVal);
}

void GenericAABB::set(const glm::vec3& inMin, const glm::vec3& inMax)
{
  this->_min = inMin;
  this->_max = inMax;
}

void GenericAABB::expand(const glm::vec3& inValue) {
  this->_min = glm::min(this->_min, inValue);
  this->_max = glm::max(this->_max, inValue);
}

void GenericAABB::grow(float inValue) {
  // if (!this->isValid()) {
  //   return;
  // }
  this->_min -= inValue;
  this->_max += inValue;
}

bool GenericAABB::contains(const glm::vec3& inValue) const
{
  // if (!this->isValid()) {
  //   return false;
  // }
  return (!(
    inValue.x < this->_min.x ||
    inValue.y < this->_min.y ||
    inValue.z < this->_min.z ||
    inValue.x > this->_max.x ||
    inValue.y > this->_max.y ||
    inValue.z > this->_max.z
  ));
}

bool GenericAABB::overlap(const GenericAABB& inAABB) const
{
  // if (!this->isValid()) {
  //   return false;
  // }
  return (!(
    this->_max.x < inAABB._min.x ||
    this->_max.y < inAABB._min.y ||
    this->_max.z < inAABB._min.z ||
    this->_min.x > inAABB._max.x ||
    this->_min.y > inAABB._max.y ||
    this->_min.z > inAABB._max.z
  ));
}
