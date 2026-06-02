
#pragma once

// #include "application/context/logic/simulation/voxels/VoxelEntitiesManager.hpp"
// #include "application/context/logic/simulation/graphic-trails/GraphicTrails.hpp"

// #include "geronimo/system/math/constants.hpp"
#include "geronimo/helpers/GLMath.hpp"

#include <cstdint>

namespace cmp {

struct transform {
  glm::vec3 pos; // offset position
  glm::quat quat; // offset orientation

  glm::vec3 dstPos; // scene position
  glm::quat dstQuat; // scene orientation
};

// struct childEntity {
//   uint32_t parentId;
// };

struct boid {
  // uint32_t shipId;
  glm::vec3 forwardAxis;
  glm::vec3 acceleration;
  glm::vec3 velocity;
  // float timeAliveLeft{-1.0f};
};

// struct engine {
//   uint32_t currentCapacity;
//   uint32_t maxCapacity;
// };

struct faction {
  uint32_t factionId;
  glm::vec3 color;
};

// struct shield {
//   glm::vec3 scale;
//   uint32_t health;
// };

// struct graphicModel {
//   uint32_t modelId;
// };

// struct voxelEntityWeakRef {
//   // uint32_t modelId;
//   VoxelEntityRef hullEntity;
//   VoxelEntityRef shieldEntity;
// };

// struct turret {
//   uint32_t turretId;
//   uint32_t weaponId;

//   glm::vec3 forward{1,0,0};
//   // float timeLeftToNewTarget; // cmp::targeting?
//   // int32_t targetId; // cmp::targeting?
//   uint32_t ammoClip;
//   float reloadCooldown;

//   float upAxisMinDotVal = -1; // opposite of up axis alignment (down axis?)
//   float upAxisMaxDotVal = +1; // up axis aligned
//   float forwardAxisMinDotVal = -1.0f; // range of left axis alignment

//   VoxelEntityRef basketEntity;
//   VoxelEntityRef mantletEntity;
//   VoxelEntityRef barrelEntity;
// };

// struct targeting {
//   float timeLeftToNewTarget;
//   int32_t targetId;
// };

// struct projectile {
//   glm::vec3 dir;
//   float speed;
//   float distanceLeft;
//   float damage;
// };

// struct trailWeakRef {
//   VoxelEntityRef parentEntity;
//   glm::ivec3 voxelPos;
//   TrailWeakRef trailRef;
// };

struct debugging {
  int32_t debugMode{0};
};

}; // namespace cmp
