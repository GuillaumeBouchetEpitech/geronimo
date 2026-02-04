
#include "FlockingManager.hpp"

#include "application/context/Context.hpp"

#include "geronimo/graphics/make-geometries/MakeGeometries.hpp"
#include "geronimo/system/asValue.hpp"
#include "geronimo/system/easing/easingFunctions.hpp"
#include "geronimo/system/math/angles.hpp"
#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/math/constants.hpp"
#include "geronimo/system/math/lerp.hpp"
#include "geronimo/system/math/safe-normalize.hpp"
// #include "geronimo/system/rng/RandomNumberGenerator.hpp"
#include "geronimo/system/rng/DeterministicRng.hpp"

FlockingManager::FlockingManager() {
  constexpr std::size_t totalEntities = 64;
  _allBoids.resize(totalEntities);

  gero::rng::DeterministicRng rng;
  rng.setSeed(0);

  for (std::size_t ii = 0; ii < _allBoids.size(); ++ii) {
    auto& currBoid = _allBoids.at(ii);
    currBoid.horizontalAngle = gero::math::hpi * (((ii % 2) == 0) ? 1.0f : -1.0f);
    currBoid.verticalAngle = gero::math::hpi + gero::math::qpi * (((ii % 3) == 0) ? 1.0f : -1.0f);

    currBoid.color.r = rng.getNormalisedValue();
    currBoid.color.g = rng.getNormalisedValue();
    currBoid.color.b = 1.0f - (currBoid.color.r + currBoid.color.g) * 0.5f;

    currBoid.color *= 0.5; // turn it darker (was too bright colored...)
  }
}

void FlockingManager::update(float elapsedTime) {

  glm::vec3 target = glm::vec3(80, -40, 30);

  constexpr float k_timePerTrailUpdate = 1.0f / 60.0f;

  bool needTrailUpdate = false;
  if (_timeUntilTrailUpdate > 0.0f)
    _timeUntilTrailUpdate -= elapsedTime;

  if (_timeUntilTrailUpdate <= 0.0f) {
    _timeUntilTrailUpdate = k_timePerTrailUpdate;
    needTrailUpdate = true;
  }

  constexpr float maxAcc = 0.05f * 1.0f;
  constexpr float maxVel = 2.0f * 1.0f;

  _spatialIndexer.clear();
  _spatialIndexer.pre_allocate(_allBoids.size());
  for (Boid& currBoid : _allBoids)
    _spatialIndexer.push(currBoid.position, &currBoid);
  _spatialIndexer.sync();

  for (Boid& currBoid : _allBoids)
    currBoid.computeAabb();
  _bvhTree.reset();
  _bvhTree.synchronize(_allBoids);

  std::vector<Boid*> searchResult;
  searchResult.reserve(32); // pre-allocate

  for (Boid& currBoid : _allBoids) {

    currBoid.acceleration = {0, 0, 0};

    const float distanceToTarget = glm::distance(currBoid.position, target);

    constexpr float k_strafeRadius = 40.0f;
    constexpr float k_range = 5.0f;

    if (distanceToTarget > k_strafeRadius + k_range) {
      currBoid.seek(target, 1.0f);
    } else if (distanceToTarget < k_strafeRadius - k_range) {
      currBoid.flee(target, 3.0f);
    } else {
      currBoid.strafe(target, currBoid.horizontalAngle, currBoid.verticalAngle, 1.5f);
    }

    constexpr float k_sepRadius = 4.0f;
    // const auto& searchResult = _spatialIndexer.search(currBoid.position, k_sepRadius);
    searchResult.clear();
    _bvhTree.searchByRadius(currBoid.position, k_sepRadius, _allBoids, searchResult);

    currBoid.separate(searchResult, 2.0f);
    currBoid.wander(0.5f);

    constexpr float k_maxDistance = 60.0f;
    const float coef = 1.0f + 1.0f * distanceToTarget / k_maxDistance;
    currBoid.applyAcceleration(maxAcc * coef, maxVel * coef, elapsedTime);

    if (needTrailUpdate) {
      currBoid.updateTrail();
    } else {
      currBoid.trail.at(0).position = currBoid.position;
    }
  }
}

void FlockingManager::render() {

  if (_allBoids.empty())
    return;

  auto& context = Context::get();
  // auto& stackRenderer = context.graphic.renderer.getSceneRenderer().getStackRenderers();
  auto& geometriesStackRenderer = context.graphic.renderer.getSceneRenderer().getGeometriesStackRenderer();

  const glm::vec4 k_color = glm::vec4(0.8f, 0.8f, 0.8f, 0.4f);

  GeometriesStackRenderer::GeometryInstance instance;
  instance.position = glm::vec3(0.0f);
  instance.orientation = glm::identity<glm::quat>();
  // instance.scale = glm::vec3(0.2f);
  // instance.scale = glm::vec3(0.1f);
  instance.scale = glm::vec3(0.8f);
  instance.color = k_color;
  // instance.outlineValue = 0.0f;

  for (const Boid& currBoid : _allBoids) {
    for (const auto& currData : currBoid.graphicData.trailData) {
      instance.scale.x = currData.length;
      instance.orientation = currData.orientation;
      instance.position = currData.center;
      instance.color = glm::vec4(currBoid.color, 1.0f);

      // // stackRenderer.pushBox(instance);
      // stackRenderer.getWireFramesStack().pushCross(currData.center, k_color, 0.25f);

      // GeometriesStackRenderer::GeometryInstance instance;
      // instance.position = inPosition;
      // instance.orientation = inOrientation;
      // instance.scale = size;
      // instance.color = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);
      // instance.light = 0.5f;

      geometriesStackRenderer.pushAlias(2222, instance);
    }
  }
}

const AbstractBoid& FlockingManager::getBoid(std::size_t index) const { return _allBoids.at(index); }

std::size_t FlockingManager::getTotalBoids() const { return _allBoids.size(); }
