
#pragma once

#include "geronimo/system/containers/dynamic_heap_array.hpp"

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace gero {
namespace metrics {

class HistoricalTimeData {
public:
  HistoricalTimeData(std::size_t inSize = 60U);
  HistoricalTimeData(const HistoricalTimeData& other) = delete;
  HistoricalTimeData(HistoricalTimeData&& other) = delete;
  HistoricalTimeData& operator=(const HistoricalTimeData& other) = delete;
  HistoricalTimeData& operator=(HistoricalTimeData&& other) = delete;
  ~HistoricalTimeData() = default;

public:
  void start();
  void stop();
  void reset();
  void pushNewValue(int32_t inDuration);
  void setSize(std::size_t inSize);

public:
  int32_t getLatestDuration() const;
  int32_t getAverageDuration() const;
  int32_t getMaxDuration() const;
  int32_t getMinDuration() const;
  int32_t getDurationByIndex(std::size_t index) const;
  std::size_t getTotalDurations() const;
  std::size_t getSize() const;

private:
  std::chrono::high_resolution_clock::time_point _start;
  std::chrono::high_resolution_clock::time_point _stop;
  int32_t _latestDuration = 0;
  gero::dynamic_heap_array<int32_t, int32_t> _historicData;
  int32_t _historicIndex = 0;
  int32_t _averageDuration = 0;
  int32_t _maxDuration = 0;
  int32_t _minDuration = 0;

  std::size_t _size = 60U;

};

} // namespace metrics
} // namespace gero
