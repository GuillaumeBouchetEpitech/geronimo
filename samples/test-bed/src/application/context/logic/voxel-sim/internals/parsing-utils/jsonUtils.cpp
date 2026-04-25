
#include "jsonUtils.hpp"

// #include "geronimo/system/ErrorHandler.hpp"
// #include "geronimo/helpers/Json.hpp"
// #include "geronimo/helpers/GLMath.hpp"

namespace jsonUtils {

  // MARK:common
  namespace common {

    void propertyExist(const json& inJson, std::string_view inKey)
    {
      if (!inJson.contains(inKey)) {
        D_THROW(std::invalid_argument, "property '" << inKey << "' is missing");
      }
    }
  }

  // MARK:uint8
  namespace uint8 {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_number_unsigned()
      );
    }

    void get(const json& inJson, std::string_view inKey, uint8_t& outFloat)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a uint8_t -> " << inKey);
      }
      inJson[inKey].get_to(outFloat);
    }

    void tryGet(const json& inJson, std::string_view inKey, uint8_t& outFloat)
    {
      if (is(inJson, inKey)) {
        inJson[inKey].get_to(outFloat);
      }
    }
  }

  // MARK:int32
  namespace int32 {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_number_unsigned()
      );
    }

    void get(const json& inJson, std::string_view inKey, uint32_t& outFloat)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a int32_t -> " << inKey);
      }
      inJson[inKey].get_to(outFloat);
    }

    void tryGet(const json& inJson, std::string_view inKey, int32_t& outVal)
    {
      if (is(inJson, inKey)) {
        inJson[inKey].get_to(outVal);
      }
    }
  }

  // MARK:uint32
  namespace uint32 {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_number_unsigned()
      );
    }

    void get(const json& inJson, std::string_view inKey, uint32_t& outFloat)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a uint32_t -> " << inKey);
      }
      inJson[inKey].get_to(outFloat);
    }

    void tryGet(const json& inJson, std::string_view inKey, int32_t& outVal)
    {
      if (is(inJson, inKey)) {
        inJson[inKey].get_to(outVal);
      }
    }
  }

  // MARK:float32
  namespace float32 {
    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_number()
      );
    }

    void get(const json& inJson, std::string_view inKey, float& outFloat)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a float -> " << inKey);
      }
      inJson[inKey].get_to(outFloat);
    }

    void tryGet(const json& inJson, std::string_view inKey, float& outFloat)
    {
      if (is(inJson, inKey)) {
        inJson[inKey].get_to(outFloat);
      }
    }
  }

  // MARK:fvec3
  namespace fvec3 {
    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_array() &&
        inJson[inKey].size() == 3 &&
        inJson[inKey][0].is_number() &&
        inJson[inKey][1].is_number() &&
        inJson[inKey][2].is_number()
      );
    }

    void get(const json& inJson, std::string_view inKey, glm::vec3& outVec3)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a vec3 -> " << inKey);
      }
      inJson[inKey][0].get_to(outVec3.x);
      inJson[inKey][1].get_to(outVec3.y);
      inJson[inKey][2].get_to(outVec3.z);
    }

    void tryGet(const json& inJson, std::string_view inKey, glm::vec3& outVec3)
    {
      if (is(inJson, inKey)) {
        inJson[inKey][0].get_to(outVec3.x);
        inJson[inKey][1].get_to(outVec3.y);
        inJson[inKey][2].get_to(outVec3.z);
      }
    }
  }

  // MARK:uvec3
  namespace uvec3 {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_array() &&
        inJson[inKey].size() == 3 &&
        inJson[inKey][0].is_number_unsigned() &&
        inJson[inKey][1].is_number_unsigned() &&
        inJson[inKey][2].is_number_unsigned()
      );
    }

    void get(const json& inJson, std::string_view inKey, glm::uvec3& outUvec3)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a uvec3 -> " << inKey);
      }
      inJson[inKey][0].get_to(outUvec3.x);
      inJson[inKey][1].get_to(outUvec3.y);
      inJson[inKey][2].get_to(outUvec3.z);
    }

    void tryGet(const json& inJson, std::string_view inKey, glm::uvec3& outUvec3)
    {
      if (is(inJson, inKey)) {
        inJson[inKey][0].get_to(outUvec3.x);
        inJson[inKey][1].get_to(outUvec3.y);
        inJson[inKey][2].get_to(outUvec3.z);
      }
    }
  }

  // MARK:ivec3
  namespace ivec3 {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_array() &&
        inJson[inKey].size() == 3 &&
        inJson[inKey][0].is_number_integer() &&
        inJson[inKey][1].is_number_integer() &&
        inJson[inKey][2].is_number_integer()
      );
    }

    void get(const json& inJson, std::string_view inKey, glm::ivec3& outIvec3)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a uvec3 -> " << inKey);
      }
      inJson[inKey][0].get_to(outIvec3.x);
      inJson[inKey][1].get_to(outIvec3.y);
      inJson[inKey][2].get_to(outIvec3.z);
    }

    void tryGet(const json& inJson, std::string_view inKey, glm::ivec3& outIvec3)
    {
      if (is(inJson, inKey)) {
        inJson[inKey][0].get_to(outIvec3.x);
        inJson[inKey][1].get_to(outIvec3.y);
        inJson[inKey][2].get_to(outIvec3.z);
      }
    }
  }

  // MARK:str
  namespace str {

    bool is(const json& inJson, std::string_view inKey)
    {
      return (
        inJson.contains(inKey) &&
        inJson[inKey].is_string()
      );
    }

    void get(const json& inJson, std::string_view inKey, std::string& outStr)
    {
      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a string -> " << inKey);
      }
      inJson[inKey].get_to(outStr);
    }

    void tryGet(const json& inJson, std::string_view inKey, std::string& outStr)
    {
      if (is(inJson, inKey)) {
        inJson[inKey].get_to(outStr);
      }
    }
  }

  // MARK:quat
  namespace quat {

    bool is(const json& inJson, std::string_view inKey) {
      if (
        !inJson.contains(inKey) ||
        !inJson[inKey].is_array() // ||
        // inJson[inKey].size() == 0
      ) {
        return false;
      }

      auto operations = inJson[inKey];
      for (auto& currOp : operations) {

        if (
          !float32::is(currOp, "angle") ||
          !fvec3::is(currOp, "axis")
        ) {
          return false;
          // D_THROW(std::invalid_argument, "not a quat -> " << inKey);
        }
      }
      return true;
    }

    void get(const json& inJson, std::string_view inKey, glm::quat& outQuat) {

      if (!is(inJson, inKey)) {
        D_THROW(std::invalid_argument, "not a quat -> " << inKey);
      }

      outQuat = glm::identity<glm::quat>();
      // if (
      //   inJson.contains(inKey) &&
      //   inJson[inKey].is_array() &&
      //   inJson[inKey].size() > 0
      // ) {
        auto operations = inJson[inKey];
        for (auto& currOp : operations) {

          float angle = 0.0f;
          glm::vec3 axis = glm::vec3(0,0,0);

          float32::get(currOp, "angle", angle);
          fvec3::get(currOp, "axis", axis);

          outQuat *= glm::angleAxis(angle, axis);
        }
      // }
    }

    void tryGet(const json& inJson, std::string_view inKey, glm::quat& outQuat) {

      if (!is(inJson, inKey)) {
        // D_THROW(std::invalid_argument, "not a quat -> " << inKey);
        return;
      }

      outQuat = glm::identity<glm::quat>();
      // if (
      //   inJson.contains(inKey) &&
      //   inJson[inKey].is_array() &&
      //   inJson[inKey].size() > 0
      // ) {
        auto operations = inJson[inKey];
        for (auto& currOp : operations) {

          float angle = 0.0f;
          glm::vec3 axis = glm::vec3(0,0,0);

          float32::get(currOp, "angle", angle);
          fvec3::get(currOp, "axis", axis);

          outQuat *= glm::angleAxis(angle, axis);
        }
      // }
    }
  }

};
