
#pragma once

#include "geronimo/helpers/GLMath.hpp"
#include "geronimo/physics/body/PhysicBodyManager.hpp"

#include <array>

namespace gero {
namespace physics {

class PhysicWorld;

class Raycaster {
  friend PhysicWorld;

private:
  PhysicWorld& _physicWorld;

private:
  Raycaster(PhysicWorld& physicWorld);
  ~Raycaster() = default;

public:
  struct RaycastParams {
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
      AbstractPhysicBody* body = nullptr;
      // PhysicBodyManager::BodyWeakRef bodyRef;
    };

    template <std::size_t N> struct ResultArray {
      bool hasHit = false;

      std::array<ResultImpact, N> allImpactsData;
      std::size_t allImpactsTotal;
    };

    RaycastParams(const glm::vec3& rayFrom, const glm::vec3& rayTo,
                  float sweepRadius = 0.0f, short group = -1, short mask = -1,
                  Type inType = Type::closest, void* inToIgnore = nullptr)
      : from(rayFrom), to(rayTo), radius(sweepRadius), collisionGroup(group),
        collisionMask(mask), type(inType), toIgnore(inToIgnore) {}
  };

public:
  using OnNewPhysicBodyCallback = std::function<bool(const RaycastParams::ResultImpact&)>;

private:
  void _normalRaycast(RaycastParams& params, const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);
  void _convexSweep(RaycastParams& params, const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);
  void _raycast(RaycastParams& params, const Raycaster::OnNewPhysicBodyCallback& onNewPhysicBodyCallback);

public:
  template <std::size_t N>
  bool raycast(RaycastParams& inParams,
               RaycastParams::ResultArray<N>& outResultArray) {

    outResultArray.hasHit = false;
    outResultArray.allImpactsTotal = 0;

    const OnNewPhysicBodyCallback callback = [&inParams, &outResultArray](const RaycastParams::ResultImpact& inResult) -> bool
    {
      if (outResultArray.allImpactsTotal >= N)
        return false;

      if (inParams.type == RaycastParams::Type::closest)
      {
        outResultArray.allImpactsData[0] = inResult;
        outResultArray.allImpactsTotal = 1;
      }
      else
      {
        // check duplicates
        for (std::size_t ii = 0; ii < outResultArray.allImpactsTotal; ++ii)
          if (outResultArray.allImpactsData[ii].body == inResult.body)
            return true;

        outResultArray.allImpactsData[outResultArray.allImpactsTotal] = inResult;
        outResultArray.allImpactsTotal += 1;
      }

      return true;
    };

    _raycast(inParams, callback);

    outResultArray.hasHit = outResultArray.allImpactsTotal > 0;

    return outResultArray.hasHit;
  }

  bool raycast(RaycastParams& params, std::vector<RaycastParams::ResultImpact>& outResultVector);
};

} // namespace physics
} // namespace gero
