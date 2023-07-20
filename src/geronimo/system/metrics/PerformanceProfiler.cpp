
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

PerformanceProfiler::PerformanceProfiler(std::size_t inPreAllocatedHistoricSize /*= 60*/)
  : _preAllocatedHistoricSize(inPreAllocatedHistoricSize) {
  _allDataKeys.reserve(32);
}

void PerformanceProfiler::start(const std::string& name) { _getOrCreate(name).start(); }

void PerformanceProfiler::stop(const std::string& name) { _getOrCreate(name).stop(); }

void PerformanceProfiler::set(const std::string& name, int32_t inDuration) {
  _getOrCreate(name).pushNewValue(inDuration);
}

const PerformanceProfiler::HistoricalTimeDataMap& PerformanceProfiler::getHistoricalTimeDataMap() const { return _allTimes; }

const PerformanceProfiler::MaybeTimeDataRef PerformanceProfiler::tryGetTimeData(const std::string& name) const {
  auto it = _allTimes.find(name);
  if (it == _allTimes.end())
    return {};

  return it->second;
}

const std::vector<std::string>& PerformanceProfiler::getAllDataKeys() const { return _allDataKeys; }

std::size_t PerformanceProfiler::getHistoricSize() const { return _preAllocatedHistoricSize; }

HistoricalTimeData& PerformanceProfiler::_getOrCreate(const std::string& name) {
  auto it = _allTimes.find(name);
  if (it == _allTimes.end()) {
    it = _allTimes.emplace(name, 60U).first;
    it->second.setSize(_preAllocatedHistoricSize);
    _allDataKeys.push_back(name);
  }

  return it->second;
}

} // namespace metrics
} // namespace gero
