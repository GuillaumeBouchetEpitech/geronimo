
#include "VoxelSimImpl.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/math/constants.hpp"
#include "geronimo/helpers/GLMath_sets_and_maps.hpp"

#include <array>
#include <algorithm>

VoxelSimImpl::VoxelSimImpl()
{

  {
    this->voxelManager.initialize();
  }

  {
    const std::string_view tmpFilename = "./assets/data/voxel-data.jsonc";
    this->voxelManager.loadJsonFile(tmpFilename);
  }

}

void VoxelSimImpl::update(float elapsedTime)
{
}

void VoxelSimImpl::render()
{
  this->voxelManager.render();
}
