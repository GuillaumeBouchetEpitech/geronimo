
#pragma once

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/helpers/Json.hpp"
#include "geronimo/helpers/GLMath.hpp"

namespace jsonUtils {

  namespace common {
    void propertyExist(const json& inJson, std::string_view inKey);
  }

  namespace uint8 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, uint8_t& outFloat);
  }

  namespace int32 {
    bool is(const json& inJson, std::string_view inKey);
    void tryGet(const json& inJson, std::string_view inKey, int32_t& outVal);
  }

  namespace uint32 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, uint32_t& outFloat);
  }

  namespace float32 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, float& outFloat);
    void tryGet(const json& inJson, std::string_view inKey, float& outFloat);
  }

  namespace fvec2 {
    bool is(const json& inJson);
    void get(const json& inJson, glm::vec2& outVec2);
    void tryGet(const json& inJson, glm::vec2& outVec2);
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::vec2& outVec2);
    void tryGet(const json& inJson, std::string_view inKey, glm::vec2& outVec2);
  }

  namespace fvec3 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::vec3& outVec3);
    void tryGet(const json& inJson, std::string_view inKey, glm::vec3& outVec3);
  }

  namespace fvec4 {
    bool is(const json& inJson);
    void get(const json& inJson, glm::vec4& outVec4);
    void tryGet(const json& inJson, glm::vec4& outVec4);
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::vec4& outVec4);
    void tryGet(const json& inJson, std::string_view inKey, glm::vec4& outVec4);
  }

  namespace uvec3 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::uvec3& outUvec3);
    void tryGet(const json& inJson, std::string_view inKey, glm::uvec3& outUvec3);
  }

  namespace ivec3 {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::ivec3& outIvec3);
    void tryGet(const json& inJson, std::string_view inKey, glm::ivec3& outIvec3);
  }

  namespace str {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, std::string& outStr);
    void tryGet(const json& inJson, std::string_view inKey, std::string& outStr);
  }

  namespace quat {
    bool is(const json& inJson, std::string_view inKey);
    void get(const json& inJson, std::string_view inKey, glm::quat& outQuat);
    void tryGet(const json& inJson, std::string_view inKey, glm::quat& outQuat);
  }

};
