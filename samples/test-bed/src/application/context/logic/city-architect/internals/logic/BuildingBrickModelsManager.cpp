
#include "BuildingBrickModelsManager.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

BuildingBrickModelsManager::BuildingBrickModelsManager()
{
  this->_buildingBrickModels.reserve(16);
}

