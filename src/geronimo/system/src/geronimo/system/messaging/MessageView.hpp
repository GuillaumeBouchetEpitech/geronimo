
#pragma once

#include "AbstractMessage.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>

namespace gero {
namespace messaging {

class MessageView : public AbstractMessage {
private:
  uint32_t _readIndex = 0;

public:
  MessageView() = default;
  MessageView(const char* dataPointer, uint32_t size);

public:
  virtual void clear() override;

public:
  MessageView& read(void* dataPointer, uint32_t size);

public:
  MessageView& readBoolean(bool& data);
  MessageView& readInt8(int8_t& data);
  MessageView& readUint8(uint8_t& data);
  MessageView& readInt16(int16_t& data);
  MessageView& readUint16(uint16_t& data);
  MessageView& readInt32(int32_t& data);
  MessageView& readUint32(uint32_t& data);
  MessageView& readInt64(int64_t& data);
  MessageView& readUint64(uint64_t& data);
  MessageView& readFloat(float& data);
  MessageView& readDouble(double& data);
  MessageView& readString(std::string& data);
  MessageView& readVec3(glm::vec3& data);
  MessageView& readVec4(glm::vec4& data);
  MessageView& readQuat(glm::quat& data);
  MessageView& readMat4(glm::mat4& data);

public:
  MessageView& operator>>(bool& data);
  MessageView& operator>>(int8_t& data);
  MessageView& operator>>(uint8_t& data);
  MessageView& operator>>(int16_t& data);
  MessageView& operator>>(uint16_t& data);
  MessageView& operator>>(int32_t& data);
  MessageView& operator>>(uint32_t& data);
  MessageView& operator>>(int64_t& data);
  MessageView& operator>>(uint64_t& data);
  MessageView& operator>>(float& data);
  MessageView& operator>>(double& data);
  MessageView& operator>>(std::string& data);
  MessageView& operator>>(glm::vec3& data);
  MessageView& operator>>(glm::vec4& data);
  MessageView& operator>>(glm::quat& data);
  MessageView& operator>>(glm::mat4& data);
};

} // namespace messaging
} // namespace gero
