
#pragma once

#include "FileLoadFileCallback.hpp"

#include "geronimo/system/containers/weak_ref_data_pool.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

namespace gero {
namespace fileUtils {

class DataBuffer : public gero::weak_ref_data_pool_base_class {
private:
  std::size_t _size;
  std::unique_ptr<std::uint8_t[]> _data;

public:
  DataBuffer(const std::uint8_t* inData, std::size_t inSize);
  DataBuffer(const std::int8_t* inData, std::size_t inSize);
  DataBuffer(const std::uint8_t* str);
  DataBuffer(const std::int8_t* str);
  DataBuffer(const std::string& str);

  virtual ~DataBuffer() = default;

public:
  DataBuffer(const DataBuffer& other) = delete;            // no copy
  DataBuffer& operator=(const DataBuffer& other) = delete; // no copy

public:
  DataBuffer(DataBuffer&& other) = default;            // allow move
  DataBuffer& operator=(DataBuffer&& other) = default; // allow move

public:
  const uint8_t* getUCharData() const;
  const char* getCharData() const;
  std::size_t getSize() const;
  const std::string_view asStringView() const;
  void fillString(std::string& outFileContent) const;
};

using BuffersPool = safe_weak_ref_data_pool<DataBuffer, DataBuffer, 256, false>;
using BufferWeakRef = BuffersPool::weak_ref;

class FileManager {

private:
  BuffersPool _buffersPool;
  std::unordered_map<std::string, BufferWeakRef> _buffersMap;

public:
  FileManager() = default;
  ~FileManager() = default;

public:
  void load(const std::string& filename);

  BufferWeakRef getDataBuffer(const std::string& filename);
};

LoadCallback getFileManagerCallback(FileManager& fileManager);

} // namespace fileUtils
} // namespace gero
