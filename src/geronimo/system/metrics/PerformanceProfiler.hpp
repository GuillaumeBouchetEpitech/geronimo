
#pragma once

#include "geronimo/system/containers/dynamic_heap_array.hpp"

#include <chrono>
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
    int64_t _latestDuration = 0;
    gero::dynamic_heap_array<int64_t, int64_t> _historicData;
    int32_t _historicIndex = 0;
    int64_t _averageDuration = 0;

  private:
    void _onStart();
    void _onStop();
    void _pushNewValue(int64_t inDuration);

  public:
    TimeData(std::size_t inPreAllocatedHistoricSize);
    TimeData(const TimeData& other) = delete;
    TimeData(TimeData&& other) = delete;
    TimeData& operator=(const TimeData& other) = delete;
    TimeData& operator=(TimeData&& other) = delete;
    ~TimeData() = default;

  public:
    int64_t getLatestDuration() const;
    int64_t getAverageDuration() const;

  };

  using TimeDataMap = std::unordered_map<std::string, TimeData>;

private:
  std::size_t _preAllocatedHistoricSize;
  TimeDataMap _allTimes;

public:
  PerformanceProfiler(std::size_t inPreAllocatedHistoricSize = 60);
  ~PerformanceProfiler() = default;

public:
  void start(const std::string& name);
  void stop(const std::string& name);
  void set(const std::string& name, int64_t inDuration);
  const TimeDataMap& getTimeDataMap() const;

private:
  TimeData& _getOrCreate(const std::string& name);

};

}
}
