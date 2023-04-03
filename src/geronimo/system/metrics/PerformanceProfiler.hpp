
#pragma once

#include "geronimo/system/containers/dynamic_heap_array.hpp"

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace gero {
namespace metrics {

class PerformanceProfiler {

public:
  class TimeData {

    friend PerformanceProfiler;

  private:
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _stop;
    int32_t _latestDuration = 0;
    gero::dynamic_heap_array<int32_t, int32_t> _historicData;
    int32_t _historicIndex = 0;
    int32_t _averageDuration = 0;
    int32_t _maxDuration = 0;
    int32_t _minDuration = 0;

  private:
    void _onStart();
    void _onStop();
    void _pushNewValue(int32_t inDuration);

  public:
    TimeData(std::size_t inPreAllocatedHistoricSize);
    TimeData(const TimeData& other) = delete;
    TimeData(TimeData&& other) = delete;
    TimeData& operator=(const TimeData& other) = delete;
    TimeData& operator=(TimeData&& other) = delete;
    ~TimeData() = default;

  public:
    int32_t getLatestDuration() const;
    int32_t getAverageDuration() const;
    int32_t getMaxDuration() const;
    int32_t getMinDuration() const;
    int32_t getDurationByIndex(std::size_t index) const;
    std::size_t getTotalDurations() const;

  };

  using TimeDataMap = std::unordered_map<std::string, TimeData>;

private:
  std::size_t _preAllocatedHistoricSize;
  TimeDataMap _allTimes;
  std::vector<std::string> _allDataKeys;

public:
  PerformanceProfiler(std::size_t inPreAllocatedHistoricSize = 60);
  ~PerformanceProfiler() = default;

public:
  void start(const std::string& name);
  void stop(const std::string& name);
  void set(const std::string& name, int32_t inDuration);

public:
  const TimeDataMap& getTimeDataMap() const;

public:
  using MaybeTimeDataRef = std::optional<std::reference_wrapper<const TimeData>>;

  const MaybeTimeDataRef tryGetTimeData(const std::string& name) const;

public:
  const std::vector<std::string>& getAllDataKeys() const;

  std::size_t getHistoricSize() const;

private:
  TimeData& _getOrCreate(const std::string& name);
};

} // namespace metrics
} // namespace gero
