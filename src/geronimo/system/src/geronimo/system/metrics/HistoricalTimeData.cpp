
#include "HistoricalTimeData.hpp"

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

HistoricalTimeData::HistoricalTimeData(std::size_t inSize) { setSize(inSize); }

void HistoricalTimeData::start() { _start = _getTime(); }

void HistoricalTimeData::stop() {
  _stop = _getTime();

  if (_start > _stop)
    return;

  pushNewValue(int32_t(_asMilliSeconds(_stop - _start).count()));
}

void HistoricalTimeData::reset() {
  _historicData.clear();
  _latestDuration = 0;
  _historicIndex = 0;
  _averageDuration = 0;
  _maxDuration = 0;
  _minDuration = 0;
}

void HistoricalTimeData::pushNewValue(int32_t inDuration) {
  _latestDuration = inDuration;

  if (_historicData.size() == _size) {
    _historicData.at(std::size_t(_historicIndex)) = int32_t(_latestDuration);
    _historicIndex = (_historicIndex + 1) % int32_t(_historicData.size());

    for (int32_t value : _historicData)
      _averageDuration += value;
    _averageDuration /= int32_t(_historicData.size());
  } else {
    _historicData.push_back(_latestDuration);
  }

  _maxDuration = 0;
  for (int32_t value : _historicData)
    _maxDuration = std::max(_maxDuration, value);

  _minDuration = _maxDuration;
  for (int32_t value : _historicData)
    _minDuration = std::min(_minDuration, value);
}

void HistoricalTimeData::setSize(std::size_t inSize) {
  _size = inSize;
  _historicData.clear();
  _historicData.pre_allocate(_size);
}

int32_t HistoricalTimeData::getLatestDuration() const { return _latestDuration; }

int32_t HistoricalTimeData::getAverageDuration() const { return _averageDuration; }

int32_t HistoricalTimeData::getMaxDuration() const { return _maxDuration; }

int32_t HistoricalTimeData::getMinDuration() const { return _minDuration; }

int32_t HistoricalTimeData::getDurationByIndex(std::size_t index) const {
  return _historicData.at(std::size_t((_historicIndex + int32_t(index)) % int32_t(_historicData.size())));
}

std::size_t HistoricalTimeData::getTotalDurations() const { return _historicData.size(); }

std::size_t HistoricalTimeData::getSize() const { return _size; }

//
//
//
//
//

} // namespace metrics
} // namespace gero
