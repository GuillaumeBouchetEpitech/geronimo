
#include "AbstractArtificialStupiditySim.hpp"

#include "internals/ArtificialStupiditySimImpl.hpp"

std::unique_ptr<AbstractArtificialStupiditySim> AbstractArtificialStupiditySim::create() {
  return std::make_unique<ArtificialStupiditySimImpl>();
}
