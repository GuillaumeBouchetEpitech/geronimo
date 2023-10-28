
#include "MessageBuffer.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cstring> // <= std::memcpy

namespace gero {
namespace messaging {

MessageBuffer::MessageBuffer() {
  /**
   * grow the cached memory block
   * => it reallocate when needed, will only grow () (not on it's own anyway)
   * => it do so with a power of 2 value (1, 2, 4, 8, 16, etc.)
   * => starting it at a "high" value (1024) make that part faster
   */
  _dataContainer.reserve(1024); // pre-allocate
}

MessageBuffer& MessageBuffer::append(const void* dataPointer, uint32_t dataSize) {
  if (dataPointer && dataSize > 0) {
    // this will cause a reallocation if not large enough
    _dataContainer.resize(_dataSize + dataSize);

    /**
     * this is done in case _dataContainer needed to reallocate
     * => the pointer value would differ
     */
    _dataPointer = _dataContainer.data();

    // _dataSize is holding it's pre-resize value
    std::memcpy(_dataPointer + _dataSize, dataPointer, dataSize);

    // update the _dataSize value
    _dataSize = uint32_t(_dataContainer.size());
  }

  return *this;
}

void MessageBuffer::clear() {
  AbstractMessage::clear();
  _dataContainer.clear(); // <= clear the used size but keep it's (cached) capacity
}

//
//
//

MessageBuffer& MessageBuffer::writeBoolean(bool data) { return writeInt8(data ? 1 : 0); }
MessageBuffer& MessageBuffer::writeInt8(int8_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeUint8(uint8_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeInt16(int16_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeUint16(uint16_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeInt32(int32_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeUint32(uint32_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeInt64(int64_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeUint64(uint64_t data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeFloat(float data) { return append(&data, sizeof(data)); }
MessageBuffer& MessageBuffer::writeDouble(double data) { return append(&data, sizeof(data)); }

MessageBuffer& MessageBuffer::writeString(const std::string& data) {
  if (data.empty()) {
    D_THROW(std::runtime_error, "can't append an empty string");
  }

  const uint32_t length = uint32_t(data.size());
  const char* rawString = data.c_str();
  writeUint32(length);
  append(rawString, length * sizeof(char));

  return *this;
}

MessageBuffer& MessageBuffer::writeVec3(const glm::vec3& data) { return append(&data.x, sizeof(glm::vec3)); }
MessageBuffer& MessageBuffer::writeVec4(const glm::vec4& data) { return append(&data.x, sizeof(glm::vec4)); }
MessageBuffer& MessageBuffer::writeQuat(const glm::quat& data) { return append(&data.x, sizeof(glm::quat)); }
MessageBuffer& MessageBuffer::writeMat4(const glm::mat4& data) { return append(glm::value_ptr(data), sizeof(glm::mat4)); }

//
//
//

MessageBuffer& MessageBuffer::operator<<(bool data) { return writeBoolean(data); }
MessageBuffer& MessageBuffer::operator<<(int8_t data) { return writeInt8(data); }
MessageBuffer& MessageBuffer::operator<<(uint8_t data) { return writeUint8(data); }
MessageBuffer& MessageBuffer::operator<<(int16_t data) { return writeInt16(data); }
MessageBuffer& MessageBuffer::operator<<(uint16_t data) { return writeUint16(data); }
MessageBuffer& MessageBuffer::operator<<(int32_t data) { return writeInt32(data); }
MessageBuffer& MessageBuffer::operator<<(uint32_t data) { return writeUint32(data); }
MessageBuffer& MessageBuffer::operator<<(int64_t data) { return writeInt64(data); }
MessageBuffer& MessageBuffer::operator<<(uint64_t data) { return writeUint64(data); }
MessageBuffer& MessageBuffer::operator<<(float data) { return writeFloat(data); }
MessageBuffer& MessageBuffer::operator<<(double data) { return writeDouble(data); }
MessageBuffer& MessageBuffer::operator<<(const std::string& data) { return writeString(data); }
MessageBuffer& MessageBuffer::operator<<(const glm::vec3& data) { return writeVec3(data); }
MessageBuffer& MessageBuffer::operator<<(const glm::vec4& data) { return writeVec4(data); }
MessageBuffer& MessageBuffer::operator<<(const glm::quat& data) { return writeQuat(data); }
MessageBuffer& MessageBuffer::operator<<(const glm::mat4& data) { return writeMat4(data); }

} // namespace messaging
} // namespace gero
