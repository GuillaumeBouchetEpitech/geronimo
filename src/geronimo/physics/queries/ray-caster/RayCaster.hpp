
#pragma once

// #include "geronimo/physics/body/PhysicBodyManager.hpp"
#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/body/AbstractPhysicBody.hpp"

#include <array>

namespace gero {
namespace physics {

class PhysicWorld;

class RayCaster {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;

private:
  RayCaster(PhysicWorld& physicWorld);
  ~RayCaster() = default;

public:
  struct RayCastParams {
    glm::vec3 from;
    glm::vec3 to;
    float radius;

    short collisionGroup;
    short collisionMask;

    enum Type {
      closest = 0,
      everything,
    };
    Type type = Type::closest;

    void* toIgnore = nullptr;

    struct ResultImpact {
      glm::vec3 impactPoint;
      glm::vec3 impactNormal;
      float hitFraction;
      AbstractPhysicBody* body = nullptr;
    };

    template <std::size_t N> struct ResultArray {
      bool hasHit = false;

      std::array<ResultImpact, N> allImpactsData;
      std::size_t allImpactsTotal;
    };

    RayCastParams(const glm::vec3& rayFrom,
                  const glm::vec3& rayTo,
                  float sweepRadius = 0.0f,
                  short group = -1,
                  short mask = -1,
                  Type inType = Type::closest,
                  void* inToIgnore = nullptr)
      : from(rayFrom), to(rayTo), radius(sweepRadius), collisionGroup(group), collisionMask(mask), type(inType),
        toIgnore(inToIgnore) {}
  };

public:
  using OnNewPhysicBodyCallback = std::function<bool(const RayCastParams::ResultImpact&)>;

private:
  void _normalRayCast(RayCastParams& params, const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);
  void _convexSweep(RayCastParams& params, const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);
  void _rayCast(RayCastParams& params, const RayCaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);

public:
  template <std::size_t N> bool rayCast(RayCastParams& inParams, RayCastParams::ResultArray<N>& outResultArray) {

    outResultArray.hasHit = false;
    outResultArray.allImpactsTotal = 0;

    const OnNewPhysicBodyCallback callback = [&inParams,
                                              &outResultArray](const RayCastParams::ResultImpact& inResult) -> bool {
      if (inParams.type == RayCastParams::Type::closest) {
        outResultArray.allImpactsData[0] = inResult;
        outResultArray.allImpactsTotal = 1;
        return true;
      } else {
        if (outResultArray.allImpactsTotal >= outResultArray.allImpactsData.size())
          return false;

        // check duplicates
        for (std::size_t ii = 0; ii < outResultArray.allImpactsTotal; ++ii)
          if (outResultArray.allImpactsData[ii].body == inResult.body) {
            if (outResultArray.allImpactsData[ii].hitFraction > inResult.hitFraction) {
              // replace if new one is closer
              outResultArray.allImpactsData[ii] = inResult;
            }
            return true;
          }

        outResultArray.allImpactsData[outResultArray.allImpactsTotal] = inResult;
        outResultArray.allImpactsTotal += 1;
      }

      return true;
    };

    _rayCast(inParams, callback);

    // if (
    //   inParams.type == RayCastParams::Type::closest && inParams.radius > 0.0f
    // ) {
    //   D_MYLOG("inParams.from " << inParams.from);
    //   D_MYLOG("inParams.to   " << inParams.to);
    //   if (outResultArray.allImpactsTotal > 0) {
    //     D_MYLOG(" => allImpactsData[0].impactPoint " << outResultArray.allImpactsData[0].impactPoint);
    //     D_MYLOG(" => allImpactsData[0].impactNormal " << outResultArray.allImpactsData[0].impactNormal);
    //     D_MYLOG(" => allImpactsData[0].body " << outResultArray.allImpactsData[0].body);
    //   }
    // }

    outResultArray.hasHit = outResultArray.allImpactsTotal > 0;

    return outResultArray.hasHit;
  }

  bool rayCast(RayCastParams& params, std::vector<RayCastParams::ResultImpact>& outResultVector);
};

} // namespace physics
} // namespace gero
