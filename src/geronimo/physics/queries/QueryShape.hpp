
#pragma once

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/body/PhysicBodyManager.hpp"
#include "geronimo/physics/shape/PhysicShapeDef.hpp"

#include <array>

namespace gero {
namespace physics {

class PhysicWorld;

class QueryShape {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;

private:
  QueryShape(PhysicWorld& physicWorld);
  ~QueryShape() = default;

public:
  struct QueryShapeParams {
    glm::vec3 position;

    PhysicShapeDef shape;

    short collisionGroup;
    short collisionMask;

    void* toIgnore = nullptr;

    struct ResultRaw {
      bool hasHit = false;

      AbstractPhysicBody** allRawBodiesData; // abstract
      std::size_t allBodiesMaxSize;
      std::size_t allBodiesTotal;
    };

    template <std::size_t N> struct ResultArray {
      bool hasHit = false;

      std::array<AbstractPhysicBody*, N> allBodiesData;
      std::size_t allBodiesTotal;
    };

    QueryShapeParams(const glm::vec3& inPosition, PhysicShapeDef inShape,
                     short inGroup = -1, short inMask = -1,
                     void* inToIgnore = nullptr)
      : position(inPosition), shape(inShape), collisionGroup(inGroup),
        collisionMask(inMask), toIgnore(inToIgnore) {}
  };

private:
  bool _queryShape(QueryShapeParams& inParams,
                   QueryShapeParams::ResultRaw& outResultArray);

public:
  template <std::size_t N>
  bool queryShape(QueryShapeParams& inParams,
                  QueryShapeParams::ResultArray<N>& outResultArray) {
    QueryShapeParams::ResultRaw resultRaw;
    resultRaw.allRawBodiesData = outResultArray.allBodiesData.data();
    resultRaw.allBodiesMaxSize = N;
    resultRaw.allBodiesTotal = 0;
    const bool hasHit = _queryShape(inParams, resultRaw);

    outResultArray.hasHit = resultRaw.hasHit;
    outResultArray.allBodiesTotal = resultRaw.allBodiesTotal;

    return hasHit;
  }

  bool queryShape(QueryShapeParams& inParams,
                  std::vector<AbstractPhysicBody*>& outResultVector);

};

} // namespace physics
} // namespace gero
