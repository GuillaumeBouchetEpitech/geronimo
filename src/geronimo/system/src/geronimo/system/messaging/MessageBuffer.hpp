
#pragma once

#include "AbstractMessage.hpp"

#include <cstdint>
#include <vector>

namespace gero {
namespace messaging {

class MessageBuffer : public AbstractMessage {
private:
  std::vector<char> _dataContainer;

public:
  MessageBuffer();

public:
  MessageBuffer& append(const void* dataPointer, uint32_t size);

public:
  virtual void clear() override;

public:
  MessageBuffer& writeBoolean(bool data);
  MessageBuffer& writeInt8(int8_t data);
  MessageBuffer& writeUint8(uint8_t data);
  MessageBuffer& writeInt16(int16_t data);
  MessageBuffer& writeUint16(uint16_t data);
  MessageBuffer& writeInt32(int32_t data);
  MessageBuffer& writeUint32(uint32_t data);
  MessageBuffer& writeInt64(int64_t data);
  MessageBuffer& writeUint64(uint64_t data);
  MessageBuffer& writeFloat(float data);
  MessageBuffer& writeDouble(double data);
  MessageBuffer& writeString(const std::string& data);
  MessageBuffer& writeVec3(const glm::vec3& data);
  MessageBuffer& writeVec4(const glm::vec4& data);
  MessageBuffer& writeQuat(const glm::quat& data);
  MessageBuffer& writeMat4(const glm::mat4& data);

public:
  MessageBuffer& operator<<(bool data);
  MessageBuffer& operator<<(int8_t data);
  MessageBuffer& operator<<(uint8_t data);
  MessageBuffer& operator<<(int16_t data);
  MessageBuffer& operator<<(uint16_t data);
  MessageBuffer& operator<<(int32_t data);
  MessageBuffer& operator<<(uint32_t data);
  MessageBuffer& operator<<(int64_t data);
  MessageBuffer& operator<<(uint64_t data);
  MessageBuffer& operator<<(float data);
  MessageBuffer& operator<<(double data);
  MessageBuffer& operator<<(const std::string& data);
  MessageBuffer& operator<<(const glm::vec3& data);
  MessageBuffer& operator<<(const glm::vec4& data);
  MessageBuffer& operator<<(const glm::quat& data);
  MessageBuffer& operator<<(const glm::mat4& data);
};

} // namespace messaging
} // namespace gero
