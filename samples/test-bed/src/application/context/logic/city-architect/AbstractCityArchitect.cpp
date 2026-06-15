
#include "AbstractCityArchitect.hpp"

#include "internals/CityArchitect.hpp"

std::unique_ptr<AbstractCityArchitect> AbstractCityArchitect::create() {
  return std::make_unique<CityArchitect>();
}
