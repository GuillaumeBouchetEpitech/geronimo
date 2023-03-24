
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
