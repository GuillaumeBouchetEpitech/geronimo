
#include "MessageView.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cstring> // <= std::memcpy
#include <memory>  // <= std::make_unique

namespace gero {
namespace messaging {

MessageView::MessageView(const char* dataPointer, uint32_t dataSize) : AbstractMessage(dataPointer, dataSize) {}

//

void MessageView::clear() {
  AbstractMessage::clear();
  _readIndex = 0;
}

//

MessageView& MessageView::read(void* dataPointer, uint32_t dataSize) {
  // verify size left
  if (_readIndex + dataSize > _dataSize)
    D_THROW(std::runtime_error,
            "can't read, not enough size left"
              << ", current index=" << _readIndex << ", next index=" << (_readIndex + dataSize)
              << ", data size=" << _dataSize);

  std::memcpy(dataPointer, _dataPointer + _readIndex, dataSize);
  _readIndex += dataSize;
  return *this;
}

//

MessageView& MessageView::readBoolean(bool& data) {
  int8_t value = 0;
  readInt8(value); // <= read like a char
  data = (value != 0);
  return *this;
}
MessageView& MessageView::readInt8(int8_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readUint8(uint8_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readInt16(int16_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readUint16(uint16_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readInt32(int32_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readUint32(uint32_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readInt64(int64_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readUint64(uint64_t& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readFloat(float& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readDouble(double& data) { return read(&data, sizeof(data)); }
MessageView& MessageView::readString(std::string& data) {
  // read string length
  uint32_t stringLength = 0;
  readUint32(stringLength);

  // read string characters
  auto bufferPtr = std::make_unique<char[]>(stringLength + 1);
  char* rawBuffer = bufferPtr.get();
  std::memset(rawBuffer, 0, stringLength + 1);
  read(rawBuffer, stringLength * sizeof(char));

  data = rawBuffer;

  return *this;
}
MessageView& MessageView::readVec3(glm::vec3& data) { return read(&data.x, sizeof(glm::vec3)); }
MessageView& MessageView::readVec4(glm::vec4& data) { return read(&data.x, sizeof(glm::vec4)); }
MessageView& MessageView::readQuat(glm::quat& data) { return read(&data.x, sizeof(glm::quat)); }
MessageView& MessageView::readMat4(glm::mat4& data) { return read(glm::value_ptr(data), sizeof(glm::mat4)); }

MessageView& MessageView::operator>>(bool& data) { return readBoolean(data); }
MessageView& MessageView::operator>>(int8_t& data) { return readInt8(data); }
MessageView& MessageView::operator>>(uint8_t& data) { return readUint8(data); }
MessageView& MessageView::operator>>(int16_t& data) { return readInt16(data); }
MessageView& MessageView::operator>>(uint16_t& data) { return readUint16(data); }
MessageView& MessageView::operator>>(int32_t& data) { return readInt32(data); }
MessageView& MessageView::operator>>(uint32_t& data) { return readUint32(data); }
MessageView& MessageView::operator>>(int64_t& data) { return readInt64(data); }
MessageView& MessageView::operator>>(uint64_t& data) { return readUint64(data); }
MessageView& MessageView::operator>>(float& data) { return readFloat(data); }
MessageView& MessageView::operator>>(double& data) { return readDouble(data); }
MessageView& MessageView::operator>>(std::string& data) { return readString(data); }
MessageView& MessageView::operator>>(glm::vec3& data) { return readVec3(data); }
MessageView& MessageView::operator>>(glm::vec4& data) { return readVec4(data); }
MessageView& MessageView::operator>>(glm::quat& data) { return readQuat(data); }
MessageView& MessageView::operator>>(glm::mat4& data) { return readMat4(data); }

} // namespace messaging
} // namespace gero
