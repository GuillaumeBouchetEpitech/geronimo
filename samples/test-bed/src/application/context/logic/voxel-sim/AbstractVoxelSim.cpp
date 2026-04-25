
#include "AbstractVoxelSim.hpp"

#include "internals/VoxelSimImpl.hpp"

std::unique_ptr<AbstractVoxelSim> AbstractVoxelSim::create() {
  return std::make_unique<VoxelSimImpl>();
}
