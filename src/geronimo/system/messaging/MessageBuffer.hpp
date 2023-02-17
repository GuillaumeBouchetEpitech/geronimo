
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
  MessageBuffer& operator<<(bool data);
  MessageBuffer& operator<<(char data);
  MessageBuffer& operator<<(unsigned char data);
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
