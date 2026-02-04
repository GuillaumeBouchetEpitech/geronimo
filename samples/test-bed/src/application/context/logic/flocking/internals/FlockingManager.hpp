
#include "../AbstractFlockingManager.hpp"

#include "./Boid.hpp"

#include "geronimo/system/trees/BvhTree.hpp"
#include "geronimo/system/trees/SpatialIndexer.hpp"

class FlockingManager : public AbstractFlockingManager {
public:
  FlockingManager();
  ~FlockingManager() = default;

public:
  void update(float elapsedTime) override;
  void render() override;

public:
  const AbstractBoid& getBoid(std::size_t index) const override;
  std::size_t getTotalBoids() const override;

private:
  std::vector<Boid> _allBoids;

  float _timeUntilTrailUpdate = 0.0f;

  gero::trees::Spatial3dIndexer<Boid*> _spatialIndexer;
  gero::trees::BvhTree<Boid> _bvhTree;
};
