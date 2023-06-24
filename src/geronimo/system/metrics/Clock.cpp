
#include "Clock.hpp"

#include "geronimo/system/aliasing.hpp"

#include <chrono>

namespace {

D_ALIAS_FUNCTION(_getTime, std::chrono::high_resolution_clock::now);
D_ALIAS_FUNCTION(_asMilliSeconds, std::chrono::duration_cast<std::chrono::milliseconds>);

} // namespace

//
//
//
//
//

namespace gero {
namespace metrics {

void Clock::start() { _start = _getTime(); }

void Clock::stop() {
  _stop = _getTime();
  if (_start > _stop) {
    _duration = -1;
  } else {
    _duration = int32_t(_asMilliSeconds(_stop - _start).count());
  }
}

int32_t Clock::getDuration() const { return _duration; }

} // namespace metrics
} // namespace gero
