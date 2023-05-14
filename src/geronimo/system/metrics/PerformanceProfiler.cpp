
#include "PerformanceProfiler.hpp"

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

PerformanceProfiler::TimeData::TimeData(std::size_t inPreAllocatedHistoricSize) {
  _historicData.pre_allocate(inPreAllocatedHistoricSize);
}

void PerformanceProfiler::TimeData::_onStart() { _start = _getTime(); }

void PerformanceProfiler::TimeData::_onStop() {
  _stop = _getTime();

  if (_start > _stop)
    return;

  _pushNewValue(int32_t(_asMilliSeconds(_stop - _start).count()));
}

void PerformanceProfiler::TimeData::_pushNewValue(int32_t inDuration) {
  _latestDuration = inDuration;

  if (_historicData.size() == _historicData.capacity()) {
    _historicData.at(std::size_t(_historicIndex)) = int32_t(_latestDuration);
    _historicIndex = (_historicIndex + 1) % int32_t(_historicData.size());

    for (int32_t value : _historicData)
      _averageDuration += value;
    _averageDuration /= _historicData.size();
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

int32_t PerformanceProfiler::TimeData::getLatestDuration() const { return _latestDuration; }

int32_t PerformanceProfiler::TimeData::getAverageDuration() const { return _averageDuration; }

int32_t PerformanceProfiler::TimeData::getMaxDuration() const { return _maxDuration; }

int32_t PerformanceProfiler::TimeData::getMinDuration() const { return _minDuration; }

int32_t PerformanceProfiler::TimeData::getDurationByIndex(std::size_t index) const {
  return _historicData.at(std::size_t((_historicIndex + int32_t(index)) % int32_t(_historicData.size())));
}

std::size_t PerformanceProfiler::TimeData::getTotalDurations() const { return _historicData.size(); }

//
//
//
//
//

PerformanceProfiler::PerformanceProfiler(std::size_t inPreAllocatedHistoricSize /*= 60*/)
  : _preAllocatedHistoricSize(inPreAllocatedHistoricSize) {
  _allDataKeys.reserve(32);
}

void PerformanceProfiler::start(const std::string& name) { _getOrCreate(name)._onStart(); }

void PerformanceProfiler::stop(const std::string& name) { _getOrCreate(name)._onStop(); }

void PerformanceProfiler::set(const std::string& name, int32_t inDuration) {
  _getOrCreate(name)._pushNewValue(inDuration);
}

const PerformanceProfiler::TimeDataMap& PerformanceProfiler::getTimeDataMap() const { return _allTimes; }

const PerformanceProfiler::MaybeTimeDataRef PerformanceProfiler::tryGetTimeData(const std::string& name) const {
  auto it = _allTimes.find(name);
  if (it == _allTimes.end())
    return {};

  return it->second;
}

const std::vector<std::string>& PerformanceProfiler::getAllDataKeys() const { return _allDataKeys; }

std::size_t PerformanceProfiler::getHistoricSize() const { return _preAllocatedHistoricSize; }

PerformanceProfiler::TimeData& PerformanceProfiler::_getOrCreate(const std::string& name) {
  auto it = _allTimes.find(name);
  if (it == _allTimes.end()) {
    it = _allTimes.emplace(name, _preAllocatedHistoricSize).first;
    _allDataKeys.push_back(name);
  }

  return it->second;
}

} // namespace metrics
} // namespace gero
