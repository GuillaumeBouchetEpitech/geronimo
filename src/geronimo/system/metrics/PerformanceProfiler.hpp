
#pragma once

#include "HistoricalTimeData.hpp"

#include "geronimo/system/containers/dynamic_heap_array.hpp"

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace gero {
namespace metrics {

class PerformanceProfiler {
public:
  using HistoricalTimeDataMap = std::unordered_map<std::string, HistoricalTimeData>;

public:
  PerformanceProfiler(std::size_t inPreAllocatedHistoricSize = 60);
  ~PerformanceProfiler() = default;

public:
  void start(const std::string& name);
  void stop(const std::string& name);
  void set(const std::string& name, int32_t inDuration);

public:
  const HistoricalTimeDataMap& getHistoricalTimeDataMap() const;

public:
  using MaybeTimeDataRef = std::optional<std::reference_wrapper<const HistoricalTimeData>>;

  const MaybeTimeDataRef tryGetTimeData(const std::string& name) const;

public:
  const std::vector<std::string>& getAllDataKeys() const;

  std::size_t getHistoricSize() const;

private:
  HistoricalTimeData& _getOrCreate(const std::string& name);

private:
  std::size_t _preAllocatedHistoricSize;
  HistoricalTimeDataMap _allTimes;
  std::vector<std::string> _allDataKeys;
};

} // namespace metrics
} // namespace gero
