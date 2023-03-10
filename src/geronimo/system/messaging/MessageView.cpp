
#include "MessageView.hpp"

#include "geronimo/system/ErrorHandler.hpp"

#include <cstring> // <= std::memcpy
#include <memory>  // <= std::make_unique

namespace gero {
namespace messaging {

MessageView::MessageView(const char* dataPointer, unsigned int dataSize)
  : AbstractMessage(dataPointer, dataSize) {}

//

void MessageView::clear() {
  AbstractMessage::clear();
  _readIndex = 0;
}

//

MessageView& MessageView::read(void* dataPointer, unsigned int dataSize) {
  // verify size left
  if (_readIndex + dataSize > _dataSize)
    D_THROW(std::runtime_error, "can't read, not enough size left"
                                  << ", current index=" << _readIndex
                                  << ", next index=" << (_readIndex + dataSize)
                                  << ", data size=" << _dataSize);

  std::memcpy(dataPointer, _dataPointer + _readIndex, dataSize);
  _readIndex += dataSize;
  return *this;
}

//

MessageView& MessageView::operator>>(bool& data) {
  char value = 0;
  read(&value, sizeof(char)); // <= read like a char
  data = (value != 0);
  return *this;
}

MessageView& MessageView::operator>>(char& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(unsigned char& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(int16_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(uint16_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(int32_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(uint32_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(int64_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(uint64_t& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(float& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(double& data) {
  return read(&data, sizeof(data));
}

MessageView& MessageView::operator>>(std::string& data) {
  // read string length
  unsigned int stringLength = 0;
  read(&stringLength, sizeof(stringLength));

  // read string characters
  auto bufferPtr = std::make_unique<char[]>(stringLength + 1);
  char* rawBuffer = bufferPtr.get();
  std::memset(rawBuffer, 0, stringLength + 1);
  read(rawBuffer, stringLength * sizeof(char));

  data = rawBuffer;

  return *this;
}

MessageView& MessageView::operator>>(glm::vec3& data) {
  return read(&data.x, sizeof(glm::vec3));
}

MessageView& MessageView::operator>>(glm::vec4& data) {
  return read(&data.x, sizeof(glm::vec4));
}

MessageView& MessageView::operator>>(glm::quat& data) {
  return read(&data.x, sizeof(glm::quat));
}

MessageView& MessageView::operator>>(glm::mat4& data) {
  return read(glm::value_ptr(data), sizeof(glm::mat4));
}

} // namespace messaging
} // namespace gero
