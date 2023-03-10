
#include "PerformanceProfiler.hpp"

#include "geronimo/system/aliasing.hpp"

#include <chrono>

namespace {

D_ALIAS_FUNCTION(_getTime, std::chrono::high_resolution_clock::now);
D_ALIAS_FUNCTION(_asMilliSeconds, std::chrono::duration_cast<std::chrono::milliseconds>);

}

//
//
//
//
//

namespace gero {
namespace metrics {

PerformanceProfiler::TimeData::TimeData(std::size_t inPreAllocatedHistoricSize)
{
  _historicData.pre_allocate(inPreAllocatedHistoricSize);
}

void
PerformanceProfiler::TimeData::_onStart()
{
  _start = _getTime();
}

void
PerformanceProfiler::TimeData::_onStop() {
  _stop = _getTime();

  if (_start > _stop)
    return;

  _pushNewValue(_asMilliSeconds(_stop - _start).count());
}

void PerformanceProfiler::TimeData::_pushNewValue(int64_t inDuration)
{
  _latestDuration = inDuration;

  if (_historicData.size() == _historicData.capacity())
  {
    _historicIndex = (_historicIndex + 1) % int32_t(_historicData.size());
    _historicData.at(std::size_t(_historicIndex)) = int64_t(_latestDuration);

    for (int64_t value : _historicData)
      _averageDuration += value;
    _averageDuration /= _historicData.size();
  }
  else
  {
    _historicData.push_back(_latestDuration);
  }
}

int64_t
PerformanceProfiler::TimeData::getLatestDuration() const {
  return _latestDuration;
}

int64_t
PerformanceProfiler::TimeData::getAverageDuration() const {
  return _averageDuration;
}

//
//
//
//
//

PerformanceProfiler::PerformanceProfiler(std::size_t inPreAllocatedHistoricSize /*= 60*/)
  : _preAllocatedHistoricSize(inPreAllocatedHistoricSize)
{}

void
PerformanceProfiler::start(const std::string& name) {
  _getOrCreate(name)._onStart();
}

void
PerformanceProfiler::stop(const std::string& name) {
  _getOrCreate(name)._onStop();
}

void
PerformanceProfiler::set(const std::string& name, int64_t inDuration) {
  _getOrCreate(name)._pushNewValue(inDuration);
}

const PerformanceProfiler::TimeDataMap&
PerformanceProfiler::getTimeDataMap() const {
  return _allTimes;
}

PerformanceProfiler::TimeData& PerformanceProfiler::_getOrCreate(const std::string& name)
{
  auto it = _allTimes.find(name);
  if (it == _allTimes.end())
  {
    it = _allTimes.emplace(name, _preAllocatedHistoricSize).first;
  }

  return it->second;
}



}
}
