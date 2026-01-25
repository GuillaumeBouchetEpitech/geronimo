
#pragma once

#include "GLMath.hpp"

#include <unordered_map>
#include <unordered_set>

//
//
//

namespace tests_ivec2 {

struct hash_key_funcs {
  size_t operator()(const glm::ivec2& k) const { return std::hash<int>()(k.x) ^ std::hash<int>()(k.y); }

  bool operator()(const glm::ivec2& a, const glm::ivec2& b) const { return a.x == b.x && a.y == b.y; }
};

using hash_set = std::unordered_set<glm::ivec2, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::ivec2, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_ivec2

//
//
//

namespace tests_uvec2 {

struct hash_key_funcs {
  size_t operator()(const glm::uvec2& k) const { return std::hash<uint32_t>()(k.x) ^ std::hash<uint32_t>()(k.y); }

  bool operator()(const glm::uvec2& a, const glm::uvec2& b) const { return a.x == b.x && a.y == b.y; }
};

using hash_set = std::unordered_set<glm::uvec2, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::uvec2, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_uvec2

//
//
//

namespace tests_vec2 {

struct hash_key_funcs {
  size_t operator()(const glm::vec2& k) const { return std::hash<float>()(k.x) ^ std::hash<float>()(k.y); }

  bool operator()(const glm::vec2& a, const glm::vec2& b) const { return a.x == b.x && a.y == b.y; }
};

using hash_set = std::unordered_set<glm::vec2, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::vec2, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_vec2

//
//
//

namespace tests_ivec3 {

struct hash_key_funcs {
  size_t operator()(const glm::ivec3& k) const {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
  }

  bool operator()(const glm::ivec3& a, const glm::ivec3& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

using hash_set = std::unordered_set<glm::ivec3, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::ivec3, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_ivec3

//
//
//

namespace tests_uvec3 {

struct hash_key_funcs {
  size_t operator()(const glm::uvec3& k) const {
    return std::hash<uint32_t>()(k.x) ^ std::hash<uint32_t>()(k.y) ^ std::hash<uint32_t>()(k.z);
  }

  bool operator()(const glm::uvec3& a, const glm::uvec3& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

using hash_set = std::unordered_set<glm::uvec3, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::uvec3, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_uvec3

//
//
//

namespace tests_vec3 {

struct hash_key_funcs {
  size_t operator()(const glm::vec3& k) const {
    return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
  }

  bool operator()(const glm::vec3& a, const glm::vec3& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

using hash_set = std::unordered_set<glm::vec3, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::vec3, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_vec3

//
//
//

namespace tests_ivec4 {

struct hash_key_funcs {
  size_t operator()(const glm::ivec4& k) const {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z) ^ std::hash<int>()(k.w);
  }

  bool operator()(const glm::ivec4& a, const glm::ivec4& b) const {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
  }
};

using hash_set = std::unordered_set<glm::ivec4, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::ivec4, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_ivec4

//
//
//

namespace tests_uvec4 {

struct hash_key_funcs {
  size_t operator()(const glm::uvec4& k) const {
    return std::hash<uint32_t>()(k.x) ^ std::hash<uint32_t>()(k.y) ^ std::hash<uint32_t>()(k.z) ^
           std::hash<uint32_t>()(k.w);
  }

  bool operator()(const glm::uvec4& a, const glm::uvec4& b) const {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
  }
};

using hash_set = std::unordered_set<glm::uvec4, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::uvec4, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_uvec4

//
//
//

namespace tests_vec4 {

struct hash_key_funcs {
  size_t operator()(const glm::vec4& k) const {
    return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z) ^ std::hash<float>()(k.w);
  }

  bool operator()(const glm::vec4& a, const glm::vec4& b) const {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
  }
};

using hash_set = std::unordered_set<glm::vec4, hash_key_funcs, hash_key_funcs>;

template <typename T> using hash_map = std::unordered_map<glm::vec4, T, hash_key_funcs, hash_key_funcs>;

} // namespace tests_vec4

//
//
//
